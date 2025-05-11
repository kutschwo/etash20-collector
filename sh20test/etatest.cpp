#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
 
#include <iostream>
#include <unistd.h>

// g++ -o etatest etatest.cpp -lserial


constexpr const char* const SERIAL_PORT_1 = "/dev/ttyUSB0" ;

int main()
{
  using namespace std;
  using namespace LibSerial ;
  unsigned char request10[] = { '{', 'M', 'C', 07, 109, 0x0a, 0x08, 0x00, 0x0d, 0x08, 0x00, 70, '}' };
  unsigned char request2[] = { '{', 'M', 'C', 07, 101, 0x02, 0x08, 0x00, 0x0d, 0x08, 0x00, 70, '}'};
  // Specify a timeout value (in milliseconds).
  size_t ms_timeout = 50 ;
  // Char variable to store data coming from the serial port.
  char data_byte ;
  char endbyte ='}';
  
  DataBuffer read_buffer ;
  unsigned char puffer[256];
  int idx = 0;
  int max;
  int num_data_bytes;
  int data_start = 5;
  int data_end ;
  unsigned char hbyte, lbyte;
  float wert;
  
  DataBuffer req10 = { '{', 'M', 'C', 07, 109, 0x0a, 0x08, 0x00, 0x0d, 0x08, 0x00, 70, '}' };
  DataBuffer req02 = { '{', 'M', 'C', 07, 101, 0x02, 0x08, 0x00, 0x0d, 0x08, 0x00, 70, '}'};
  //DataBuffer buf02 = { '{', 'M', 'C', 07, 101, 0x02, 0x08, 0x00, 0x0d, 0x08, 0x00, 70, '}'};
// Create serial_port object
  SerialPort serial_port ;
  //  open serial_port
  serial_port.Open(SERIAL_PORT_1);
  // Set the baud rates.
  serial_port.SetBaudRate( BaudRate::BAUD_19200 ) ;
//  8Bit size
  serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8);
//  No Flow controll
  serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;
  // No Parity
  serial_port.SetParity(Parity::PARITY_NONE) ;
  // 2 StopBits
  serial_port.SetStopBits(StopBits::STOP_BITS_1) ;
  //Write Bytes to Port
  serial_port.Write(req02);
 // std::cout << std::hex << (int)endbyte;
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
          std::cout << std::hex << (int)data_byte;
          std::cout << ' ' << std::flush ;
          puffer[idx] = data_byte;
          idx++;
          puffer[idx] = 0x00;
          max=idx;
      }
      catch (const ReadTimeout&)
      {
        std::cerr << "\nThe ReadByte() call has timed out." << std::endl ;

      } // END try ...catch (const ReadTimeout&)
    }
    while(data_byte != 0xdf);

std::cout << "\nDaten gelesen.\n";

num_data_bytes = puffer[3];
idx=5;
do
{
  hbyte = int(puffer[idx+3]);
  lbyte = int(puffer[idx+4]);
  std::cout << "idx: " << int(idx) << ", hbyte: " << hbyte << ", lbyte: " << lbyte << "\n"; 
  wert = (hbyte*256 + lbyte)/10;
  std::cout << "Wert: " << wert << "\n";
  idx=idx+5;
}
while(idx <= max );
 

    // Successful program completion.
    std::cout << "The example program successfully completed!" << std::endl ;
  return EXIT_SUCCESS ;
}
