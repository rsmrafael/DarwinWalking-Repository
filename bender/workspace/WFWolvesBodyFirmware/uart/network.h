#ifdef DEBUG_X86

#ifdef WIN32
  #include <winsock.h>
  typedef int socklen_t;
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netdb.h>
typedef int SOCKET;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

void network_init(unsigned int address, unsigned int port);
void network_term(void);

unsigned int network_sendData(const char* data, unsigned int length);
unsigned int network_receiveData(char* data, unsigned int maxLength, struct timeval *timeout, unsigned long *addr, unsigned int *port);
void network_printIP(unsigned int ip);
unsigned int network_resolveHost(char* host);

#endif
