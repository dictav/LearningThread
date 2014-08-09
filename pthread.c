#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define BUF_LEN 256

typedef struct {
    int    sec;
    int    line;
    char   msg[BUF_LEN];
}   alarm_t;

void alarm_func(int sec, char* msg, int lineNumber) {
  for( int i = sec; i >= 0; i -- ) {
    printf("\0337");
    printf("\033[%d;1H", lineNumber);
    printf("(%03d) %s ", i, msg );
    printf("\0338");
    fflush( stdout );
    sleep( 1 );
  }
}

void * pt_alarm_func(void *arg) {
  alarm_t * alarm = ( alarm_t * )arg;
  pthread_detach( pthread_self() );
  alarm_func(alarm->sec,alarm->msg,alarm->line);
  return 0;
}


int main( int argc, char ** argv ) {
  struct sigaction sa;
  int   sec = 0;
  int   i = 0;
  char  line[BUF_LEN];
  char  msg[BUF_LEN];
  int lineNumber = 2;
  alarm_t * alarm;
  pthread_t pt;

  sa.sa_handler = SIG_IGN;
  sigemptyset( &sa.sa_mask );
  sigaction( SIGCHLD, &sa, 0 );

  system("clear");
  while( 1 ) {
    printf("\033[1;1H");
    printf( "Alarm ( sec msg ) -->                             " );
    printf("\033[1;23H");
    fgets( line, sizeof( line ), stdin );
    if( strlen( line ) <= 1 ) {
      continue;
    }
    if( memcmp( line, "quit", 4 ) == 0 ) break;


    alarm = malloc( sizeof( alarm_t ) );
    alarm->line = lineNumber;
    line[strlen( line ) - 1] = '\0';
    if( sscanf( line, "%d %s", &alarm->sec, alarm->msg ) < 2 ) {
      continue;
    }


    pthread_create(&pt, NULL, &pt_alarm_func, alarm);
    lineNumber++;
  }

  system("clear");
  return 0;
}


