//****************************************************************************
// config.cpp
//
// Wolfgang Kutscherauer 2025
// License GPL V2
// class + functions to read values from JSON file
// Implemantation
//****************************************************************************

// system includes
#include <iostream>
#include <string>
#include <fstream>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include "spdlog/spdlog.h"

//project includes
#include "config.hpp"


// Get_..... Methods returning the values
Config::Config()
 :cfgfile{"etash20-collector.json"} {}
 
Config::Config(std::string s)
 :cfgfile{s} {}


  bool Config::Get_testmode()
  {
    return testmode;
  }
  
  std::string Config::Get_device()
  {
    return device;
  }
  
  
  bool Config::Get_loopforever()
  {
    return loopforever;
  }
  
  
   int Config::Get_delay()
  {
    return delay;
  }
  
   
   int Config::Get_loglevel()
  {
    return loglevel;
  }
  
   
  bool Config::Get_withSql()
  {
    return withSql;
  }
  
  
  std::string Config::Get_database()
  {
    return database;
  }
  
    
  bool Config::Get_print_stdout()
  {
    return print_stdout;
  }
  
  
  bool Config::Get_mqtt_enabled()
  {
    return mqtt_enabled;
  }
  
  
  std::string Config::Get_mqtt_sensor_base()
  {
    return mqtt_sensor_base;
  }
  
  
  std::string Config::Get_mqtt_actor_base()
  {
    return mqtt_actor_base;
  }
  
  
  std::string Config::Get_mqtt_server()
  {
    return mqtt_server;
  }
  
  
  std::string Config::Get_mqtt_client_id()
  {
    return mqtt_client_id;
  }
  
  
  std::string Config::Config::Get_mqtt_user()
  {
    return mqtt_user;
  }
  
  
  std::string Config::Get_mqtt_password()
  {
    return mqtt_password;
  }
  
  bool Config::SetConfigFile(std::string& f)
  {
    cfgfile=f;
    SPDLOG_TRACE("SetConfigFile: {}", cfgfile);
    return true;
  }

  bool Config::ParseConfigFile()
  {
    //std::cout << "bool ParseConfigFile() not implemented yet!" << std::endl;
    // Using fstream to get the file pointer in file
    std::ifstream file(cfgfile);
    Json::Value actualJson;
    Json::Reader reader;
  // Using the reader, we are parsing the json file
    reader.parse(file, actualJson);
    testmode = actualJson["testmode"].asBool() ;
    device = actualJson["device"].asString() ;
    loopforever = actualJson["loopforever"].asBool() ;
    delay = actualJson["delay"].asInt() ;
    loglevel = actualJson["loglevel"].asInt() ;
    withSql = actualJson["withSql"].asBool() ;
    database = actualJson["database"].asString() ;
    print_stdout = actualJson["print_stdout"].asBool() ;
    mqtt_enabled = actualJson["mqtt_enabled"].asBool() ;
    mqtt_sensor_base = actualJson["mqtt_sensor_base"].asString() ;
    mqtt_actor_base = actualJson["mqtt_actor_base"].asString() ;
    mqtt_server = actualJson["mqtt_server"].asString() ;
    mqtt_client_id = actualJson["mqtt_client_id"].asString() ;
    mqtt_user = actualJson["mqtt_user"].asString() ;
    mqtt_password = actualJson["mqtt_password"].asString() ;
    return true;
  }
  


  
