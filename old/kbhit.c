//****************************************************************************
// kbhit.c
//
// (c) Hewell Technology Ltd. 2014
// functions to handle Ctrl-C an other keys
// 2025 Wolfgang Kutscherauer added log functions
//
//****************************************************************************


#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

#include "kbhit.h"
#include "log.h"

struct termios oldtermios;

void (*usr1Callback)();

int ttyraw(int fd)
{
    /* Set terminal mode as follows:
       Noncanonical mode - turn off ICANON.
       Turn off signal-generation (ISIG)
        including BREAK character (BRKINT).
       Turn off any possible preprocessing of input (IEXTEN).
       Turn ECHO mode off.
       Disable CR-to-NL mapping on input.
       Disable input parity detection (INPCK).
       Disable stripping of eighth bit on input (ISTRIP).
       Disable flow control (IXON).
       Use eight bit characters (CS8).
       Disable parity checking (PARENB).
       Disable any implementation-dependent output processing (OPOST).
       One byte at a time input (MIN=1, TIME=0).
    */
    struct termios newtermios;
    log_trace("Started ttyraw");
    if(tcgetattr(fd, &oldtermios) < 0)
    {
        log_trace("No Key hit, return %d", -1);
        return(-1);
    }

    newtermios = oldtermios;

    newtermios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    /* OK, why IEXTEN? If IEXTEN is on, the DISCARD character
       is recognized and is not passed to the process. This
       character causes output to be suspended until another
       DISCARD is received. The DSUSP character for job control,
       the LNEXT character that removes any special meaning of
       the following character, the REPRINT character, and some
       others are also in this category.
    */

    newtermios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /* If an input character arrives with the wrong parity, then INPCK
       is checked. If this flag is set, then IGNPAR is checked
       to see if input bytes with parity errors should be ignored.
       If it shouldn't be ignored, then PARMRK determines what
       character sequence the process will actually see.

       When we turn off IXON, the start and stop characters can be read.
    */

    newtermios.c_cflag &= ~(CSIZE | PARENB);
    /* CSIZE is a mask that determines the number of bits per byte.
       PARENB enables parity checking on input and parity generation
       on output.
    */

    newtermios.c_cflag |= CS8;
    /* Set 8 bits per character. */

    newtermios.c_oflag &= ~(OPOST);
    /* This includes things like expanding tabs to spaces. */

    newtermios.c_cc[VMIN] = 1;
    newtermios.c_cc[VTIME] = 0;

    /* You tell me why TCSAFLUSH. */
    if(tcsetattr(fd, TCSAFLUSH, &newtermios) < 0)
    {
        return(-1);
    }

    return(0);
}


int ttyreset(int fd)
{
    log_trace("Started ttyreset", "");
    if(tcsetattr(fd, TCSAFLUSH, &oldtermios) < 0)
        return(-1);

    return(0);
}

// catching a signal
// reacting to signal SIGUSR1
void sigcatch(int sig)
{
    log_trace("Started sigcatch", "");
    if (sig == SIGUSR1)
    {
        usr1Callback();
    }
    else 
    {
        ttyreset(0);
        exit(0);
    }
}

int caughtSigQuit(void (*userSignalCallback)())
{
    log_trace("Started caughtSigQuit", "");
    usr1Callback = *userSignalCallback;

    signal(SIGUSR1, sigcatch);

	return ((size_t)signal(SIGQUIT, sigcatch) < 0);
}

