
/**************************************************

file: demo_rx.c
purpose: simple demo that receives characters from
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc rstest.c rs232.c -Wall -Wextra -o2 -o rstest

**************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "rs232.h"



int main()
{
  int i, n, result, 
      cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
      bdrate=19200;       /* 9600 baud */
  char mode[]={'8','N','1',0};
  unsigned char buf[256];
  float oben, aussen;
  unsigned char req10[] = { '{', 'M', 'C', 07, 109, 0x0a, 0x08, 0x00, 0x0d, 0x08, 0x00, 70, '}' };
  unsigned char req02[] = { '{', 'M', 'C', 07, 100, 0x02, 0x08, 0x00, 0x0c, 0x08, 0x00, 70, '}', 0 };
#define RLEN = 13;

  // request for standard set of data as defined in sh20entry mqtt struct
static unsigned char StdDataRequest[] = { '{','M','C', 0x3d, 0x3a, 0x3c, 0x08, 0x00, 0x03, 0x08, 0x00, 0x07, 0x08, 0x00, 0x08, 0x08, 0x00, 0x09, 0x08, 0x00, 0x0a, 0x08, 0x00, 0x0b, 0x08, 0x00, 0x0c, 0x08, 0x00, 0x0f, 0x08, 0x00, 0x10, 0x08, 0x00, 0x11, 0x08, 0x00, 0x1f, 0x08, 0x00, 0x27, 0x08, 0x00, 0x2b, 0x08, 0x00, 0x44, 0x08, 0x00, 0x46, 0x08, 0x00, 0x4b, 0x08, 0x00, 0x4c, 0x08, 0x00, 0xc5, 0x08, 0x00, 0xc6, 0x08, 0x00, 0xd4, '}' };
#define LenStdReqest 67


  if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
  {
    printf("Can not open comport\n");

    return(0);
  }

  if( RS232_SendBuf(cport_nr, StdDataRequest, LenStdReqest))
 // {
 //   printf("Can not write to comport\n");
    
   // return(1);
 // }
  printf("\n");
  while(1)
  {
    n = RS232_PollComport(cport_nr, buf, 4095);

    if(n > 0)
    {
     // buf[n] = 0;   /* always put a "null" at the end of a string! */
      
  oben = (buf[8]*256 + buf[9])/10;
  aussen= (buf[13]*256 + buf[14])/10;
      for(i=0; i < n; i++)
      {
        printf(" %02X",buf[i]);
        if(buf[i] < 32)  /* replace unreadable control-codes by dots */
        {
          buf[i] = '.';
        }
      }

      printf("\nreceived %i bytes: %s\n", n, (char *)buf);
      printf("oben: %5.1f, aussen: %5.1f\n", oben, aussen);
    }
    if(buf[0]== '{' && buf[n] =='}')
    {
      printf("\nVollständig Daten --> ENDE\n");
    }

    usleep(20000);  /* sleep for 100 milliSeconds */
    printf("\nNochmal von vorne!\n");
  }

  return(0);
}

