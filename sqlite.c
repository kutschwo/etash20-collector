//****************************************************************************
// sqlite.c
//
// Created by Tobias Tangemann 2020
// Wolfgang Kutscherauer 2024-2025
// adptiert für eta sh20 Scheitholzkessel
// modified to use for eta sh20 wood heating
// function to publish data to sqlite database implementation
// added log.c lib for handling logging
//
//****************************************************************************
#include <sqlite3.h>
#include <stdio.h>

#include "datatypes.h"
#include "sqlite.h"
#include "log.h"


static sqlite3 *db = NULL; // var for sqlite3 database struct
static char *dbpath = NULL; // path to datase file

// func to print sqlite error messages
void sqlite_print_error(char* error_msg)
{
  printf("SQL error: %s\n", error_msg);
  log_error("sqlite3 SQL error: %s\n", error_msg);
  sqlite3_free(error_msg);
}

// func to oben sqlite3 database with error-handling
bool sqlite_open(const char *path)
{
  if (sqlite3_open(path, &db) != 0)
  {
    printf("Can't open database: %s\n", sqlite3_errmsg(db));
    log_error("Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite_close();
    return false;
  }
  dbpath = path;
  //dbpaht = sqlite3_filename(db, "");
  log_info("sqlite-database opend: %s",dbpath);
  return true;
}

// Close the sqlite3 database connection
void sqlite_close()
{
  if (db != NULL)
  {
    sqlite3_close(db);
    log_info("sqlite-database close: %s\n",dbpath);
  } // END if (db != NULL)
} // END void sqlite_close()

// execute a SQL statement and handle error message
bool sqlite_exec(char* sql)
{
  char *error_msg;
  if (sqlite3_exec(db, sql, NULL, 0, &error_msg) != SQLITE_OK) {
    sqlite_print_error(error_msg);
    log_error("sqlite3 problem %s",error_msg);
    return false;
  }  // END if (sqlite3_exec(db, sql, NULL, 0, &error_msg) != SQLITE_OK) {
  return true;
} // END bool sqlite_exec(char* sql)


// insert the collected data from the eta sh20 into the sqlite database with handling of error messages
bool sqlite_insert_data(Data_Packet* packet)
{
  char *error_msg;
  char sql_buffer[512];
  //    short dow, h, m;
  sprintf(sql_buffer, "INSERT INTO data ");
  sprintf(sql_buffer,"(time, 003_0_bei_Tuer_offen, 007_RpmGeblaese, 008_TempKessel, 009 TempRueckl, 010_PufferUnten, 011_PufferMitte, 012_PufferOben, 015_TempAbgas, 016_LuftOben, 017_LuftUnten, 031_unknown, 039_TagNacht_evtl, 043_Pumpe_MK1_evtl, 068_Vorlauf_MK1, 070_TempAussen, 075_Pufferladung, 076_unknown, 197_unknown, 198_unknown, 212_unknown  ) VALUES " );
  sprintf(sql_buffer, "CURRENT_TIMESTAMP, "); 
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.TuerKontakt_003);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.RpmGeblaese_007);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.TempKesssel_008);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.TempRueckl_009);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.PufferUnten_010);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.PufferMitte_011);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.PufferOben_012);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.TempAbgas_015);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.LuftOben_016);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.LuftUnten_017);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.unknown_031);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.TagNacht_039);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.MK1_Pumpe_043);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.NK1_Vorlauf_068);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.TempAussen_070);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.Pufferladung_075);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.unknown_076);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.unknown_197);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.unknown_198);
  sprintf(sql_buffer, "%d, ", packet->etash20Pkt.unknown_212);
  sprintf(sql_buffer, " );" );

  // logging the SQL statement at verbose mode
  log_info("SQL-command: %s\n",sql_buffer);
  // insert data into sqlite databas db
  if (sqlite3_exec(db, sql_buffer, NULL, 0, &error_msg) != 0)
  {
    sqlite_print_error(error_msg);
    log_error("sqlite3 problem: %s", error_msg);
    return false;
  } // END if (sqlite3_exec(db, sql_buffer, NULL, 0, &error_msg) != 0)
  return true;
} // END bool sqlite_insert_data(Data_Packet* packet)


// Create the TABLE for the eta sh20 data at first start of the programm.
bool sqlite_create_table()
{
  char *error_msg;
  char sql_create_table[] = "CREATE TABLE IF NOT EXISTS data ("
    "\"id\"                   INTEGER PRIMARY KEY AUTOINCREMENT,"
    "\"time\"                 DEFAULT CURRENT_TIMESTAMP NOT NULL,"
    "\"003_0_bei_Tuer_offen\" INTEGER NOT NULL,"
    "\"007_RpmGeblaese\"      INTEGER NOT NULL,"
    "\"008_TempKessel\"       INTEGER NOT NULL,"
    "\"009 TempRueckl\"       INTEGER NOT NULL,"
    "\"010_PufferUnten\"      INTEGER NOT NULL,"
    "\"011_PufferMitte\"      INTEGER NOT NULL,"
    "\"012_PufferOben\"       INTEGER NOT NULL,"
    "\"015_TempAbgas\"        INTEGER NOT NULL,"
    "\"016_LuftOben\"         INTEGER NOT NULL,"
    "\"017_LuftUnten\"        INTEGER NOT NULL,"
    "\"031_unknown\"          INTEGER NOT NULL,"
    "\"039_TagNacht_evtl\"    INTEGER NOT NULL,"
    "\"043_Pumpe_MK1_evtl\"   INTEGER NOT NULL,"
    "\"068_Vorlauf_MK1\"      INTEGER NOT NULL,"
    "\"070_TempAussen\"       INTEGER NOT NULL,"
    "\"075_Pufferladung\"     INTEGER NOT NULL,"
    "\"076_unknown\"          INTEGER NOT NULL,"
    "\"197_unknown\"          INTEGER NOT NULL,"
    "\"198_unknown\"          INTEGER NOT NULL,"
    "\"212_unknown\"          INTEGER NOT NULL);";
  log_info("sqlite3 CREATE TABLE data: %s", sql_create_table);
  if (sqlite3_exec(db, sql_create_table, NULL, 0, &error_msg) != 0)
  {
    sqlite_print_error(error_msg);
    log_error("sqlite3 problem: %s", error_msg);
    return false;
  } // END if (sqlite3_exec(db, sql_create_table, NULL, 0, &error_msg) != 0)
  log_info("sqlite3 initial table creating: %s", " DONE!");
  return true;
} // END bool sqlite_create_table()

