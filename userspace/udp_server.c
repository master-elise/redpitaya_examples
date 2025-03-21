#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <ctype.h>    // toupper
#include <strings.h>  // bzero
#include <unistd.h>   // close

#define BUFSIZE         1024

void alltoupper(char* s)
{while ( *s != 0 ) *s++ = toupper(*s);}

int main()
{ char buffer[BUFSIZE];
  struct sockaddr_in addr;
  int sd, addr_size, bytes_read;

  sd = socket(PF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9999);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sd, (struct sockaddr*)&addr, sizeof(addr));
  do {bzero(buffer, BUFSIZE);addr_size = BUFSIZE;
      bytes_read=recvfrom(sd,buffer,BUFSIZE,0, \
         (struct sockaddr*)&addr,&addr_size);
      printf("Connect: %s:%d %s\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buffer);
      alltoupper(buffer);
      sendto(sd,buffer,bytes_read,0,(struct sockaddr*)&addr, addr_size);
  } while ( bytes_read > 0 );
  close(sd);return 0;
}
