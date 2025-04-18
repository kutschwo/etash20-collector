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

#include "datatypes.h"

#define FRSTDATA 5                  // Die Nutzdaten starten ab Byte 6, Zeitinterval --> 5 weil der Index bei 0 start.
#define REF_TIME 15                  // Refreshtime, alle n Sekunden werden neue Daten vom Kessel gesendet.

#define unknow_0 0
#define unknow_1 1
#define unknow_2 2
#define Tuerkontakt3
#define unknow_4 4
#define unknow_5 5
#define unknow_6 6
#define Drehzahl_Geläse 7
#define Kessel-Temp 8
#define Kesselrücklauf-Temp 9
#define Puffer-unten 10
#define Puffer-mitte 11
#define Puffer-oben 12
#define Boiler-Temp 13
#define unknow_14 14
#define Abgas-Temp 15
#define Luft-oben 16
#define Luft-unten 17
#define unknow_18 18
#define unknow_19 19
#define unknow_20 20
#define unknow_21 21
#define unknow_22 22
#define unknow_23 23
#define unknow_24 24
#define unknow_25 25
#define unknow_26 26
#define unknow_27 27
#define unknow_28 28
#define unknow_29 29
#define unknow_30 30
#define Anheizen 31
#define unknow_32 32
#define unknow_33 33
#define unknow_34 34
#define unknow_35 35
#define unknow_36 36
#define unknow_37 37
#define unknow_38 38
#define MK1-Modus 39
#define unknow_40 40
#define unknow_41 41
#define unknow_42 42
#define MK1-Pumpe 43
#define unknow_44 44
#define unknow_45 45
#define unknow_46 46
#define unknow_47 47
#define unknow_48 48
#define unknow_49 49
#define unknow_50 50
#define unknow_51 51
#define unknow_52 52
#define unknow_53 53
#define unknow_54 54
#define unknow_55 55
#define unknow_56 56
#define unknow_57 57
#define unknow_58 58
#define unknow_59 59
#define unknow_60 60
#define unknow_61 61
#define Kollektor-Temp 62
#define Boiler-unten-Solar 63
#define unknow_64 64
#define unknow_65 65
#define unknow_66 66
#define Raum-MK2 67
#define Vorlauf-MK1 68
#define Vorlauf-MK2 69
#define Aussentemp._70 70
#define unknow_71 71
#define unknow_72 72
#define unknow_73 73
#define unknow_74 74
#define Proz-Pufferladung 75
#define Aenderung-Tuer-offen 76
#define unknow_77 77
#define unknow_78 78
#define unknow_79 79
#define unknow_80 80
#define unknow_81 81
#define Vorlauf-MK3 82
#define unknow_83 83
#define unknow_84 84
#define unknow_85 85
#define unknow_86 86
#define unknow_87 87
#define unknow_88 88
#define unknow_89 89
#define unknow_90 90
#define unknow_91 91
#define unknow_92 92
#define Raum-MK4 93
#define Vorlauf-MK4 94
#define unknow_95 95
#define unknow_96 96
#define unknow_97 97
#define unknow_98 98
#define unknow_99 99
#define unknow_100 100
#define unknow_101 101
#define unknow_102 102
#define unknow_103 103
#define unknow_104 104
#define unknow_105 105
#define BWT-Rücklauf 106
#define BWT-Mitte 107
#define unknow_108 108
#define unknow_109 109
#define unknow_110 110
#define unknow_111 111
#define unknow_112 112
#define unknow_113 113
#define unknow_114 114
#define unknow_115 115
#define unknow_116 116
#define Brenner 117
#define unknow_118 118
#define unknow_119 119
#define unknow_120 120
#define unknow_121 121
#define unknow_122 122
#define unknow_123 123
#define unknow_124 124
#define unknow_125 125
#define unknow_126 126
#define unknow_127 127
#define unknow_128 128
#define unknow_129 129
#define unknow_130 130
#define unknow_131 131
#define unknow_132 132
#define unknow_133 133
#define unknow_134 134
#define unknow_135 135
#define unknow_136 136
#define unknow_137 137
#define unknow_138 138
#define unknow_139 139
#define unknow_140 140
#define unknow_141 141
#define unknow_142 142
#define unknow_143 143
#define unknow_144 144
#define unknow_145 145
#define unknow_146 146
#define unknow_147 147
#define unknow_148 148
#define unknow_149 149
#define unknow_150 150
#define unknow_151 151
#define unknow_152 152
#define unknow_153 153
#define unknow_154 154
#define unknow_155 155
#define unknow_156 156
#define unknow_157 157
#define unknow_158 158
#define unknow_159 159
#define unknow_160 160
#define Boiler-oben-Solar 161
#define unknow_162 162
#define unknow_163 163
#define Brauchwasser-Temp 164
#define unknow_165 165
#define unknow_166 166
#define unknow_167 167
#define unknow_168 168
#define unknow_169 169
#define unknow_170 170
#define unknow_171 171
#define unknow_172 172
#define unknow_173 173
#define unknow_174 174
#define unknow_175 175
#define unknow_176 176
#define unknow_177 177
#define unknow_178 178
#define unknow_179 179
#define unknow_180 180
#define unknow_181 181
#define unknow_182 182
#define unknow_183 183
#define unknow_184 184
#define unknow_185 185
#define unknow_186 186
#define unknow_187 187
#define unknow_188 188
#define unknow_189 189
#define unknow_190 190
#define unknow_191 191
#define unknow_192 192
#define unknow_193 193
#define unknow_194 194
#define unknow_195 195
#define unknow_196 196
#define unknow_197 197
#define unknow_198 198
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
static const int LenModeMsg  = 8 ;

// Vordefininierte Anweisung an den Kessel um für Tests Daten zu übertragen.
static const unsigned char MsgSendBasicData[] = { '{','M','C', 0x19,  0x58, 0x3c, 0x08, 0x00, 0x0a, 0x08, 0x00, 0x0b, 0x08, 0x00, 0x0c, 0x08, 0x00, 0x0f, 0x08, 0x00, 0x44, 0x08, 0x00, 0x46, 0x08, 0x00, 0x03, 0x08, 0x00, 0x27 };

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

// Berechnung der Checksumme über die Nutzdaten
// Summe aller Nutzdaten, dann Modulo 256 der Summe
unsigned char EtaChkSum(unsigned char data[], int numvals);

// Erzeugung eines kompletten Anforderungsstrings
int MakeEtaRequest(int numvals, unsigned char refreshtime, unsigned char start_idx, unsigned char node, unsigned char* request );

//#endif
