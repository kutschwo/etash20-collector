//******************************************************************************************************************
// datatypes.h
//
// (c) Wolfgang Kutscherauer 2024
// created for the data of an wood heating eta sh22 
// production time about 2006 to 2012
// the series with the RS232 9 Pin Connector
// 
//*******************************************************************************************************************

#ifndef DATATYPES_H
#define DATATYPES_H


#pragma pack(1)
typedef union {
  struct {
    unsigned short TuerKontakt;
    unsigned short RpmGeblaese;
    unsigned short TempKesssel;
    unsigned short RuecklKessel;
    unsigned short PufferUnten;
    unsigned short PufferMitte;
    unsigned short PufferOben;
    unsigned short TempBoiler;
    unsigned short TempAbgas;
    unsigned short LuftOben;
    unsigned short LuftUnten;
    unsigned short MK1Modus;
    unsigned short MK1Pumpe;
    unsigned short NichtBekannt_1;
    unsigned short TempSolarBoiler;
    unsigned short TempRaumMK2;
    unsigned short VorlaufMK1;
    unsigned short VorlaufMK2;
    unsigned short TempAussen;
    unsigned short PufferProz;
    unsigned short AndersTuerOffen;
    unsigned short VorlaufMK3;
    unsigned short RaumMK4;
    unsigned short VorlaufMK4;
    unsigned short VorlaufBWT;
    unsigned short MitteBWT;
    unsigned short Brenner;
    unsigned short BoilerSolarOben;
    unsigned short TempBrauchwasser;
  } etash20Pkt;
  unsigned char asBytes[28];
} Data_Packet;


typedef struct  {
    unsigned char  id;
    const char* topic ;
} sh20entry;

#endif
