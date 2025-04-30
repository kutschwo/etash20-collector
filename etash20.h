//*******************************************************************************************************************
// etash20.h
//
// (c) Wolfgang Kutscherauer 2024
//
// ETA SH20 Scheitholzkessel über RS232 SMS-Schnittstelle
// Konstanten, Listen, etc.
//*******************************************************************************************************************
//#ifndef ETASH20_H

//#define ETASH20_H

#pragma once

#include <stddef.h>
#include "datatypes.h"
#include "serial.h"
#include <stdbool.h>
#include <linux/serial.h>

#define FRSTDATA 5                  // Die Nutzdaten starten ab Byte 6, Zeitinterval --> 5 weil der Index bei 0 start.
#define REF_TIME 15                  // Refreshtime, alle n Sekunden werden neue Daten vom Kessel gesendet.

#define unknow0 0
#define unknow1 1
#define unknow2 2
#define Tuerkontakt3
#define unknow4 4
#define unknow5 5
#define unknow6 6
#define Drehzahl_Geläse 7
#define KesselTemp 8
#define KesselrücklaufTemp 9
#define Pufferunten 10
#define Puffermitte 11
#define Pufferoben 12
#define BoilerTemp 13
#define unknow14 14
#define AbgasTemp 15
#define Luftoben 16
#define Luftunten 17
#define unknow18 18
#define unknow19 19
#define unknow20 20
#define unknow21 21
#define unknow22 22
#define unknow23 23
#define unknow24 24
#define unknow25 25
#define unknow26 26
#define unknow27 27
#define unknow28 28
#define unknow29 29
#define unknow30 30
#define Anheizen 31
#define unknow32 32
#define unknow33 33
#define unknow34 34
#define unknow35 35
#define unknow36 36
#define unknow37 37
#define unknow38 38
#define MK1Modus 39
#define unknow40 40
#define unknow41 41
#define unknow42 42
#define MK1Pumpe 43
#define unknow44 44
#define unknow45 45
#define unknow46 46
#define unknow47 47
#define unknow48 48
#define unknow49 49
#define unknow50 50
#define unknow51 51
#define unknow52 52
#define unknow53 53
#define unknow54 54
#define unknow55 55
#define unknow56 56
#define unknow57 57
#define unknow58 58
#define unknow59 59
#define unknow60 60
#define unknow61 61
#define KollektorTemp 62
#define BoileruntenSolar 63
#define unknow64 64
#define unknow65 65
#define unknow66 66
#define RaumMK2 67
#define VorlaufMK1 68
#define VorlaufMK2 69
#define Aussentemp 70
#define unknow71 71
#define unknow72 72
#define unknow73 73
#define unknow74 74
#define Pufferladung 75
#define AenderungTueroffen 76
#define unknow77 77
#define unknow78 78
#define unknow79 79
#define unknow80 80
#define unknow81 81
#define VorlaufMK3 82
#define unknow83 83
#define unknow84 84
#define unknow85 85
#define unknow86 86
#define unknow87 87
#define unknow88 88
#define unknow89 89
#define unknow90 90
#define unknow91 91
#define unknow92 92
#define RaumMK4 93
#define VorlaufMK4 94
#define unknow95 95
#define unknow96 96
#define unknow97 97
#define unknow98 98
#define unknow99 99
#define unknow100 100
#define unknow101 101
#define unknow102 102
#define unknow103 103
#define unknow104 104
#define unknow105 105
#define BWTRücklauf 106
#define BWTMitte 107
#define unknow108 108
#define unknow109 109
#define unknow110 110
#define unknow111 111
#define unknow112 112
#define unknow113 113
#define unknow114 114
#define unknow115 115
#define unknow116 116
#define Brenner 117
#define unknow118 118
#define unknow119 119
#define unknow120 120
#define unknow121 121
#define unknow122 122
#define unknow123 123
#define unknow124 124
#define unknow125 125
#define unknow126 126
#define unknow127 127
#define unknow128 128
#define unknow129 129
#define unknow130 130
#define unknow131 131
#define unknow132 132
#define unknow133 133
#define unknow134 134
#define unknow135 135
#define unknow136 136
#define unknow137 137
#define unknow138 138
#define unknow139 139
#define unknow140 140
#define unknow141 141
#define unknow142 142
#define unknow143 143
#define unknow144 144
#define unknow145 145
#define unknow146 146
#define unknow147 147
#define unknow148 148
#define unknow149 149
#define unknow150 150
#define unknow151 151
#define unknow152 152
#define unknow153 153
#define unknow154 154
#define unknow155 155
#define unknow156 156
#define unknow157 157
#define unknow158 158
#define unknow159 159
#define unknow160 160
#define BoilerobenSolar 161
#define unknow162 162
#define unknow163 163
#define BrauchwasserTemp 164
#define unknow165 165
#define unknow166 166
#define unknow167 167
#define unknow168 168
#define unknow169 169
#define unknow170 170
#define unknow171 171
#define unknow172 172
#define unknow173 173
#define unknow174 174
#define unknow175 175
#define unknow176 176
#define unknow177 177
#define unknow178 178
#define unknow179 179
#define unknow180 180
#define unknow181 181
#define unknow182 182
#define unknow183 183
#define unknow184 184
#define unknow185 185
#define unknow186 186
#define unknow187 187
#define unknow188 188
#define unknow189 189
#define unknow190 190
#define unknow191 191
#define unknow192 192
#define unknow193 193
#define unknow194 194
#define unknow195 195
#define unknow196 196
#define unknow197 197
#define unknow198 198
#define unknow_199 199
#define unknow_200 200
#define unknow_201 201
#define unknow_202 202
#define unknow_203 203
#define unknow_204 204
#define unknow_205 205
#define unknow_206 206
#define unknow_207 207
#define unknow_208 208
#define unknow_209 209
#define unknow_210 210
#define unknow_211 211
#define unknow_212 212
#define unknow_213 213
#define unknow_214 214
#define unknow_215 215
#define unknow_216 216
#define unknow_217 217
#define unknow_218 218
#define unknow_219 219
#define unknow_220 220
#define unknow_221 221
#define unknow_222 222
#define unknow_223 223
#define unknow_224 224
#define unknow_225 225
#define unknow_226 226
#define unknow_227 227
#define unknow_228 228
#define unknow_229 229
#define unknow_230 230
#define unknow_231 231
#define unknow_232 232
#define unknow_233 233
#define unknow_234 234
#define unknow_235 235
#define unknow_236 236
#define unknow_237 237
#define unknow_238 238
#define unknow_239 239
#define unknow_240 240
#define unknow_241 241
#define unknow_242 242
#define unknow_243 243
#define unknow_244 244
#define unknow_245 245
#define unknow_246 246
#define unknow_247 247
#define unknow_248 248
#define unknow_249 249
#define unknow_250 250
#define unknow_251 251
#define unknow_252 252
#define unknow_253 253
#define unknow_254 254
#define unknow_255 255



static const int sh20_size  = 20;
static const unsigned char AEC = 0x08; // Kennung Scheitholzkessel
static const int PosChksum =  4;       // Position der Checksumme 5. Byte --> 4 weil der Index bei 0 start.
static const int FirstData = 5;        // Nutzdaten ab Byte Nr 6
static const int EtaBaudRate = 19200;



static const unsigned char MsgStart[3]          = {'{','M','C'}; // Jede Anfrage an den Kessel bginnt mit diesen Zeichen.

// Vordefininierte Nachricht zum stoppen der Datenübertragung
static const unsigned char StopMsg[6]    = {'{','M','E',0,0,'}'};                   // Dieser Befehl stoppt die Datenübertragung des eta-Kessels
static const int LenStopMsg = 6;


// Vordefininierte Nachrichten zum wechseln der Betriebsmodi
static const unsigned char heizung_reset_msg[8] = {'{','I','H',0x02,0x01,0x01,0x00,'}'}; //  Führt einen Reset durch wie ein/aus Hauptschalter
static const unsigned char heizung_auto_msg[8] = {'{','I','H',0x02,0x02,0x02,0x00,'}'}; //  Schaltet Mischkreis auf AUTO-Betrieb
static const unsigned char heizung_tag_msg[8] = {'{','I','H',0x02,0x04,0x04,0x00,'}'}; //  Schaltet Mischkreis auf Tag-Betrieb
static const unsigned char heizung_nacht_msg[8]  = {'{','I','H',0x02,0x08,0x08,0x00,'}'}; // Schalteet Mischkreis auf NACHT-Betrieb
static const unsigned char heizung_ein_msg[8] = {'{','I','H',0x02,0x10,0x10,0x00,'}'}; //  Schaltet den Kessel ein
static const unsigned char heizung_aus_msg[8] = {'{','I','H',0x02,0x20,0x20,0x00,'}'}; //  Schaltet den Kessel aus
static const unsigned char boiler_laden_msg[8] = {'{','I','H',0x02,0x40,0x40,0x00,'}'}; //  Boiler laden
static const unsigned char heizung_00_msg[8]   = {'{','I','H',0x02,0x00,0x00,0x00,'}'}; // Nicht dokumentier zum probieren was passiert
#define LenModeMsg  8 

// Vordefininierte Anweisung an den Kessel um für Tests Daten zu übertragen.
static const unsigned char MsgSendBasicData[] = { '{','M','C', 0x19,  0x58, 0x3c, 0x08, 0x00, 0x0a, 0x08, 0x00, 0x0b, 0x08, 0x00, 0x0c, 0x08, 0x00, 0x0f, 0x08, 0x00, 0x44, 0x08, 0x00, 0x46, 0x08, 0x00, 0x03, 0x08, 0x00, 0x27 };

// request for standard set of data as defined in sh20entry mqtt struct
static const unsigned char StdDataRequest[] = { '{','M','C', 0x3d, 0x3a, 0x3c, 0x08, 0x00, 0x03, 0x08, 0x00, 0x07, 0x08, 0x00, 0x08, 0x08, 0x00, 0x09, 0x08, 0x00, 0x0a, 0x08, 0x00, 0x0b, 0x08, 0x00, 0x0c, 0x08, 0x00, 0x0f, 0x08, 0x00, 0x10, 0x08, 0x00, 0x11, 0x08, 0x00, 0x1f, 0x08, 0x00, 0x27, 0x08, 0x00, 0x2b, 0x08, 0x00, 0x44, 0x08, 0x00, 0x46, 0x08, 0x00, 0x4b, 0x08, 0x00, 0x4c, 0x08, 0x00, 0xc5, 0x08, 0x00, 0xc6, 0x08, 0x00, 0xd4, '}' };
#define LenStdReqest 67

static const sh20entry sh20mqttid[20] = {
    {  3, "003_0_bei_Tuer_offen"},            // 0
    {  7, "007_RpmGeblaese"},             // 1
    {  8, "008_TempKessel"},                // 2
    {  9, "009 TempRueckl"},       // 3
    { 10, "010_PufferUnten"},               // 4
    { 11, "011_PufferMitte"},               // 5, Ebene 2
    { 12, "012_PufferOben"},                // 6
    { 15, "015_TempAbgas"},                 // 7
    { 16, "016_LuftOben"},            // 8
    { 17, "017_LuftUten"},           // 9
    { 31, "031_unknown"},// 10, Ebene 1
    { 39, "039_TagNacht_evtl"},             // 11
    { 43, "043_Pumpe_MK1_evtl"},             // 12
    { 68, "068_Vorlauf_MK1"},                // 13
    { 70, "070_TempAussen"},                 // 14
    { 75, "075_Pufferladung"},               // 15, Ebene 2
    { 76, "076_unknown"},// 16
    {197, "197_unknown"},        // 17
    {198, "198_unknown"},        // 18
    {212, "212_unknown"}         // 19
};


// Berechnung der Checksumme über die Nutzdaten
// Summe aller Nutzdaten, dann Modulo 256 der Summe
unsigned char EtaChkSum(unsigned char data[], int numvals);

// Erzeugung eines kompletten Anforderungsstrings
int MakeEtaRequest(int numvals, unsigned char refreshtime, unsigned char start_idx, unsigned char node, unsigned char* request );

//int SendCommand(unsgined char *request, int length );


// Senden 

/*
unsigned char *bez[256] ={"0 unbekannt", 
"1 unbekannt", 
"2 unbekannt", 
"3 Tuer Kontakt", 
"4 unbekannt", 
"5 unbekannt", 
"6 unbekannt", 
"7 Drehzahl Geblaese", 
"008 Kessel Temp.", 
"009 Kesselrücklauf Temp.", 
"010 Puffer unten", 
"011 Puffer mitte", 
"012 Puffer oben", 
"13 Boiler Temp.", 
"14 unbekannt", 
"015 Abgas Temp.", 
"16 Luft oben", 
"17 Luft unten", 
"18 unbekannt", 
"19 unbekannt", 
"20 unbekannt", 
"21 unbekannt", 
"22 unbekannt", 
"23 unbekannt", 
"24 unbekannt", 
"25 unbekannt", 
"26 unbekannt", 
"27 unbekannt", 
"28 unbekannt", 
"29 unbekannt", 
"30 unbekannt", 
"31 anheizen", 
"32 unbekannt", 
"33 unbekannt", 
"34 unbekannt", 
"35 unbekannt", 
"36 unbekannt", 
"37 unbekannt", 
"38 unbekannt", 
"39 Tag/NAch/Urlaub", 
"40 unbekannt", 
"41 unbekannt", 
"42 unbekannt", 
"43 MK1 Pumpe", 
"44 unbekannt", 
"45 unbekannt", 
"46 unbekannt", 
"47 nicht bekannt", 
"48 unbekannt", 
"49 unbekannt", 
"50 unbekannt", 
"51 unbekannt", 
"52 unbekannt", 
"53 unbekannt", 
"54 unbekannt", 
"55 unbekannt", 
"56 unbekannt", 
"57 unbekannt", 
"58 unbekannt", 
"59 unbekannt", 
"60 unbekannt", 
"61 unbekannt", 
"062 Kollektor Temp.", 
"063 Boiler unten Solar", 
"64 unbekannt", 
"65 unbekannt", 
"66 unbekannt", 
"067 Raum MK2", 
"068 Vorlauf MK1", 
"069 Vorlauf MK2", 
"070 Aussentemp.", 
"71 unbekannt", 
"72 unbekannt", 
"73 unbekannt", 
"74 unbekannt", 
"75 Proz Pufferladung", 
"76 Aenderung Tuer offen", 
"77 unbekannt", 
"78 unbekannt", 
"79 unbekannt", 
"80 unbekannt", 
"81 unbekannt", 
"082 Vorlauf MK3", 
"83 unbekannt", 
"84 unbekannt", 
"85 unbekannt", 
"86 unbekannt", 
"87 unbekannt", 
"88 unbekannt", 
"89 unbekannt", 
"90 unbekannt", 
"91 unbekannt", 
"92 unbekannt", 
"093 Raum MK4", 
"094 Vorlauf MK4", 
"95 unbekannt", 
"96 unbekannt", 
"97 unbekannt", 
"98 unbekannt", 
"99 unbekannt", 
"100 unbekannt", 
"101 unbekannt", 
"102 unbekannt", 
"103 unbekannt", 
"104 unbekannt", 
"105 unbekannt", 
"106 BWT Rücklauf", 
"107 BWT Mitte", 
"108 unbekannt", 
"109 unbekannt", 
"110 unbekannt", 
"111 unbekannt", 
"112 unbekannt", 
"113 unbekannt", 
"114 unbekannt", 
"115 unbekannt", 
"116 unbekannt", 
"117 Brenner", 
"118 unbekannt", 
"119 unbekannt", 
"120 unbekannt", 
"121 unbekannt", 
"122 unbekannt", 
"123 unbekannt", 
"124 unbekannt", 
"125 unbekannt", 
"126 unbekannt", 
"127 unbekannt", 
"128 unbekannt", 
"129 unbekannt", 
"130 unbekannt", 
"131 unbekannt", 
"132 unbekannt", 
"133 unbekannt", 
"134 unbekannt", 
"135 unbekannt", 
"136 unbekannt", 
"137 unbekannt", 
"138 unbekannt", 
"139 unbekannt", 
"140 unbekannt", 
"141 unbekannt", 
"142 unbekannt", 
"143 unbekannt", 
"144 unbekannt", 
"145 unbekannt", 
"146 unbekannt", 
"147 unbekannt", 
"148 unbekannt", 
"149 unbekannt", 
"150 unbekannt", 
"151 unbekannt", 
"152 unbekannt", 
"153 unbekannt", 
"154  unbekannt", 
"155 unbekannt", 
"156 unbekannt", 
"157 unbekannt", 
"158 unbekannt", 
"159 unbekannt", 
"160 unbekannt", 
"161 Boiler oben Solar", 
"162 unbekannt", 
"163 unbekannt", 
"164 Brauchwasser Temp.", 
"165 unbekannt", 
"166 unbekannt", 
"167 unbekannt", 
"168 unbekannt", 
"169 unbekannt", 
"170 unbekannt", 
"171 unbekannt", 
"172 unbekannt", 
"173 unbekannt", 
"174 unbekannt", 
"175 unbekannt", 
"176 unbekannt", 
"177 unbekannt", 
"178 unbekannt", 
"179 unbekannt", 
"180 unbekannt", 
"181 unbekannt", 
"182 unbekannt", 
"183 unbekannt", 
"184 unbekannt", 
"185 unbekannt", 
"186 unbekannt", 
"187 unbekannt", 
"188 unbekannt", 
"189 unbekannt", 
"190 unbekannt", 
"191 unbekannt", 
"192 unbekannt", 
"193 unbekannt", 
"194 unbekannt", 
"195 unbekannt", 
"196 unbekannt", 
"197 NichtBekannt_2", 
"198 NichtBekannt_3", 
"199 unbekannt", 
"200 unbekannt", 
"201 unbekannt", 
"202 unbekannt", 
"203 unbekannt", 
"204 unbekannt", 
"205 unbekannt", 
"206 unbekannt", 
"207 unbekannt", 
"208 unbekannt", 
"209 unbekannt", 
"210 unbekannt", 
"211 unbekannt", 
"212 unbekannt", 
"213 unbekannt", 
"214 unbekannt", 
"215 unbekannt", 
"216 unbekannt", 
"217 unbekannt", 
"218 unbekannt", 
"219 unbekannt", 
"220 unbekannt", 
"221 unbekannt", 
"222 unbekannt", 
"223 unbekannt", 
"224 unbekannt", 
"225 unbekannt", 
"226 unbekannt", 
"227 unbekannt", 
"228 unbekannt", 
"229 unbekannt", 
"230 unbekannt", 
"231 unbekannt", 
"232 unbekannt", 
"233 unbekannt", 
"234 unbekannt", 
"235 unbekannt", 
"236 unbekannt", 
"237 unbekannt", 
"238 unbekannt", 
"239 unbekannt", 
"240 unbekannt", 
"241 unbekannt", 
"242 unbekannt", 
"243 unbekannt", 
"244 unbekannt", 
"245 unbekannt", 
"246 unbekannt", 
"247 unbekannt", 
"248 unbekannt", 
"249 unbekannt", 
"250 unbekannt", 
"251 unbekannt", 
"252 unbekannt", 
"253 unbekannt", 
"254 unbekannt", 
"255 unbekannt"
};
*/


