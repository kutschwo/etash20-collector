// main.c
//
// based on work of (c) Hewell Technology Ltd. 2014
//
// modified by Tobias Tangemann 2020
// Wolfgang Kutscherauer 2024
// reused to deal with wood-heating ETA SH20
//
// last change 27.04.2025 by Wolfgang Kutscherauer kutschi@dingolfing.org
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
#include <termios.h>

//project includes
#include "config.h"
#include "datatypes.h"
#include "kbhit.h"
#include "serial.h"
#include "etash20.h"
#include "mqtt.h"
#include "sh20prnfuncs.h"
#include "log.h"
#ifdef __SQLITE__
    #include "sqlite.h"
#endif

#if !defined(GIT_VERSION)
    #define GIT_VERSION "unknown"
#endif

char serial_buffer[256];

int tty_fd;

// Pointer to config
CONFIG* maincfg;
// Struct for conifg settings
CONFIG cfg;
// String for filename of json config-file
char CfgFileName[256];
// length of argument string
int len;
// LogFile
FILE *logfptr;
// Num of byte written to serial device
ssize_t written;
// integer return value
int result;

// need helping function as void 
void enableVerbose();


int main(int argc, char *argv[])
{
;
    printf("etash20-collector "GIT_VERSION"\n");
    log_info("etash20-collector "GIT_VERSION"");

    Data_Packet packet;
    //PVBUS_V1_CMD pPacket = (PVBUS_V1_CMD)&serial_buffer[0];
    unsigned char i = 0;
    int framedata = 0;          // 1 solange Daten eines Frames empfangen werden
    int frameready = 0;         // sobald } empfangen wird ist der Frame fertig
    int loopforever = 0;        // wenn das Programm als Dienst läuft = 1 für Endlos-Betrieb.
    int packet_displayed = 0;
    bool firstLoop = true;      // wenn das Programm das 1. mal durchlaufen wird --> Einrichtung DB, etc.
    
    log_trace("Num args: %d",argc);
    
// parse command line options
    for (int idx = 0; idx < argc; ++idx)
    {
        char *option = argv[idx];
        log_trace("argv[%d]: %s", argv[idx]);
        if (strcmp("-c", option) == 0 || strcmp("--config", option) == 0)
        {
               // printf("Config file is not supported\\");
               // return 7;
                if (argc <= idx + 1)
                {
                    printf("Missing config file\n");
                    log_fatal("Missing config file");
                    return 7;
                }
                // Use next option as file name/path
                idx++;
                len = strlen(argv[idx]);
                log_trace("argv[%d]: %s", idx, argv[idx]);
                strncpy(CfgFileName,argv[idx],len);
                log_trace("CfgFileName: %s", CfgFileName);
                CfgFileName[len]='\0';
                log_trace("Config File: %s",CfgFileName);
                //printf("ConfigFile: %s\n\n", CfgFileName);

        }// END if for --config
            
        if (strcmp("-h", option)==0 || strcmp("--help", option)==0)
        {
            print_help();
            return 0;
        } // END if for help message
    } // END for (int idx = 1; idx < argc; ++idx)

//Initialisierung der der CONFIG-Struktur, alles auf 0, false, NULL
// damit der Initial-Zustand definiert ist.
    cfg.device = NULL;
    cfg.delay = 0;
    cfg.database = NULL;
    cfg.withSql = false;
    cfg.print_result = true;
    cfg.loglevel = 4;
    cfg.logfile = NULL;
    cfg.mqtt_enabled = false;
    cfg.mqtt_user = NULL;
    cfg.mqtt_password = NULL;
    cfg.mqtt_server = NULL;
    cfg.mqtt_sensor_base = NULL;
    cfg.mqtt_actor_base = NULL;
    cfg.mqtt_client_id = NULL;

// set pointer to cfg struct
maincfg = &cfg;

// parse the JSON config file using parseConfig from config.c
    if (parseConfig(CfgFileName, &cfg) != 0)
    {
        printf("Error parsing config file\n");
        log_fatal("Fatal error parsing config file %s",CfgFileName);
        print_help();
        return 7;
    }
    
    log_set_level(cfg.loglevel);
    logfptr = fopen(cfg.logfile, "w");
    result = log_add_fp(logfptr, cfg.loglevel);
    log_trace("open tty: ", cfg.device);
  //  result = tty_open("/dev/ttyUSB0\0") ;

start:
    i = 0; framedata = 0; packet_displayed = 0; frameready = 0;
    // set index in serial_buffer, sync byte not received, count of published packets, end of data flag

    // open serial connection (fn from serial.c)
    log_trace("Start opening serial device %s", cfg.device);
    if (!serial_open_port(cfg.device))
    {
        log_fatal("Fatal error opening serial port %s, error: %d, %s", cfg.device, errno, strerror(errno) );
        printf("Errno(%d) opening serial port %s: %s\n", errno, cfg.device, strerror(errno));
        return 2;
    }
  
    log_trace("Start setting baud rate to: %d", 19200);
    if (!serial_set_baud_rate(19200))
    {
        log_fatal("Failed to set baud rate: %s", serial_get_error());
        printf("Failed to set baud rate: %s\n", serial_get_error());
        return 3;
    }
    
    log_trace("Baud rate for %s ist set to %d", cfg.device, 19200);
        // if the filename for sqlite db ist given, open it.
    if (cfg.database != NULL)
    {
      log_info("Opening database %s", cfg.database);
      if (!sqlite_open(cfg.database))
      {
        log_fatal("sqlite database %s could not be opend, stopping", cfg.database);
        return 6;
      }

      if (firstLoop) // at first run of the main loop check for db, etc.
      {
        log_trace("First start of software, creating table on %s", cfg.database);
        sqlite_create_table();            
      }
        cfg.withSql = true;
    }    
    
    if (cfg.mqtt_enabled)
    {
      log_info("Connecting to mqtt server %s",cfg.mqtt_server);
      reconnect_mqtt(&cfg);
    }
    log_info("collecting data from %s", cfg.device);
    // write request to eta sh20
    written = serial_write(StdDataRequest, LenStdReqest);
    
// start main loop
    do
    {    
// exit when got Ctrl C
        if (caughtSigQuit(enableVerbose))
        {
            log_info("Ctrl-C was pressed.","");
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
            log_info("received start byte from %s", cfg.device);
            // print Byte when verbose is on, line wrap after 16 bytes
            if (cfg.loglevel == LOG_INFO)
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

        log_info("SH20 frame received from %",cfg.device);

#if __SQLITE__
          if (cfg.withSql)
          {
            log_info("Writing values to sqlite database %s", cfg.database);
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
              log_info("Publishing to mqtt broker");
              // publish_mqtt("tkol", packet.DSECtrlPkt.TempSensor01 * 0.1, "%.1f");
              log_error("Publishing to mqtt broker nicht implementiert.");
          } 
          
          

    } while (loopforever == true || packet_displayed == 0); //END of main do .. while loop

    serial_close_port();

    #if __SQLITE__
    log_trace("Closing sqlite3 db %s",cfg.database);
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

    return 0;
}


void enableVerbose()
{
    maincfg->loglevel = 2;
    log_set_level(LOG_INFO);
}
