//****************************************************************************
// config.hpp
//
// Wolfgang Kutscherauer 2025
// Header for class config
// class + functions to read values from JSON file
//
//****************************************************************************
//#ifndef _CONFIG_HPP_
//#define _CONFIG_HPP_
#pragma once

//#include <iostream>
//#include <iterator>
#include <string>

class Config
{
  private:
    // name of config file, with path
    std::string cfgfile;
    // config values
    bool testmode = true;
    std::string device = "/dev/ttyUSB0";
           bool loopforever = true;
            int delay = 60;
            int loglevel = 0;
           bool withSql = true;
    std::string database = "etash20.db";
           bool print_stdout = true;
           bool mqtt_enabled = true;
    std::string mqtt_sensor_base = "sensor base";
    std::string mqtt_actor_base = "actor base";
    std::string mqtt_server = "tcp://server:1883";
    std::string mqtt_client_id = "softliq";
    std::string mqtt_user = "softliq";
    std::string mqtt_password = "password";
    
  public:
  //Config: Config(""){}
  Config();
  Config(std::string);
  bool Get_testmode();
  std::string Get_device();
  bool Get_loopforever();
   int Get_delay();
   int Get_loglevel();
  bool Get_withSql();
  std::string Get_database();
  bool Get_print_stdout();
  bool Get_mqtt_enabled();
  std::string Get_mqtt_sensor_base();
  std::string Get_mqtt_actor_base();
  std::string Get_mqtt_server();
  std::string Get_mqtt_client_id();
  std::string Get_mqtt_user();
  std::string Get_mqtt_password();
  bool SetConfigFile(std::string& f);
  bool ParseConfigFile();
 
};

//#endif
