#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <signal.h>

enum KEY_ACTION{
        KEY_NULL = 0,       /* NULL */
        CTRL_A = 1,         /* Ctrl-a or Ctrl-A */
        CTRL_B = 2,         /* Ctrl-b or Ctrl-B */
        CTRL_C = 3,         /* Ctrl-c or Ctrl-C */
        CTRL_D = 4,         /* Ctrl-d or Ctrl-D */
        CTRL_E = 5,         /* Ctrl-e or Ctrl-E */
        CTRL_F = 6,         /* Ctrl-f or Ctrl-F */
        CTRL_G = 7,         /* Ctrl-g or Ctrl-G */
        CTRL_H = 8,         /* Ctrl-h or Ctrl-H */
        TAB    = 9,         /* Tab which is same as Ctrl-i or Ctrl-I */
        CTRL_J = 10,        /* Ctrl-j or Ctrl-J */
        // CTRL_K = 11,     /* Ctrl-k or Ctrl-K */
        CTRL_L = 12,        /* Ctrl+l or Ctrl-L */
        ENTER  = 13,        /* Enter which is same as Ctrl-m or Ctrl-M */
        CTRL_N = 14,        /* Ctrl+n or Ctrl-N */
        CTRL_O = 15,        /* Ctrl+o or Ctrl-O */
        CTRL_P = 16,        /* Ctrl+p or Ctrl-P */
        CTRL_Q = 17,        /* Ctrl-q or Ctrl-Q */
        CTRL_R = 18,        /* Ctrl-r or Ctrl-R */
        CTRL_S = 19,        /* Ctrl-s or Ctrl-S */
        CTRL_T = 20,        /* Ctrl-t or Ctrl-T */
        CTRL_U = 21,        /* Ctrl-u or Ctrl-U */
        CTRL_V = 22,        /* Ctrl-v or Ctrl-V */
        CTRL_W = 23,        /* Ctrl-w or Ctrl-W */
        CTRL_X = 24,        /* Ctrl-x or Ctrl-X */
        CTRL_Y = 25,        /* Ctrl-y or Ctrl-Y */
        CTRL_Z = 26,        /* Ctrl-z or Ctrl-Z */

        ESC    = 27,        /* Escape */
        BACKSPACE =  127,   /* Backspace */
        /* The following are just soft codes, not really reported by the
         * terminal directly. */
        ARROW_LEFT = 1000,
        ARROW_RIGHT,
        ARROW_UP,
        ARROW_DOWN,
        DEL_KEY,
        HOME_KEY,
        END_KEY,
        PAGE_UP,
        PAGE_DOWN
};

static struct termios orig_termios; /* In order to restore at exit.*/

void disableRawMode(int fd) {
    tcsetattr(fd,TCSAFLUSH,&orig_termios);
}

/* Called at exit to avoid remaining in raw mode. */
void editorAtExit(void) {
    disableRawMode(STDIN_FILENO);
}

/* Raw mode: 1960 magic shit. */
int enableRawMode(int fd) {
    struct termios raw;

    if (!isatty(STDIN_FILENO)) goto fatal;
    atexit(editorAtExit);
    if (tcgetattr(fd,&orig_termios) == -1) goto fatal;

    raw = orig_termios;  /* modify the original mode */
    /* input modes: no break, no CR to NL, no parity check, no strip char,
     * no start/stop output control. */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /* output modes - disable post processing */
    raw.c_oflag &= ~(OPOST);
    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);
    /* local modes - choing off, canonical off, no extended functions,
     * no signal chars (^Z,^C) */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    /* control chars - set return condition: min number of bytes and timer. */
    raw.c_cc[VMIN] = 0; /* Return each byte, or zero for timeout. */
    raw.c_cc[VTIME] = 1; /* 100 ms timeout (unit is tens of second). */

    /* put terminal in raw mode after flushing */
    if (tcsetattr(fd,TCSAFLUSH,&raw) < 0) goto fatal;
    return 0;

fatal:
    errno = ENOTTY;
    return -1;
}

int editorReadKey(int fd) {
    int nread;
    char c, seq[3];
    while ((nread = read(fd,&c,1)) == 0);
    if (nread == -1) exit(1);

    while(1) {
        switch(c) {
        case ESC:    /* escape sequence */
            /* If this is just an ESC, we'll timeout here. */
            if (read(fd,seq,1) == 0) return ESC;
            if (read(fd,seq+1,1) == 0) return ESC;

            /* ESC [ sequences. */
            if (seq[0] == '[') {
                if (seq[1] >= '0' && seq[1] <= '9') {
                    /* Extended escape, read additional byte. */
                    if (read(fd,seq+2,1) == 0) return ESC;
                    if (seq[2] == '~') {
                        switch(seq[1]) {
                        case '3': return DEL_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        }
                    }
                } else {
                    switch(seq[1]) {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                    }
                }
            }

            /* ESC O sequences. */
            else if (seq[0] == 'O') {
                switch(seq[1]) {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
                }
            }
            break;
        default:
            return c;
        }
    }
}


// int main(int argc, char **argv) {
int main() {
   int c = 0;

   enableRawMode(STDIN_FILENO);
   while(1) {
        c = editorReadKey(STDIN_FILENO);
        printf("c = %d. \r\n", c);
        if(c == CTRL_Q)
        {
            break;
        }
   }
}
