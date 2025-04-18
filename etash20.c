// +++++++++++++++++++++++++++++++++++++++++++++++++++
// etash20.c




#include "etash20.h"

// As defined by ETA maximal 106 bytes are possible for query and answer.
// Per Definition sind max, nur 106 Bytes für Anfrage und Antwort möglich.
unsigned char query[110];
unsigned char eta_answer[110];

// Calculation of the checsumm of usage-data.
// sum of all usage-date, then modulo 256 of the sum.
// Berechnung der Checksumme über die Nutzdaten
// Summe aller Nutzdaten, dann Modulo 256 der Summe
unsigned char EtaChkSum(unsigned char data[], int numvals){

    return 1;

}

// Erzeugung eines kompletten Anforderungsstrings
int MakeEtaRequest(int numvals, unsigned char refreshtime, unsigned char start_idx, unsigned char node, unsigned char* request ){

return 3;
}
