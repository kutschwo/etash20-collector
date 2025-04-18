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
    unsigned short TuerKontakt_003;
    unsigned short RpmGeblaese_007;
    unsigned short TempKesssel_008;
    unsigned short TempRueckl_009;
    unsigned short PufferUnten_010;
    unsigned short PufferMitte_011;
    unsigned short PufferOben_012;
    unsigned short TempAbgas_015;
    unsigned short LuftOben_016;
    unsigned short LuftUnten_017;
    unsigned short unknown_031;
    unsigned short TagNacht_039;
    unsigned short MK1_Pumpe_043;
    unsigned short NK1_Vorlauf_068;
    unsigned short TempAussen_070;
    unsigned short Pufferladung_075;
    unsigned short unknown_076;
    unsigned short unknown_197;
    unsigned short unknown_198;
    unsigned short unknown_212;
  } etash20Pkt;
  unsigned char asBytes[40];
} Data_Packet;


typedef struct  {
    unsigned char  id;
    const char* topic ;
} sh20entry;

#endif
