
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "WorkUnit.h"
#include "GobHeader.h"
#ifdef _MOSIX
#include <mos.h>
#endif

#define talky // if defined, all sorts of printfs come to life, telling you
              //  every boring detail of what's going on

#define CONNECT_RETRIES 5
#define CONNECT_RETRYDELAY 500 // time in ms before trying again
// attempts to open a SOCK_STREAM socket to masteraddr
// waits and retries according to constants:
//  CONNECT_RETRIES
//  CONNECT_RETRYDELAY
// returns negative on failure
int PatchMe(sockaddr_in * masteraddr);

// you heard the man
// also attempts to connect to master and report the demise of this whatever
// ... if it's given an addr, otherwise, it dies quietly... alone... and afraid
void GoHomeAndDie(int exitcode, sockaddr_in * masteraddr);

////////////////////////////////////////////////////////////////

int main(int argc, char ** argv)
{
#ifdef _MOSIX
  msx_lock();
#endif
  if(argc != 3)
  {
    printf("usage: %s [master hostname or ip] [master port]\n", argv[0]);
    exit(1);
  }
  sockaddr_in masteraddr;
  hostent * serverhost = gethostbyname(argv[1]);
  if(serverhost == NULL)
  {
    printf("failed to resolve hostname %s\n", argv[1]);
    exit(1);
  }
  
  // TODO: Why new?
  
  char * serveripbuf = new char[16];
  
  strncpy(serveripbuf, inet_ntoa(*((in_addr*)serverhost->h_addr_list[0])), 16);
  serveripbuf[15] = '\0'; // paranoia?
  if(inet_aton(serveripbuf, &masteraddr.sin_addr) == 0)
  {
    printf("error: invalid ip\n");
    exit(1);
  }
  delete[] serveripbuf;
  serveripbuf = NULL;
  
  masteraddr.sin_port = htons(atoi(argv[2]));
  masteraddr.sin_family = AF_INET;
  printf("using master at %s:%d\n",
      inet_ntoa(masteraddr.sin_addr),
      ntohs(masteraddr.sin_port));
  
  WorkUnit work; // this is important. don't lose this.     D-:|
  int sock = -1;
  GobHeader gobin, gobout;
  gobout.sender = getpid();
  
  while(true) // that means forever
  {
#ifdef talky
    printf("connecting to master..."); fflush(stdout);
#endif
#ifdef _MOSIX
    msx_lock();
#endif
    if((sock = PatchMe(&masteraddr)) < 0)
    {
#ifdef talky
      printf("failed to contact master, dying\n");
#endif
      GoHomeAndDie(1, NULL);
    }
#ifdef talky
    printf("connected\n"); fflush(stdout);
#endif
    if(work.done())
    {
#ifdef talky
      printf("sending results...");
#endif
      gobout.contents = GOB_RESULT;
      send(sock, &gobout, sizeof(gobout), 0);
      recv(sock, &gobin, sizeof(gobin), MSG_WAITALL);
      if(gobin.contents == GOB_OK)
        work.exportToSocket(sock);
      else
      {
#ifdef talky
          printf("something went wrong, dying\n");
#endif
          close(sock);
          GoHomeAndDie(1, &masteraddr);
      }
    }
#ifdef talky
    printf("requesting work...");
#endif
    gobout.contents = GOB_WORKREQUEST;
    send(sock, &gobout, sizeof(gobout), 0);
    recv(sock, &gobin, sizeof(gobin), MSG_WAITALL);
    switch(gobin.contents)
    {
      case GOB_WORKUNIT:
        work.importFromSocket(sock);
        break;
      case GOB_NOWORK:
#ifdef talky
        printf("no work!\n");
#endif
        close(sock);
        GoHomeAndDie(0, &masteraddr);
        break;
      default:
#ifdef talky
        printf("some kind of problem, dying\n");
#endif
        close(sock);
        GoHomeAndDie(1, &masteraddr);
    }
    close(sock);
#ifdef _MOSIX
    msx_unlock();
#endif
#ifdef talky
    printf("ok, disconnected\n");
    printf("executing workunit %d\n...\n", (unsigned int)work.getid());
#endif
    work.Execute();
    if(!work.done())
    {
      printf("workunit execution failed, exiting\n");
      GoHomeAndDie(1, NULL);
    }
  }
  printf("donuts\n"); // this statement truly is on the edge of forever
  return 0;
}

////////////////////////////////////////////////////////////////

int PatchMe(sockaddr_in * masteraddr)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0)
    return sock;
  int retriesleft = CONNECT_RETRIES;
  while(retriesleft > 0)
  {
    if(connect(sock, (sockaddr *)masteraddr, sizeof(sockaddr)) == 0)
      return sock;
    retriesleft--;
    if(retriesleft > 0)
      usleep(CONNECT_RETRYDELAY * 1000);
  }
  close(sock);
  return -1;
}

////////////////////////////////////////////////////////////////

void GoHomeAndDie(int exitcode, sockaddr_in * masteraddr)
{
  // on second thought, there's no point to this
  /*if(masteraddr != NULL)
  {
    int sock = PatchMe(masteraddr);
    if(sock >= 0)
    {
      GobHeader gobhead;
      gobhead.sender = getpid();
      gobhead.contents = GOB_DYING;
      send(sock, &gobhead, sizeof(gobhead), MSG_NOSIGNAL);
      close(sock);
    }
  }*/
  (void)masteraddr;
#ifdef _MOSIX
  msx_unlock(); // is this needed?
  msx_go_back_home();
#endif
#ifdef talky
  printf("EXITING, CODE %d\n", exitcode);
#endif
  exit(exitcode);
}

