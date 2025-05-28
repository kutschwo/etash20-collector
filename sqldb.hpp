//****************************************************************************
// sqldb.hpp
//
// 2025 Wolfgang Kutscherauer
// 
// Class + functions to publish data to sqlite database
// header file
//
//****************************************************************************
#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <sqlite3.h>

//#include <SQLiteCpp/SQLiteCpp.h>
//#include <SQLiteCpp/VariadicBind.h>

class SqlDb
{

  private:
/*
    std::string create_table_str[] = "CREATE TABLE IF NOT EXISTS data ( " 
    "\"id\"                   INTEGER PRIMARY KEY AUTOINCREMENT, " 
    "\"time\"                 DEFAULT CURRENT_TIMESTAMP NOT NULL, " 
    "\"003_0_bei_Tuer_offen\" INTEGER NOT NULL, " 
    "\"007_RpmGeblaese\"      INTEGER NOT NULL, " 
    "\"008_TempKessel\"       INTEGER NOT NULL, " 
    "\"009 TempRueckl\"       INTEGER NOT NULL, " 
    "\"010_PufferUnten\"      INTEGER NOT NULL, " 
    "\"011_PufferMitte\"      INTEGER NOT NULL, " 
    "\"012_PufferOben\"       INTEGER NOT NULL, " 
    "\"015_TempAbgas\"        INTEGER NOT NULL, " 
    "\"016_LuftOben\"         INTEGER NOT NULL, " 
    "\"017_LuftUnten\"        INTEGER NOT NULL, " 
    "\"031_unknown\"          INTEGER NOT NULL, " 
    "\"039_TagNacht_evtl\"    INTEGER NOT NULL, " 
    "\"043_Pumpe_MK1_evtl\"   INTEGER NOT NULL, " 
    "\"068_Vorlauf_MK1\"      INTEGER NOT NULL, " 
    "\"070_TempAussen\"       INTEGER NOT NULL, " 
    "\"075_Pufferladung\"     INTEGER NOT NULL, " 
    "\"076_unknown\"          INTEGER NOT NULL, " 
    "\"197_unknown\"          INTEGER NOT NULL, " 
    "\"198_unknown\"          INTEGER NOT NULL, " 
    "\"212_unknown\"          INTEGER NOT NULL);";
*/
  sqlite3 *db = NULL; // var for sqlite3 database struct
  std::string dbpath ; // path to datase file

  public:
bool db_open(const char *dbpath);
void db_close();
bool db_exec(char* sql);
bool db_create_table();
bool db_insert_data(std::string valuestr);


private:
void db_print_error(char* error_msg);

};
