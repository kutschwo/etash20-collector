
// main.c
//
// based on work of (c) Hewell Technology Ltd. 2014
//
// modified by Tobias Tangemann 2020
// Wolfgang Kutscherauer 2024
// reused to deal with wood-heating ETA SH20
//
// last change 06.05.2024 by Wolfgang Kutscherauer kutschi@dingolfing.org
//
//****************************************************************************
// system includes
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

//project includes
#include "config.h"
#include "datatypes.h"
#include "kbhit.h"
#include "serial.h"
#include "etash20.h"
#include "mqtt.h"
#ifdef __HOASTNT__
    #include "homeassistant.h"
#endif
#ifdef __SQLITE__
    #include "sqlite.h"
#endif

#if !defined(GIT_VERSION)
    #define GIT_VERSION "unknown"
#endif

char serial_buffer[256];

CONFIG* maincfg;

//Deklaration der Funktion um einschalten des "gesprächigen" Modus


int main(int argc, char *argv[])
{
    printf("etash20-collector "GIT_VERSION"\n");

    Data_Packet packet;
    //PVBUS_V1_CMD pPacket = (PVBUS_V1_CMD)&serial_buffer[0];
    unsigned char i = 0;
    int framedata = 0;          // 1 solange Daten eines Frames empfangen werden
    int frameready = 0;         // sobald } empfangen wird ist der Frame fertig
    int loopforever = 0;        // wenn das Programm als Dienst läuft = 1 für Endlos-Betrieb.
    int packet_displayed = 0;
    bool firstLoop = true;      // wenn das Programm das 1. mal durchlaufen wird --> Einrichtung DB, etc.

//Initialisierung der der CONFIG-Struktur, alles auf 0, false, NULL
// damit der Initial-Zustand definiert ist.
    CONFIG cfg = {
        .device = NULL,
        .delay = 0,

        .database = NULL,
        .withSql = false,

        .print_result = true,
        .loglevel = 4,

        .mqtt_enabled = false,
        .mqtt_user = NULL,
        .mqtt_password = NULL,
        .mqtt_server = NULL,
        .mqtt_sensor_base = NULL,
        .mqtt_actor_base = NULL,
        .mqtt_client_id = NULL,

        .homeassistant_enabled = false,
        .homeassistant_entity_id_base = NULL
    };

    maincfg = &cfg;

// Wenn die Kommandozeile aus mehr als 2 Objekten (Kommando +  ein Parameter) besteht --> parsen
    if (argc > 2)
    {
// parse command line options
// Schleife über das Array der Zeiger auf die Parameter
        for (int idx = 1; idx < argc; ++idx)
        {
            char *option = argv[idx];
// 
            if (strcmp("-f", option)==0 || strcmp("--forever", option)==0)
            {
                loopforever = true;
        // if set repeat forever
            }

            if (strcmp("-v", option) == 0 || strcmp("--verbose", option) == 0)
            {
                cfg.loglevel = 2;
                log_set_level(LOG_INFO);
            // if set, activate detailed output 
            }

            if (strcmp("-m", option) == 0 || strcmp("--mqtt", option) == 0)
            {
                cfg.mqtt_enabled = true;
    // if set enable mqtt 
            }

            if (strcmp("-d", option)==0 || strcmp("--delay", option)==0)
            // define the delay between reading Vbus data
            {
                if (argc <= idx + 2) {
                    printf("Missing value for delay option\n");
                    return 4;
                }

                // Use next option as delay value
                idx++;
                cfg.delay = strtoul(argv[idx], NULL, 10);
            }

            #ifdef __SQLITE__
                if (strcmp("--db", option)==0)
                {
                    if (argc <= idx + 2)
                    {
                        printf("Missing value for sqlite db path\n");
                        return 5;
                    }

                    // Use next option as path to sqlite database
                    idx++;
                    cfg.database = argv[idx];
                }
            #endif

            if (strcmp("--no-print", option) == 0)
            // supress any output to stdout / console
            {
                cfg.print_result = false;
            }

            if (strcmp("-c", option) == 0 || strcmp("--config", option) == 0)
            {
                #ifndef __JSON__
                    printf("Config file is not supported");
                    return 7;
                #else
                    if (argc <= idx + 1)
                    {
                        printf("Missing config file\n");
                        return 7;
                    }

                    // Use next option as file name/path
                    idx++;

                    if (parseConfig(argv[idx], &cfg) != 0)
                    {
                        printf("Error parsing config file\n");
                        return 7;
                    }

                #endif
            }
        }
    }

    // last option is the serial port if no config file is used
    if (cfg.device == NULL)
    {
        if (argc > 2)
        {
            cfg.device = argv[argc - 1];
        }
        else
        {
            printf("No serial port set");
            return 2;
        }
    }
#ifdef __HOASTNT__
    if (cfg.homeassistant_enabled)
    {
        if (!homeassistant_init(&cfg))
        {
            printf("Error initializing homeassistant");
            return 20;
        }
    }
#endif
start:
    i = 0; framedata = 0; packet_displayed = 0; frameready = 0;
    // set index in serial_buffer, sync byte not received, count of published packets, end of data flag

    // open serial connection (fn from serial.c)
    if (!serial_open_port(cfg.device))
    {
        printf("Errno(%d) opening serial port %s: %s\n", errno, cfg.device, strerror(errno));
        return 2;
    }
    if (!serial_set_baud_rate(19200))
    {
        printf("Failed to set baud rate: %s\n", serial_get_error());
        return 3;
    }
        // if the filename for sqlite db ist given, open it.
    if (cfg.database != NULL)
    {
      log_info("Opening database %s", cfg.database);
      if (!sqlite_open(cfg.database))
      {
        return 6;
      }

      if (firstLoop) // at first run of the main loop check for db, etc.
      {
        sqlite_create_table();            
      }
        cfg.withSql = true;
    }    
    log_info("Setting baudrate...", "")

    if (cfg.mqtt_enabled)
    {
        if (cfg.loglevel == 2)
        {
            printf("Connecting to mqtt server...\n");
        }

    	reconnect_mqtt(&cfg);
    }

    if (cfg.loglevel == 2)
    {
        printf("Collecting data...\n");
    }
// start main loop
    do
    {    
// exit when got Ctrl C
        if (caughtSigQuit(log_set_level(LOG_INFO))
        {
            break;
        }
        do // START do .. while (frameready == 0) 
        {
        // try to read one byte from serial
          int count = serial_read(&(serial_buffer[i]), 1);
          if (count < 1)
        // if no byte received wait 50 ms
          {
            // sleep 50ms
            nanosleep((const struct timespec[]){{.tv_sec = 0, .tv_nsec = 50000000L}}, NULL);            
            continue;
          }
          // if the received byte = { it is the start of a data packet
          // write it to byte 0 of buffer and set header sync to 1
          if (serial_buffer[i]  == '{')
          {
            serial_buffer[0] = serial_buffer[i];
            i=0;
            framedata = 1;
            if (cfg.loglevel == 2)
            {
                printf("\nStart Byte { received.\n");
            }
            // print Byte when verbose is on, line wrap after 16 bytes
            if (cfg.loglevel == 2)
            {
              printf("%02x ", serial_buffer[i]);
              if (i != 0 && i % 16 == 0)
              {
                printf("\n");
              }
            } // END if (cfg.verbose)
        

           }  // END if (serial_buffer[i]  == '{')
           if (framedata == 1 && serial_buffer[i] == '}')
           {
             frameready = 1;
           }
           i++;  
        } while (frameready == 0) ; // END do .. while (frameready == 0) 
        if ( cfg.logelvel == 2)
          {
            printf("\nSH20 frame received\n");
          }
#if __SQLITE__
          if (cfg.withSql)
          {
            if (cfg.loglevel == 2) 
            {
              printf("\nWriting to database\n");
            }
            sqlite_insert_data(&packet);
          }
#endif
          if (cfg.print_result)
          {
            printf("\nPrint_result nicht implementier.\n");
          }
                          if (cfg.mqtt_enabled)
                {
                    if (loglevel == 2) 
                    {
                        printf("\nPublishing to mqtt broker\n");
                    }
                    // publish_mqtt("tkol", packet.DSECtrlPkt.TempSensor01 * 0.1, "%.1f");
                    printf("\nPublishing to mqtt broker nicht implementier.\n");
          } 
          
          
#ifdef __HOASTNT__
                if (cfg.homeassistant_enabled)
                {
                    if (cfg.loglevel == 2) 
                    {
                        printf("\nUpdating homeassistant\n");
                    }

                    publish_homeassistant(&cfg, &packet);
                } //end if (cfg.homeassistant_enabled)
#endif

    } while (loopforever == true || packet_displayed == 0); //END of main do .. while loop

    serial_close_port();

    #if __SQLITE__
        sqlite_close();
    #endif

    if (cfg.delay > 0)
    {
        if (cfg.delay == 60)
        {
            time_t rawtime;
            struct tm * timeinfo;

            time (&rawtime);
            timeinfo = localtime (&rawtime);

            if (timeinfo->tm_sec < 59)
            {
                sleep(cfg.delay - timeinfo->tm_sec);
            }
            else
            {
                sleep(cfg.delay);
            }
        }
        else
        {
            sleep(cfg.delay);
        }

        firstLoop = false;
        goto start;
    }
#ifdef __HOASTNT__
    if (cfg.homeassistant_enabled)
    {
        homeassistant_cleanup();
    }
#endif
    return 0;
}


