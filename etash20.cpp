//****************************************************************************
// etash20.cpp
//
// Wolfgang Kutscherauer 2025
// Implementation for class etash20
// class + functions to read, connect to ETA SH20 oven 
// parse the message and write values in an array.
//
//****************************************************************************
// assure that include will be read only once

#include <iostream>
//#include <iterator>
#include <string>
#include <sstream>
#include <iomanip>

// includes for libserial for c++
#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>

#include "spdlog/spdlog.h"

//Project includes
#include "datatypes.h"
#include "etash20.hpp"

using namespace LibSerial ;


int ReadVal(int);
SerialPort serial_port ;
unsigned char oven_msg[256];


// request for standard set of data as defined in sh20entry mqtt struct
 // unsigned char StdDataRequest[] = { '{','M','C', 0x3d, 0x3a, 0x3c, 0x08, 0x00, 0x03, 0x08, 0x00, 0x07, 0x08, 0x00, 0x08, 0x08, 0x00, 0x09, 0x08, 0x00, 0x0a, 0x08, 0x00, 0x0b, 0x08, 0x00, 0x0c, 0x08, 0x00, 0x0f, 0x08, 0x00, 0x10, 0x08, 0x00, 0x11, 0x08, 0x00, 0x1f, 0x08, 0x00, 0x27, 0x08, 0x00, 0x2b, 0x08, 0x00, 0x44, 0x08, 0x00, 0x46, 0x08, 0x00, 0x4b, 0x08, 0x00, 0x4c, 0x08, 0x00, 0xc5, 0x08, 0x00, 0xc6, 0x08, 0x00, 0xd4, '}' };
//#define LenStdReqest 67
   // char *device[];

ETAsh20::ETAsh20() {
  InitVars();
}
  
  ETAsh20::ETAsh20(char* d)
  :device{d} {
    InitVars();
  }

  
  bool ETAsh20::SetSerialPort(char* p)
  {
    device = p;
    return true;
  }

  
  bool ETAsh20::OpenConnection()
  {
  // open serial device
    serial_port.Open(device);
    SPDLOG_TRACE("serial_port.Open(device) device = {}", device);
  // Set the baud rates.
    serial_port.SetBaudRate( BaudRate::BAUD_19200 ) ;
    SPDLOG_TRACE("serial_port.SetBaudRate Set Baudrate to 19200");
//  8Bit size
    serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8);
    SPDLOG_TRACE("serial_port.SetCharacterSize to 8Bit");
//  No Flow controll
    serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;
    SPDLOG_TRACE("serial_port.SetFlowControl to none");
  // No Parity
    serial_port.SetParity(Parity::PARITY_NONE) ;
    SPDLOG_TRACE("serial_port.SetParity to NONE");
  // 2 StopBits
    serial_port.SetStopBits(StopBits::STOP_BITS_1) ;
    SPDLOG_TRACE("serial_port.SetStopBits 1 Stop Bit");
    return true;
  }
  
  
  bool ETAsh20::WriteRequest(DataBuffer buffer)
  {
    serial_port.Write(buffer);
    SPDLOG_TRACE("Wrote buffter to serial_port");
    return true;
  }
  
  int ETAsh20::ReadMessage()
  {
    char data_byte ;
    DataBuffer read_buffer ;
    size_t ms_timeout = 50 ;
    int max, idx;
    SPDLOG_TRACE("start Reading from RS232");
    do
    {
    // Wait for data to be available at the serial port.
      while(!serial_port.IsDataAvailable()) 
      {
          usleep(1000) ;
      }  // END  while(!serial_port.IsDataAvailable()) 
        // Read one byte from the serial port and print it to the terminal.
      //std::cout << "Data available\n";
      try
      {
        // Read a single byte of data from the serial port.
        serial_port.ReadByte(data_byte, ms_timeout) ;
          //serial_port.Read(read_buffer, 100, ms_timeout) ;
        // Show the user what is being read from the serial port.
        //std::cout << std::hex << (int)data_byte;
        //std::cout << ' ' << std::flush ;
        oven_msg[idx] = data_byte;
        SPDLOG_TRACE("Read byte {}: {}",idx, oven_msg[idx]);
        idx++;
        //oven_msg[idx] = 0x00;
        max=idx;
        if (data_byte == '}')
        {
          SPDLOG_TRACE("found } --> break");
          break;
        }
      }
      catch (const ReadTimeout&)
      {
        SPDLOG_TRACE("The ReadByte call has timed out.");

      } // END try ...catch (const ReadTimeout&)
    }// End of do ....
    while(data_byte != 0xdf);

    return max;
  } //END of bool ReadMessage()
  
  int ETAsh20::ParseMessage()
  {
    int idx=0;
    //int data_bytes;
    int num_data_bytes;
    int max_frames;
    int frame_idx;
    int val;
    unsigned char hbyte, lbyte, dataidx;
    
    num_data_bytes=oven_msg[3];
    max_frames = (num_data_bytes)/5;
    
    SPDLOG_TRACE("Start ParseMessage");
    
    for(frame_idx=1; frame_idx<=max_frames; frame_idx++)
    {
      idx = frame_idx*5;
      hbyte = int(oven_msg[idx+3]);
      lbyte = int(oven_msg[idx+4]);
      val = (hbyte*256 + lbyte);
      dataidx = int(oven_msg[idx+2]);
      sh20data[dataidx].value = val;
      //std::cout << "sh20data: dataidx: " << int(dataidx) << ", .value: " << sh20data[dataidx].value << std::endl;
      
    } // END for(frame_idx=1, frame_idx<=max_frames, frame_idx++)
    return max_frames;
  } // END int ParseMessage()
  
  bool ETAsh20::toFloat(int)
  {
    return true;
  }
  
  
  bool ETAsh20::toString(int)
  {
    return true;
  }
  
  int ETAsh20::GetInt(int index)
  {
    
    return sh20data[index].value;
  }
 
  std::string ETAsh20::GetTopic(int index)
  {
    return sh20data[index].topic;
  }
 
 char ETAsh20::GetType(int index)
 {
   return sh20data[index].id;
 }
 
  void ETAsh20::InitVars()
  {
    int i;
    std::stringstream strstr;
    std::string tempstr;
    //ss << std::setw(10) << std::setfill('0') << i;
    for (i=0;i<256;i++)
    {
      strstr << std::setw(3) << std::setfill('0') << i <<"_unknown\0";
      tempstr = strstr.str();
      sh20data[i].topic = tempstr.c_str();
      sh20data[i].id = 'i';
    }
    sh20data[3].topic   = "003_0_bei_Tuer_offen";
    sh20data[7].topic   = "007_RpmGeblaese";
    sh20data[8].topic   = "008_TempKessel";         // 2
    sh20data[8].id = 'f';
    sh20data[9].topic   = "009 TempRueckl";       // 3
    sh20data[9].id = 'f';
    sh20data[10].topic  = "010_PufferUnten";               // 4
    sh20data[10].id = 'f';
    sh20data[11].topic  = "011_PufferMitte";               // 5, Ebene 2
    sh20data[11].id = 'f';
    sh20data[12].topic  = "012_PufferOben";                // 6
    sh20data[12].id = 'f';
    sh20data[15].topic  = "015_TempAbgas";                 // 7
    sh20data[15].id = 'f';
    sh20data[16].topic  = "016_LuftOben";            // 8
    sh20data[16].id = 'f';
    sh20data[17].topic  = "017_LuftUten";           // 9
    sh20data[17].id = 'f';
    sh20data[31].topic  = "031_unknown";// 10, Ebene 1
    sh20data[39].topic  = "039_TagNacht_evtl";             // 11
    sh20data[43].topic  = "043_Pumpe_MK1_evtl";             // 12
    sh20data[43].id = 'f';
    sh20data[68].topic  = "068_Vorlauf_MK1";                // 13
    sh20data[68].id = 'f';
    sh20data[70].topic  = "070_TempAussen";                 // 14
    sh20data[70].id = 'f';
    sh20data[75].topic  = "075_Pufferladung";               // 15, Ebene 2
    sh20data[75].id = 'f';
    sh20data[76].topic  = "076_unknown";// 16
    sh20data[197].topic = "197_unknown";        // 17
    sh20data[198].topic = "198_unknown";        // 18
    sh20data[212].topic = "212_unknown";         // 19
    
    //std::cout << "\nETAsh20::InitVars\n";
    //for(i=0; i<256; i++)
    //{
    //  std::cout << "sh20data: i: " << i << ", id: " << sh20data[i].id << ", topic: " << sh20data[212].topic << std::endl;
    //}
  }
