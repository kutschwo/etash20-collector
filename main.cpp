// main.c
//
// based on work of (c) Hewell Technology Ltd. 2014
//
// modified by Tobias Tangemann 2020
// ported to C++ by Wolfgang Kutscherauer 2024
// reused to deal with wood-heating ETA SH20
//
// last change 27.04.2025 by Wolfgang Kutscherauer kutschi@dingolfing.org
//
//****************************************************************************
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"
#include <iostream>
#include <iterator>
#include <string>
#include <bits/stdc++.h> 
//project includes
#include "config.hpp"
#include "etash20.hpp"
#include "sqldb.hpp"




//#include "kbhit.h"
//#include "mqtt.h"

#ifdef __SQLITE__
    //#include "sqlite.h"
#endif

#if !defined(GIT_VERSION)
    #define GIT_VERSION "unknown"
#endif

void enableVerbose();
void PrintHelp();

int main(int argc, char *argv[])
{
    bool erg;
    int num;
    //int i;
    std::string  CfgFileName;
    Config eta_cfg{};
    ETAsh20 mysh20{};
    SqlDb mydb;
    std::cout << "etash20-collector " << GIT_VERSION << std::endl;
    std::copy(argv, argv + argc, std::ostream_iterator<char *>(std::cout, "\n"));
    std::string device_string;
    std::string values_string = " ";
    char serial_device[40];
    SPDLOG_INFO("etash20-collector {}",GIT_VERSION);
    int werte[20] = {3, 7, 8, 9, 10, 11, 12, 15, 16, 17, 31, 39, 43, 68, 70, 75, 76, 197, 198, 212};
spdlog::set_level(spdlog::level::trace); // Set global log level to debug
    
    SPDLOG_TRACE("Num args: {}",argc);
    
   if (argc < 3) {
        std::cerr << "Zu wenige Argumente!\n" <<  "Usage: " << argv[0] << "-c configurationfile" << std::endl;
        return 1;
    }
// parse command line options

    for (int idx = 1; idx < argc; ++idx)
    {
//        char *option = argv[idx];
      SPDLOG_TRACE("argv[{}]: {}", idx, argv[idx]);
      if (std::string(argv[idx]) == "-c" || std::string(argv[idx]) == "--config" )
      {
        CfgFileName = argv[idx+1];
        if (argc <= idx + 1)
        {
          //printf("Missing config file\n");
          SPDLOG_CRITICAL("Missing config file");
                    return 7;
                }
                // Use next option as file name/path
                // idx++;
                // len = strlen(argv[idx]);
                // log_trace("argv[%d]: %s", idx, argv[idx]);
                // strncpy(CfgFileName,argv[idx],len);
                // log_trace("CfgFileName: %s", CfgFileName);
                // CfgFileName[len]='\0';
                // log_trace("Config File: %s",CfgFileName);
                //printf("ConfigFile: %s\n\n", CfgFileName);

        }// END if for --config
            
        if (std::string(argv[idx]) == "-h" || std::string(argv[idx]) == "--help" )
        {
          PrintHelp();
          return 0;
        } // END if for help message
    } // END for (int idx = 1; idx < argc; ++idx)

     erg = eta_cfg.SetConfigFile(CfgFileName);
 //   log_trace("Parsing JSON File: %s",CfgFileName);
    eta_cfg.ParseConfigFile();
    // eta_cfg.SetDelay(500);

     
  SPDLOG_TRACE("Get_testmode:           {}", eta_cfg.Get_testmode() );
  SPDLOG_TRACE("Get_device():           {}", eta_cfg.Get_device() );
  SPDLOG_TRACE("Get_loopforever():      {}", eta_cfg.Get_loopforever() );
  SPDLOG_TRACE("Get_delay():            {}", eta_cfg.Get_delay() );
  SPDLOG_TRACE("Get_loglevel():         {}", eta_cfg.Get_loglevel() );
  SPDLOG_TRACE("Get_withSql():          {}", eta_cfg.Get_withSql() );
  SPDLOG_TRACE("Get_database():         {}", eta_cfg.Get_database() );
  SPDLOG_TRACE("Get_print_stdout():     {}", eta_cfg.Get_print_stdout() );
  SPDLOG_TRACE("Get_mqtt_enabled():     {}", eta_cfg.Get_mqtt_enabled() );
  SPDLOG_TRACE("Get_mqtt_sensor_base(): {}", eta_cfg.Get_mqtt_sensor_base() );
  SPDLOG_TRACE("Get_mqtt_sensor_base(): {}", eta_cfg.Get_mqtt_actor_base() );
  SPDLOG_TRACE("Get_mqtt_server():      {}", eta_cfg.Get_mqtt_server() );
  SPDLOG_TRACE("Get_mqtt_client_id():   {}", eta_cfg.Get_mqtt_client_id() );
  SPDLOG_TRACE("Get_mqtt_user():        {}", eta_cfg.Get_mqtt_user() );
  SPDLOG_TRACE("Get_mqtt_password():    {}", eta_cfg.Get_mqtt_password() );
    
  // Objekt für den Ofen inieren
  
  device_string = eta_cfg.Get_device();
    
    
    
	strcpy(serial_device, device_string.c_str()); 
    SPDLOG_TRACE("mysh20.SetSerialPort(serial_device): {}", serial_device);
    mysh20.SetSerialPort(serial_device);

  erg = mysh20.OpenConnection();
  SPDLOG_TRACE("mysh20.OpenConnection(); erg = {}", erg );
  erg = mysh20.WriteRequest(mysh20.StdDataRequest);
  SPDLOG_TRACE("mysh20.WriteRequest(mysh20.StdDataRequest) erg = {}", erg );
  num = mysh20.ReadMessage();
  SPDLOG_TRACE("mysh20.ReadMessage() num = {}", num  );
  num =  mysh20.ParseMessage();
  SPDLOG_TRACE("mysh20.ParseMessage() num = {}", num);
  
  //std::cout << "umschalten auf Tag-Betrieb \n\n\n";
  //erg = mysh20.WriteRequest(mysh20.heizung_tag_msg);
//     
//     log_set_level(cfg.loglevel);
// //    logfptr = fopen(cfg.logfile, "w");
// //    result = log_add_fp(logfptr, cfg.loglevel);
  
  
  for (int i : werte) {
    if(mysh20.GetType(i) == 'f')
    {
      std::cout << std::fixed << std::setprecision(1); // Setze Ausgabeformat
      
      std::cout << "i: " << i << "Typ: " << mysh20.GetType(i) << ", topic: " << mysh20.GetTopic(i) << ",Value:  " << float(mysh20.GetInt(i))/10 << std::endl;
      values_string = values_string + std::to_string(float(mysh20.GetInt(i))/10);
      if (i < 212)
      {
        values_string = values_string + " , ";
      } // END if (i < 212)
    }else{
      std::cout << "i: " << i << "Typ: " << mysh20.GetType(i) << ", topic: " << mysh20.GetTopic(i) << ",Value:  " << mysh20.GetInt(i) << std::endl;
      values_string = values_string + std::to_string(mysh20.GetInt(i));
      if (i < 212)
      {
        values_string = values_string + " , ";
      } // END if (i < 212)
    }
    
    
  }
   std::string dbfile = eta_cfg.Get_database();
   mydb.db_open(dbfile.c_str());
   mydb.db_create_table();
std::cout << "\n\n *** fertig *** \n\n";
//start:
//     i = 0; framedata = 0; packet_displayed = 0; frameready = 0;
//     // set index in serial_buffer, sync byte not received, count of published packets, end of data flag
// 
//     // open serial connection (fn from serial.c)
//     log_trace("Start opening serial device %s", cfg.device);
//     if (!serial_open_port(cfg.device))
//     {
//         log_fatal("Fatal error opening serial port %s, error: %d, %s", cfg.device, errno, strerror(errno) );
//         printf("Errno(%d) opening serial port %s: %s\n", errno, cfg.device, strerror(errno));
//         return 2;
//     } // END if (!serial_open_port(cfg.device))
//   
//     log_trace("Start setting baud rate to: %d", 19200);
//     if (!serial_set_baud_rate(19200))
//     {
//         log_fatal("Failed to set baud rate: %s", serial_get_error());
//         printf("Failed to set baud rate: %s\n", serial_get_error());
//         return 3;
//     }
//     
//     log_trace("Baud rate for %s ist set to %d", cfg.device, 19200);
//         // if the filename for sqlite db ist given, open it.
//     if (cfg.database != NULL)
//     {
//       log_info("Opening database %s", cfg.database);
//       if (!sqlite_open(cfg.database))
//       {
//         log_fatal("sqlite database %s could not be opend, stopping", cfg.database);
//         return 6;
//       }

//       if (firstLoop) // at first run of the main loop check for db, etc.
//       {
//         log_trace("First start of software, creating table on %s", cfg.database);
//         sqlite_create_table();            
//       }
//         cfg.withSql = true;
//     }    
//     
//     if (cfg.mqtt_enabled)
//     {
//       log_info("Connecting to mqtt server %s",cfg.mqtt_server);
//       reconnect_mqtt(&cfg);
//     }
//     
    // write request to eta sh20
    //written = serial_write(StdDataRequest, LenStdReqest);
    // written = serial_write(MsgTempOben, 10);
    // log_trace("Wrote request do %s", cfg.device);
    // log_info("Starting main loop");
// start main loop
//     do
//     {    
// // exit when got Ctrl C
//         if (caughtSigQuit(enableVerbose))
//         {
//             log_info("Ctrl-C was pressed.","");
//             break;
//         }
//         do // START do .. while (frameready == 0) 
//         {
//         // try to read one byte from serial
//           int count = serial_read(&(serial_buffer[i]), 1);
//           if (count < 1)
//         // if no byte received wait 50 ms
//           {
//             // sleep 50ms
//             nanosleep((const struct timespec[]){{.tv_sec = 0, .tv_nsec = 50000000L}}, NULL);            
//             printf(".");
//             continue;
//           }
//           // if the received byte = { it is the start of a data packet
//           // write it to byte 0 of buffer and set header sync to 1
//           if (serial_buffer[i]  == '{')
//           {
//             log_trace("{: Start empfangen");
//             serial_buffer[0] = serial_buffer[i];
//             i=0;
//             framedata = 1;
//             log_info("received start byte from %s", cfg.device);
//             // print Byte when verbose is on, line wrap after 16 bytes
//             if (cfg.loglevel == LOG_INFO)
//             {
//               printf("%02x ", serial_buffer[i]);
//               if (i != 0 && i % 16 == 0)
//               {
//                 printf("\n");
//               }
//             } // END if (cfg.verbose)
//         
// 
//            }  // END if (serial_buffer[i]  == '{')
//            if (framedata == 1 && serial_buffer[i] == '}')
//            {
//              frameready = 1;
//              log_trace("}: Ende empfangen");
//            }
//            i++;  
//         } while (frameready == 0) ; // END do .. while (frameready == 0) 
// 
//         log_info("SH20 frame received from %",cfg.device);
// 
// #if __SQLITE__
//           if (cfg.withSql)
//           {
//             log_info("Writing values to sqlite database %s", cfg.database);
//             if (cfg.loglevel == 2) 
//             {
//               printf("\nWriting to database\n");
//             }
//             sqlite_insert_data(&packet);
//           }
// #endif
//           if (cfg.print_result)
//           {
//             printf("\nPrint_result nicht implementier.\n");
//           }
//           if (cfg.mqtt_enabled)
//             {
//               log_info("Publishing to mqtt broker");
//               // publish_mqtt("tkol", packet.DSECtrlPkt.TempSensor01 * 0.1, "%.1f");
//               log_error("Publishing to mqtt broker nicht implementiert.");
//           } 
//           
//           
// 
//     } while (loopforever == true || packet_displayed == 0); //END of main do .. while loop

//     serial_close_port();
// 
//     #if __SQLITE__
//     log_trace("Closing sqlite3 db %s",cfg.database);
//     sqlite_close();
//         
//     #endif

    // if (cfg.delay > 0)
    // {
    //     if (cfg.delay == 60)
    //     {
    //         time_t rawtime;
    //         struct tm * timeinfo;
    // 
    //         time (&rawtime);
    //         timeinfo = localtime (&rawtime);
    // 
    //         if (timeinfo->tm_sec < 59)
    //         {
    //             sleep(cfg.delay - timeinfo->tm_sec);
    //         }
    //         else
    //         {
    //             sleep(cfg.delay);
    //         }
    //     }
    //     else
    //     {
    //         sleep(cfg.delay);
    //     }
    // 
    //     firstLoop = false;
    //     goto start;
    // }

    return 0;
}


void enableVerbose()
{
  spdlog::set_level(spdlog::level::trace); // Set global log level to debug
  SPDLOG_TRACE("Set LOGLEVEL to TRACE!");
  
}

void PrintHelp()
{
    //log_trace("print_help started...", "");
    std::cout << "Usage: etash20-collector -c, --config [configfile]..." << std::endl;
    std::cout << "-h, --help ..........print this help message an exit"  << std::endl;
    std::cout << "etash20-collector get data via RS232 19200 8N2 from a ETA sha20 wood heating." << std::endl;
    std::cout << "The data could be stored to sqlite db, mqtt brocker, homassistant server"  << std::endl;
    std::cout << "The configuration ist stored in a necessary *.json file"  << std::endl;
    std::cout << "Supported is a ETA sh20  systems from about 2005 to 2012, with 9-Pin RS232 connection." << std::endl;

}
