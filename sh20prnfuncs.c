//******************************************************************
//
// sh20prnfuncs.c
//
// (c) Wolfgang Kutscherauer 2025
// output functions for etash20-collector.
//
//******************************************************************

#include <stdio.h>

#include "datatypes.h"
#include "sh20prnfuncs.h"
#include "log.h"



void print_sh20_data(Data_Packet *data[])
{
  log_trace("print_sh20_data started...","");
    printf("To be implemented....\n");

} // END print_sh2_data

// Print Help Message
    void print_help(void)
{
    log_trace("print_help started...", "");
    printf("Usage: etash20-collector -c, --config [configfile]...\n");
    printf("-h, --help ..........print this help message an exit\n\n");
    printf("etash20-collector get data via RS232 19200 8N2 from a ETA sha20 wood heating.\n");
    printf("The data could be stored to sqlite db, mqtt brocker, homassistant server");
    printf("The configuration ist stored in a necessary *.json file");
    printf("Supported is a ETA sh20  systems from about 2005 to 2012, with 9-Pin RS232 connection.\n");

}
//*/ 

