#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BUF_LEN 256

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

int main( int argc, char ** argv ) {
  struct sigaction sa;
  int   sec = 0;
  int   i = 0;
  char  line[BUF_LEN];
  char  msg[BUF_LEN];
  int lineNumber = 2;
  pid_t pid = 0;

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

    line[strlen( line ) - 1] = '\0';
    if( sscanf( line, "%d %s", &sec, msg ) < 2 ) {
      continue;
    }

    pid = fork( );
    if( pid == 0 ) {
      alarm_func(sec, msg, lineNumber);
      exit( 0 );
    }
    lineNumber++;
  }
  kill( -( getpid( )), SIGINT );

  system("clear");
  return 0;
}


