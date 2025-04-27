//******************************************************************
//
// sh20prnfuncs.h
//
// (c) Wolfgang Kutscherauer 2023
//
// output functions for etash20-collector.
//
//*******************************************************************

#pragma once

#include <stdio.h>
#include "datatypes.h"

// function to print content of received xml sting 
// in human readable form.
void print_sh2_data(Data_Packet *data[]);

// print usage info
void print_help(void);
