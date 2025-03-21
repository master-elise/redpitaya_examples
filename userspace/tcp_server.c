#include <sys/socket.h>
#include <resolv.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>

#define MY_PORT         9999
#define MAXBUF          1024

int main()
{int sockfd;
 struct sockaddr_in self;
 char buffer[MAXBUF];
 // socket type (AF = IPv4, STREAM=TCP)
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 bzero(&self, sizeof(self));
 self.sin_family = AF_INET;
 self.sin_port = htons(MY_PORT);
 self.sin_addr.s_addr = INADDR_ANY;
 bind(sockfd, (struct sockaddr*)&self, sizeof(self));
 listen(sockfd, 20);                        // wait for incoming connection
 while (1)
 {struct sockaddr_in client_addr;
  int mysize,clientfd;
  unsigned int addrlen=sizeof(client_addr);
  clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
  printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
  mysize=recv(clientfd, buffer, MAXBUF, 0);
  send(clientfd, buffer, mysize, 0);
  close(clientfd);
 }
 close(sockfd);return(0);  // Clean up (should never get here)
}

