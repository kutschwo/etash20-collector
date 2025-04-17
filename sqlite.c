//****************************************************************************
// sqlite.c
//
// Tobias Tangemann 2020
// Wolfgang Kutscherauer 2024
// adptiert für eta sh20 Scheitholzkessel
// function to publish data to sqlite database
// implementation
//
//****************************************************************************
#include <sqlite3.h>
#include <stdio.h>

#include "sqlite.h"

static sqlite3 *db = NULL;

void sqlite_print_error(char* error_msg)
{
  printf("SQL error: %s\n", error_msg);
  sqlite3_free(error_msg);
}

bool sqlite_open(const char *path)
{
  if (sqlite3_open(path, &db) != 0)
  {
    printf("Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite_close();
    return false;
  }

  return true;
}


void sqlite_close()
{
  if (db != NULL)
  {
    sqlite3_close(db);
  }
}


bool sqlite_exec(char* sql)
{
  char *error_msg;

  if (sqlite3_exec(db, sql, NULL, 0, &error_msg) != SQLITE_OK) {
    sqlite_print_error(error_msg);
    return false;
  }

  return true;
}


bool sqlite_insert_data(Data_Packet* packet)
{
  char *error_msg;
  char sql_buffer[512];
  //    short dow, h, m;


   // m = data->DSECtrlPkt.SystemTime % 60;
   // dow = data->DSECtrlPkt.SystemTime / 1440;
   // h = (data->DSECtrlPkt.SystemTime - 1440*dow) / 60
//    sprintf(sql_buffer, "INSERT INTO dsectrl "
//    "(system_time, 003_0_bei_Tuer_offen, 007_RpmGeblaese, 008_TempKessel, 009 TempRueckl, 010_PufferUnten, 011_PufferMitte, 012_PufferOben, 015_TempAbgas, 016_LuftOben, 017_LuftUnten, 031_unknown, 039_TagNacht_evtl, 043_Pumpe_MK1_evtl, 068_Vorlauf_MK1, 070_TempAussen, 075_Pufferladung, 076_unknown, 197_unknown, 198_unknown, 212_unknown  ) VALUES "
//    "(datetime('now','localtime'), %.1f, %.1f, %.1f, %.1f, %d, %d, %d);",
 //   packet->DSECtrlPkt.TempSensor01 * 0.1,
    //packet->DSECtrlPkt.TempSensor02 * 0.1,
    //packet->DSECtrlPkt.TempSensor03 * 0.1,
//    packet->DSECtrlPkt.TempSensor04 * 0.1,
//    packet->DSECtrlPkt.PumpSpeed1,
//    packet->DSECtrlPkt.PumpSpeed2,
//    packet->DSECtrlPkt.PumpSpeed4);


  if (sqlite3_exec(db, sql_buffer, NULL, 0, &error_msg) != 0)
  {
    sqlite_print_error(error_msg);
    return false;
  }

  return true;

}



bool sqlite_create_table()
{
  char *error_msg;
  char sql_create_table[] = "CREATE TABLE IF NOT EXISTS data ("
    "\"id\"                   INTEGER PRIMARY KEY AUTOINCREMENT,"
    "\"time\"                 DEFAULT CURRENT_TIMESTAMP NOT NULL,"
    "\"system_time\"          TEXT NOT NULL,"
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

  if (sqlite3_exec(db, sql_create_table, NULL, 0, &error_msg) != 0)
  {
    sqlite_print_error(error_msg);
    return false;
  }

  return true;
}

