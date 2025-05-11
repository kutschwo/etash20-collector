
/**************************************************

file: cstest.c
test sercial.c
compile with the command: gcc cstest.c serical.c -Wall -Wextra -o2 -o cstest

**************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "serial.h"
 


int main()
{
  int i, n;
//      cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
//      bdrate=19200;       /* 9600 baud */
//  char mode[]={'8','N','1',0};
  int headerSync = 0;
  
  unsigned char buf[4096];
//  float oben, aussen;
//  unsigned char req10[] = { '{', 'M', 'C', 07, 109, 0x0a, 0x08, 0x00, 0x0d, 0x08, 0x00, 70, '}' };
  unsigned char req02[] = { '{', 'M', 'C', 07, 100, 0x02, 0x08, 0x00, 0x0c, 0x08, 0x00, 70, '}', 0 };
  size_t rlen = 13;
//ssize_t getcount;

  float oben, aussen;
  
printf("Try to open /dev/ttyUSB0\n");
  if(!serial_open_port("/dev/ttyUSB0"))
  {
    printf("Can not open comport\n");

    return(0);
  }
  else
  {
      printf("Open ttyUSB0\n");
  }
  
  printf("try to set BaudRate 19200\n");
  
  if(!serial_set_baud_rate(19200))
  {
    printf("Can not set 19200 Baud\n");

    return(0);
  }
  else
  {
      printf("Set Baud rate\n");
  }
  
  printf("try top write to port\n");
  if( !serial_write(req02, rlen))
  {
    printf("Can not write to comport\n");
    
  return(1);
  }
    else
  {
      printf("Wrote req2\n");
  }
  
  
  printf("\n");
  i=0;
  while(1)
  {
      
      
      // try to read one byte from serial
        int count = serial_read(&(buf[i]), 1);
        if (count < 1)
// if no byte received wait 50 ms
        {
            // sleep 50ms
            nanosleep((const struct timespec[]){{.tv_sec = 0, .tv_nsec = 50000000L}}, NULL);            
            continue;
        }
// if the received byte = 0xAA it it the start of a data packet
// write it to byte 0 of buffer and set header sync to 1
        if (buf[i]  == 0x7B)
        {
            buf[0] = buf[i];
            i=0;
            headerSync = 1;
        printf("\n");


        } // END (serial_buffer[i]  == 0x7B)
        printf(" %02X",buf[i]);
    if (headerSync==1 && buf[i] == 0x7D)
    {
      printf("\n'}' erreicht, Ende Message\n");
      break;
    }
   usleep(100000);  /* sleep for 100 milliSeconds */

  } // END while(1)
  oben = (buf[8]*256 + buf[9])/10;
  aussen= (buf[13]*256 + buf[14])/10;
      for(n=0; n < i; n++)
      {
        printf(" %02X",buf[n]);
        if(buf[n] < 32)  // replace unreadable control-codes by dots 
        {
          buf[n] = '.';
        }
      } // END for(n=0; n < i; n++)
      printf("\nreceived %d bytes: %s\n", n, (char *)buf);
      printf("oben: %5.1f, aussen: %5.1f\n", oben, aussen);
    

  return(0);
}

/*
    if(n > 0)
    {
      printf("n: %d\n",n);
       // buf[n] = 0;   // always put a "null" at the end of a string! 
      
  oben = (buf[8]*256 + buf[9])/10;
  aussen= (buf[13]*256 + buf[14])/10;
      for(i=0; i < n; i++)
      {
        printf(" %02X",buf[i]);
        if(buf[i] < 32)  // replace unreadable control-codes by dots 
        {
          buf[i] = '.';
        }
      }
      printf("\nreceived %i bytes: %s\n", n, (char *)buf);
      printf("oben: %5.1f, aussen: %5.1f\n", oben, aussen);
    }


*/
