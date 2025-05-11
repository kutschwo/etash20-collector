
#include <sys/types.h>
#include <sys/socket.h>                                                             
#include <stdio.h>                                                                  
#include <netinet/in.h>                                                             
#include <arpa/inet.h>                                                              
#include <string.h>                                                                 
#include <fcntl.h>                                                                  
#include <errno.h>                                                                  
#include <termios.h>                                                                
#include <sys/stat.h>                                                               
#include <stdlib.h>                                                                 
#include <stdarg.h>                                                                 
#include <signal.h>                                                                 
#include <unistd.h>                                                                 
#include "sys/ioctl.h"                                                              
#include "fcntl.h"                                                                  
#include <pthread.h>                                                                

//gcc sh20tests.c -lpthread -o sh20test  
// 

int tty_fd;
pthread_t leseTTY;
int server_socket;                                                                  
volatile int client_socket=-1;

unsigned char start[3]          = {'{','M','C'};
unsigned char datas[10][3]={{0x08,00,0x0c},{0x08,00,0x0b},{0x08,00,0x0a}
                           ,{0x08,00,0x0d},{0x08,00,70},{0x08,00,75}
                           ,{0x08,00,15},{0x08,00,8},{0x08,00,9}
                           ,{0x08,00,68}};
unsigned char *namen[10]  ={"Puffer Oben    ","Puffer Mitte   ","Puffer Unten   "
                            ,"Boiler         ","Aussen         ","Puffer Ladezst."
                            ,"Abgas          ","Kessel         ","Ruecklauf      "
                            ,"Vorlauf MK1    "};
unsigned char ende[1]     = {'}'};        
unsigned char stopp[6]    = {'{','M','E',0,0,'}'};
unsigned char boilerAn[8]    = {'{','I','H',2,0x40,0x40,0,'}'};

unsigned char heizung_reset_msg[8] = {'{','I','H',0x02,0x01,0x01,0x00,'}'}; //  Führt einen Reset durch wie ein/aus Hauptschalter
unsigned char heizung_auto_msg[8] = {'{','I','H',0x02,0x02,0x02,0x00,'}'}; //  Schaltet Mischkreis auf AUTO-Betrieb
unsigned char heizung_tag_msg[8] = {'{','I','H',0x02,0x04,0x04,0x00,'}'}; //  Schaltet Mischkreis auf Tag-Betrieb
unsigned char heizung_nacht_msg[8]  = {'{','I','H',0x02,0x08,0x08,0x00,'}'}; // Schalteet Mischkreis auf NACHT-Betrieb
unsigned char heizung_ein_msg[8] = {'{','I','H',0x02,0x10,0x10,0x00,'}'}; //  Schaltet den Kessel ein
unsigned char heizung_aus_msg[8] = {'{','I','H',0x02,0x20,0x20,0x00,'}'}; //  Schaltet den Kessel aus
unsigned char heizung_00_msg[8]   = {'{','I','H',0x02,0x00,0x00,0x00,'}'}; // Nicht dokumentier zum probieren was passiert

float werte[10];
const int anzahlWerte = 10;
volatile unsigned char ausgabe[500];
unsigned char ausgabeText[500];
volatile int sonderwaermflag=0;
volatile int resetflag=0;
volatile int autoflag=0;
volatile int tagflag=0;
volatile int nachtflag=0;
volatile int einflag=0;
volatile int ausflag=0;
volatile int msg0flag=0;


int tty_open(char* tty_dev) {
  struct termios new_attributes;
  tty_fd = open(tty_dev,O_RDWR| O_NOCTTY | O_NONBLOCK);
  bzero(&new_attributes,sizeof(new_attributes));
  new_attributes.c_cflag &= ~PARENB;
  new_attributes.c_cflag &= ~CSTOPB;
  new_attributes.c_cflag &= ~CSIZE;
  new_attributes.c_cflag |= CS8;
  new_attributes.c_cflag |= B19200;
  int zahl = tcsetattr(tty_fd, TCSANOW, &new_attributes);
  if (zahl < 0) {
    printf ("Fehler %d\n",zahl);
  }
  return tty_fd;
}

void erzeugeUndSendeOutString() {
  unsigned char tmp[123]; 
  int i,pruef=0,lang = 0;
  memcpy(tmp+lang,start,3);
  lang += 6;
  for ( i = 0 ; i < 10 ; i++) {
    memcpy(tmp+lang,datas[i],3);
    lang += 3;
  }
  tmp[5] = 20;           // Zeit
  for (i=0;i<lang;i++) {
    if (i>=5) {
      pruef += tmp[i];
    }
  }
  tmp[3] = lang-5;
  tmp[4] = pruef%256;
  tmp[lang]= ende[0];
  lang++;
  write(tty_fd,tmp,lang);
}

void setzeAusgabe(char *was) {
  strcpy(ausgabeText,was);
}


void leseLeer() {
  unsigned char tmp[30];
  unsigned wort[500];
  int lang;
  int a,i;
  int anzahlBytes = -1;
  ausgabe[0]=0;
  lang=0;
  a = read (tty_fd,tmp,1);
  while ((a != -1)) {
    if (a>0) {
      if (lang == 3) {
        anzahlBytes = tmp[0];
      }
      wort[lang]=tmp[0];
      lang++;
      if (tmp[0] == '}') {
        if (lang >= anzahlBytes && anzahlBytes > 0) {
          break;
        }
      }
    }
    a = read (tty_fd,tmp,1);
  }
}

void leseundInterpretiereInString() {
  unsigned char tmp[30];
  unsigned wort[500];
  int lang;
  int a,i;
  float wert;
  int anzahlBytes = -1;
  while (1) {
    ausgabe[0]=0;
    lang=0;
    a = read (tty_fd,tmp,1);
    while ((a != -1)) {
      if (a>0) {
        if (lang == 3) {
          anzahlBytes = tmp[0];
        }
        wort[lang]=tmp[0];
        lang++;
        if (tmp[0] == '}') {
          if (lang >= anzahlBytes && anzahlBytes > 0) {
            break;
          }
        }
      }
      a = read (tty_fd,tmp,1);
    }
    for (i=0 ; i < 10 ; i++) {
      for (a=5;a<lang;a+=5) {
        if (wort[a]==datas[i][0] && wort[a+1]==datas[i][1] && wort[a+2]==datas[i][2]) {
          wert=(float)wort[a+3]*256.0+(float)wort[a+4];
          if (wert > 65000) wert-= 65536;
         
          if (wert < -1000 || wert > 10000) {
            wert = werte[i];
            wert=0;
          }

          if (!strstr(namen[i],"Puffer Ladezst.")) {
            wert /=10;
            sprintf(tmp," %2.2f",wert);
          }
          else {
            sprintf(tmp," %2.2f%%",wert);
          }
          werte[i] = wert;
          sprintf (ausgabe,"%s%s:%s\n",ausgabe,namen[i],tmp);
        }
      }
    }
    setzeAusgabe(ausgabe);
// Boiler laden
    if (sonderwaermflag) {
      write(tty_fd,boilerAn,8);
//      leseLeer();
      sonderwaermflag=0;
    } //ende if sonderwaermflag
// RESET 
    if (resetflag) {
      write(tty_fd,heizung_reset_msg,8);
//      leseLeer();
      resetflag=0;
    } //ende if resetflag
// MK1 AUTO 
    if (autoflag) {
      write(tty_fd,heizung_auto_msg,8);
//      leseLeer();
      autoflag=0;
    } //ende if autoflag
// MK1 TAG
    if (tagflag) {
      write(tty_fd,heizung_tag_msg,8);
//      leseLeer();
      tagflag=0;
    } //ende if tagflag
// MK1 NACHT
    if (nachtflag) {
      write(tty_fd,heizung_nacht_msg,8);
//      leseLeer();
      nachtflag=0;
    } //ende if nachtflag
// MK1 EIN
    if (einflag) {
      write(tty_fd,heizung_ein_msg,8);
//      leseLeer();
      einflag=0;
    } //ende if einflag
// MK1 AUS
    if (ausflag) {
      write(tty_fd,heizung_aus_msg,8);
//      leseLeer();
      ausflag=0;
    } //ende if ausflag
// 0 = UNKNOWN Message
    if (msg0flag) {
      write(tty_fd,heizung_00_msg,8);
//      leseLeer();
      msg0flag=0;
    } //ende if msg0flag
  }
}

char *parse (char *was) {                                                           
  char *tmp;                                                                        
  char *retVal = malloc(strlen(was));                                               
  strcpy(retVal,was);                                                               
  tmp = strstr(retVal,"\n");                                                        
  if (tmp != NULL) {                                                                
    tmp[0]=0;                                                                       
  }                                                                                 
  while ((tmp = strstr(retVal,"\t"))) {                                             
    tmp[0] = (char)' ';                                                             
  }                                                                                 
  while ((tmp = strstr(retVal,"  "))) {                                             
    memmove(tmp,tmp+1,strlen(tmp));                                                 
  }                                                                                 
  return retVal;                                                                    
}   

int serverStart(int port) {                                                         
  int anzahl, laenge;                                                               
  struct sockaddr_in serverinfo, clientinfo;                                        
  char zeichen[200];                                                                
  char empfangen[1000];                                                             
  int execCode;                                                                     
                                                                                    
  if (port == -1) port = 5100;                                                      
                                                                                    
  server_socket = socket(AF_INET, SOCK_STREAM, 0);                                  
                                                                                    
  serverinfo.sin_family = AF_INET;                                                  
  serverinfo.sin_addr.s_addr = htonl(INADDR_ANY);                                   
  serverinfo.sin_port = htons(port);                                                
  laenge = sizeof(serverinfo);                                                      
                                                                                    
  execCode = bind(server_socket, (struct sockaddr *)&serverinfo, laenge);           
  if (execCode == -1) {                                                             
    printf ("Funktion bind konnte nicht ausgefuehrt werden, das Programm wird\n");  
    printf ("in 5 Sekunden beendet\n");                                             
    sleep(5);                                                                       
    return 3;                                                                       
  }                                                                                 
  execCode = listen(server_socket, 3);                                              
  if (execCode == -1) {                                                             
    printf ("Funktion listen konnte nicht ausgefuehrt werden, das Programm wird\n");
    printf ("in 5 Sekunden beendet\n");                                             
    sleep(5);                                                                       
    return 1;                                                                       
  }       
  while (1) {                                                                       
    client_socket = -1;                                                             
    printf("\n Der Server wartet...");                                              
    fflush(stdout);                                                                 
    client_socket = accept(server_socket, (struct sockaddr *)&clientinfo, &laenge); 
    printf("\n Verbindung mit %s\n",inet_ntoa(clientinfo.sin_addr));                
    sprintf(zeichen,"\nHilfe durch Eingabe von help\n\n");                          
    write(client_socket,zeichen,strlen(zeichen));                                   
    while (1) {                                                                     
      write(client_socket,"# ",2);                                                  
      anzahl = read(client_socket,empfangen,sizeof(empfangen));                     
      empfangen[anzahl]=0;                                                          
      char *tmp=parse(empfangen);                                                   
      strcpy(empfangen,tmp);                                                        
      free(tmp);                                                                    
      printf ("%s\n",empfangen);                                                    
      if (strstr(empfangen,"close")==empfangen) {                                   
        break;                                                                      
      }                                                                             
      else if (strstr(empfangen,"shutdown")==empfangen) {                           
        write(client_socket,"bye\n",4);                                             
        close(client_socket);                                                       
        close(server_socket);                                                       
        return 0;                                                                   
      }                                                                             
      else if (strstr(empfangen,"help")==empfangen) {                               
        sprintf (zeichen,"Hilfe:\n");                                               
        sprintf (zeichen,"%s  close           - Verbindung beenden\n",zeichen);      
        sprintf (zeichen,"%s  shutdown        - Server runterfahren\n",zeichen);     
        sprintf (zeichen,"%s  sonderwaerclose - Heizt den Boiler auf und terminiert\n",zeichen);
        sprintf (zeichen,"%s  sonderwaermen   - Heizt den Boiler auf\n",zeichen);
        sprintf (zeichen,"%s  statclose       - Zeigt die aktuellen Temperaturen an ",zeichen);
        sprintf (zeichen,"%sund terminiert\n",zeichen);
        sprintf (zeichen,"%s  status          - Zeigt die aktuellen Temperaturen an\n",zeichen);
        sprintf (zeichen,"%s  reset           - Reset wie ein/aus\n",zeichen);
        sprintf (zeichen,"%s  auto            - MK1 Auto\n",zeichen);
        sprintf (zeichen,"%s  tag             - MK1 Tag\n",zeichen);
        sprintf (zeichen,"%s  nacht           - MK1 Nacht\n",zeichen);
        sprintf (zeichen,"%s  ein             - MK1 ein\n",zeichen);
        sprintf (zeichen,"%s  aus             - MK1 aus\n",zeichen);
        sprintf (zeichen,"%s  000             - 0 unknown         \n",zeichen);
        
        write(client_socket,zeichen,strlen(zeichen));                               
      }                                                                             
      else if (strstr(empfangen,"status")==empfangen) {
        sprintf (zeichen,"%s",ausgabeText);
        write(client_socket,zeichen,strlen(zeichen));
      }
      else if (strstr(empfangen,"statclose")==empfangen) {
        sprintf (zeichen,"\n%s",ausgabeText);
        write(client_socket,zeichen,strlen(zeichen));
        break;
      }                                                                             
      else if (strstr(empfangen,"sonderwaermen")==empfangen) {
        sonderwaermflag=1;
      }                                                                             
      else if (strstr(empfangen,"sonderwaerclose")==empfangen) {
        sonderwaermflag=1;
        break;
      }
      else if (strstr(empfangen,"reset")==empfangen) {
        resetflag=1;
        break;
      }
      else if (strstr(empfangen,"auto")==empfangen) {
        autoflag=1;
        break;
      }
      else if (strstr(empfangen,"tag")==empfangen) {
        tagflag=1;
        break;
      }
      else if (strstr(empfangen,"nacht")==empfangen) {
        nachtflag=1;
        break;
      }
      else if (strstr(empfangen,"ein")==empfangen) {
        einflag=1;
        break;
      }
      else if (strstr(empfangen,"aus")==empfangen) {
        ausflag=1;
        break;
      }
      else if (strstr(empfangen,"000")==empfangen) {
        msg0flag=1;
        break;
      }
    }  //END while(1)
    close(client_socket);
  }
  return 0;                                                                         
}                                                                                   


int main(int argc, char *argv[]) {
  int port = 5123;  
  if (tty_open(argv[1])<0) {
    fprintf (stderr,"tty open error %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  } 
  erzeugeUndSendeOutString();
  pthread_create(&leseTTY,NULL,(void *)&leseundInterpretiereInString,NULL);
  serverStart(port);
  write(tty_fd,stopp,6);
  close(tty_fd);
  exit(0);
}
