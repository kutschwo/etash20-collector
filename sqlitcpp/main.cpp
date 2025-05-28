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

#include "sqldb.hpp"

int main()
{
    bool erg;
    int num;
    //int i;
    std::string  CfgFileName;
    Config eta_cfg{};
    ETAsh20 mysh20{};
    std::cout << "etash20-collector " << GIT_VERSION << std::endl;
    std::copy(argv, argv + argc, std::ostream_iterator<char *>(std::cout, "\n"));
    std::string device_string;
    std::string values_string = " ";
    char serial_device[40];
    SPDLOG_INFO("etash20-collector {}",GIT_VERSION);
    int werte[20] = {3, 7, 8, 9, 10, 11, 12, 15, 16, 17, 31, 39, 43, 68, 70, 75, 76, 197, 198, 212};
spdlog::set_level(spdlog::level::trace); // Set global log level to debug
    
    SPDLOG_TRACE("Num args: {}",argc);
    

std::cout << "\n\n *** fertig *** \n\n";


    return 0;
} // END main()


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
