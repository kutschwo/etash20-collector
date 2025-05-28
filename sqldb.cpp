//****************************************************************************
// sqldb.cpp
//
// Wolfgang Kutscherauer 22025
// modified to use for eta sh20 wood heating
// class + function to publish data to sqlite database implementation
// added log.c lib for handling logging
//
//****************************************************************************
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

//#include <SQLiteCpp/SQLiteCpp.h>
//#include <SQLiteCpp/VariadicBind.h>
#include <sqlite3.h>

#include "sqldb.hpp"


// func to print sqlite error messages
void SqlDb::db_print_error(char* error_msg)
{
  SPDLOG_TRACE("Started: sqlite_print_error msg: {}",error_msg);
  printf("SQL error: %s\n", error_msg);
  SPDLOG_ERROR("sqlite3 SQL error: {}", error_msg);
  sqlite3_free(error_msg);
}
 
// func to oben sqlite3 database with error-handling
bool SqlDb::db_open(const char *path)
{
    SPDLOG_TRACE("Started sqlite_open..");
  if (sqlite3_open(path, &db) != 0)
  {
    printf("Can't open database: %s\n", sqlite3_errmsg(db));
    SPDLOG_ERROR("Can't open database: %s\n", sqlite3_errmsg(db));
    db_close();
    return false;
  }
  dbpath = path;
  SPDLOG_INFO("sqlite-database opend: {}",dbpath);
  return true;
}// END SqlDb::db_open

// Close the sqlite3 database connection
void SqlDb::db_close()
{
  SPDLOG_TRACE("Started sqlite_close", "");
  if (db != NULL)
  {
    sqlite3_close(db);
    SPDLOG_INFO("sqlite-database close: %s\n",dbpath);
  } // END if (db != NULL)
} // END void sqlite_close()

// execute a SQL statement and handle error message
bool SqlDb::db_exec(char* sql)
{
  SPDLOG_TRACE("Start sqlite_exec ....", "");
  char *error_msg;
  if (sqlite3_exec(db, sql, NULL, 0, &error_msg) != SQLITE_OK) {
    db_print_error(error_msg);
    SPDLOG_ERROR("sqlite3 problem %s",error_msg);
    return false;
  }  // END if (sqlite3_exec(db, sql, NULL, 0, &error_msg) != SQLITE_OK) {
  SPDLOG_TRACE("sqlite_exec successfull excuted SQL statement: %s", sql);
  return true;
} // END bool sqlite_exec(char* sql)


// insert the collected data from the eta sh20 into the sqlite database with handling of error messages
bool SqlDb::db_insert_data( std::string val_str)
{
  char *error_msg;
  std::string insert_string;
  //    short dow, h, m;
  SPDLOG_TRACE("sqlite_instert_data started...","");
  
  insert_string = "INSERT INTO data time, 003_0_bei_Tuer_offen, 007_RpmGeblaese, 008_TempKessel, 009 TempRueckl, 010_PufferUnten, 011_PufferMitte, 012_PufferOben, 015_TempAbgas, 016_LuftOben, "; 
  insert_string = insert_string + " 017_LuftUnten, 031_unknown, 039_TagNacht_evtl, 043_Pumpe_MK1_evtl, 068_Vorlauf_MK1, 070_TempAussen, 075_Pufferladung, 076_unknown, ";
  insert_string += "197_unknown, 198_unknown, 212_unknown  ) VALUES CURRENT_TIMESTAMP, "; 
  insert_string += val_str;
  insert_string += " );" ;
  // logging the SQL statement at verbose mode
  SPDLOG_TRACE("SQL-command, insert_string: {}",insert_string);

  // insert data into sqlite databas db
  if (sqlite3_exec(db, insert_string.c_str(), NULL, 0, &error_msg) != 0)
  {
    db_print_error(error_msg);
    SPDLOG_ERROR("sqlite3 problem: %s", error_msg);
    return false;
  } // END if (sqlite3_exec(db, sql_buffer, NULL, 0, &error_msg) != 0)
  SPDLOG_TRACE("data successfull insterted SQL-Statement: ");
  return true;
} // END bool sqlite_insert_data(Data_Packet* packet)


// Create the TABLE for the eta sh20 data at first start of the programm.
bool SqlDb::db_create_table()
{
  char create_table_str[] = "CREATE TABLE IF NOT EXISTS data ( " 
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
  try
  {
    db_exec(create_table_str);
  } // END try
  catch (std::exception& e)
  {
    //SPDLOG_CRITICAL("SQLITE exception: {}", e.what);
    return false;
  } //END catch
  SPDLOG_TRACE("open db {} and created Table if not present",dbpath);
  return true;
  
} // END bool sqlite_create_table()

