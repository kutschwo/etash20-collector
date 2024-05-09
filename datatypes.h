//******************************************************************************************************************
// datatypes.h
//
// (c) Hewell Technology Ltd. 2014
//
// Tobias Tangemann 2020
// Wolfgang Kutscherauer 2023
// Modifikation to deal with different types of VBus devices
// Define a structure / union for the VBus data depending on the VBus device
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
    unsigned short TempRaumMK2
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




#endif
