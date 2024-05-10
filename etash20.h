//*******************************************************************************************************************
// etash20.h
//
// (c) Wolfgang Kutscherauer 2024
//
// ETA SH20 Scheitholzkessel über RS232 SMS-Schnittstelle
// Konstanten, Listen, etc.
//*******************************************************************************************************************
#ifndef ETA_SH_20_H

#define ETA_SH_20_H

#define FRSTDATA 5                  // Die Nutzdaten starten ab Byte 6, Zeitinterval --> 5 weil der Index bei 0 start.
#define REF_TIME 15                  // Refreshtime, alle n Sekunden werden neue Daten vom Kessel gesendet.


struct sh20entry {
    unsigned char  id;
    const char* topic ;
}

static const int sh20_size  = 20;
static const unsigned char AEC = 0x08; // Kennung Scheitholzkessel
static const int PosChksum =  4;       // Position der Checksumme 5. Byte --> 4 weil der Index bei 0 start.
static const int FirstData = 5;        // Nutzdaten ab Byte Nr 6
static const int EtaBaudRate = 19200;


static const unsigned char MsgStart[3]          = {'{','M','C'}; // Jede Anfrage an den Kessel bginnt mit diesen Zeichen.
unsigned char query[110];                        // per Definition werde nie mehr als 106 Byte benötigt.
unsigned char answer[110];

static const unsigned char StopMsg[6]    = {'{','M','E',0,0,'}'};                   // Dieser Befehl stoppt die Datenübertragung des eta-Kessels
static const int LenStopMsg = 6;
static const unsigned char heizung_reset_msg[8] = {'{','I','H',0x02,0x01,0x01,0x00,'}'}; //  Führt einen Reset durch wie ein/aus Hauptschalter
static const unsigned char heizung_auto_msg[8] = {'{','I','H',0x02,0x02,0x02,0x00,'}'}; //  Schaltet Mischkreis auf AUTO-Betrieb
static const unsigned char heizung_tag_msg[8] = {'{','I','H',0x02,0x04,0x04,0x00,'}'}; //  Schaltet Mischkreis auf Tag-Betrieb
static const unsigned char heizung_nacht_msg[8]  = {'{','I','H',0x02,0x08,0x08,0x00,'}'}; // Schalteet Mischkreis auf NACHT-Betrieb
static const unsigned char heizung_ein_msg[8] = {'{','I','H',0x02,0x10,0x10,0x00,'}'}; //  Schaltet den Kessel ein
static const unsigned char heizung_aus_msg[8] = {'{','I','H',0x02,0x20,0x20,0x00,'}'}; //  Schaltet den Kessel aus
static const unsigned char boiler_laden_msg[8] = {'{','I','H',0x02,0x40,0x40,0x00,'}'}; //  Boiler laden
static const unsigned char heizung_00_msg[8]   = {'{','I','H',0x02,0x00,0x00,0x00,'}'}; // Nicht dokumentier zum probieren was passiert
static const int LenModeMsg  = 8 ;

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


// Berechnung der Checksumme über die Nutzdaten
// Summe aller Nutzdaten, dann Modulo 256 der Summe
unsigned char EtaChkSum(unsigned char data[], int numvals);

// Erzeugung eines kompletten Anforderungsstrings
int MakeEtaRequest(int numvals, unsigned char refreshtime, unsigned char start_idx, unsigned char node, unsigned char* request );

#endif
