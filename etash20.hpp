//****************************************************************************
// etash20.hpp
//
// Wolfgang Kutscherauer 2025
// Header for class config
// class + functions to read, connect to ETA SH20 oven 
// parse the message and write values in an array.
//
//****************************************************************************
// assure that include will be read only once
#pragma once

//#include <iostream>
//#include <iterator>
#include <string>
#include <unistd.h>
#include <iostream>

// includes for libserial for c++
#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
using namespace LibSerial ;

//Project includes
#include "datatypes.h"



class ETAsh20
{
  private:
    typedef struct  {
    unsigned char  id;
    int value;
    const char* topic ;
    } sh20entry;
    sh20entry sh20data[256];
    //sh20data[3].topic = "003_0_bei_Tuer_offen";
    char* device;
// Vordefininierte Nachrichten zum wechseln der Betriebsmodi
    void InitVars();

// Create serial_port object
//  SerialPort serial_port ;
  //DataBuffer read_buffer ;

  
    int ReadVal(int);
  public:
   DataBuffer StdDataRequest = { '{','M','C', 0x3d, 0x3a, 0x3c, 0x08, 0x00, 0x03, 0x08, 0x00, 0x07, 0x08, 0x00, 0x08, 0x08, 0x00, 0x09, 0x08, 0x00, 0x0a, 0x08, 0x00, 0x0b, 0x08, 0x00, 0x0c, 0x08, 0x00, 0x0f, 0x08, 0x00, 0x10, 0x08, 0x00, 0x11, 0x08, 0x00, 0x1f, 0x08, 0x00, 0x27, 0x08, 0x00, 0x2b, 0x08, 0x00, 0x44, 0x08, 0x00, 0x46, 0x08, 0x00, 0x4b, 0x08, 0x00, 0x4c, 0x08, 0x00, 0xc5, 0x08, 0x00, 0xc6, 0x08, 0x00, 0xd4, '}' };
// #define LenStdReqest 6
    DataBuffer  heizung_reset_msg = {'{','I','H',0x02,0x01,0x01,0x00,'}'}; //  Führt einen Reset durch wie ein/aus Hauptschalter
    DataBuffer heizung_auto_msg = {'{','I','H',0x02,0x02,0x02,0x00,'}'}; //  Schaltet Mischkreis auf AUTO-Betrieb
    DataBuffer heizung_tag_msg = {'{','I','H',0x02,0x04,0x04,0x00,'}'}; //  Schaltet Mischkreis auf Tag-Betrieb
    DataBuffer heizung_nacht_msg  = {'{','I','H',0x02,0x08,0x08,0x00,'}'}; // Schalteet Mischkreis auf NACHT-Betrieb
    DataBuffer heizung_ein_msg = {'{','I','H',0x02,0x10,0x10,0x00,'}'}; //  Schaltet den Kessel ein
    DataBuffer heizung_aus_msg = {'{','I','H',0x02,0x20,0x20,0x00,'}'}; //  Schaltet den Kessel aus
    DataBuffer boiler_laden_msg = {'{','I','H',0x02,0x40,0x40,0x00,'}'}; //  Boiler laden
    DataBuffer heizung_00_msg   = {'{','I','H',0x02,0x00,0x00,0x00,'}'}; // Nicht dokumentier zum probieren was passiert
// #define LenModeMsg  8 

// Vordefininierte Anweisung an den Kessel um für Tests Daten zu übertragen.
    DataBuffer MsgSendBasicData = { '{','M','C', 0x19,  0x58, 0x3c, 0x08, 0x00, 0x0a, 0x08, 0x00, 0x0b, 0x08, 0x00, 0x0c, 0x08, 0x00, 0x0f, 0x08, 0x00, 0x44, 0x08, 0x00, 0x46, 0x08, 0x00, 0x03, 0x08, 0x00, 0x27 };

    DataBuffer  MsgTempOben = { '{','M', 'C', 4,	25, 0x05, 0x08, 0x00, 0x0c, '}' }; 

  //Config: Config(""){}
  ETAsh20();
  ETAsh20(char*);
  bool SetSerialPort(char*);
  bool OpenConnection();
  bool WriteRequest(DataBuffer);
  int ReadMessage();
  int ParseMessage();
  bool toFloat(int);
  bool toString(int);
  int GetInt(int index);
  char GetType(int index);
  std::string GetTopic(int index);
  
 
};

//#endif
