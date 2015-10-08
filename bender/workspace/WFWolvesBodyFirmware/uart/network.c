#ifdef DEBUG_X86

#include "network.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

SOCKET network_Socket;
struct sockaddr_in network_Source;
struct sockaddr_in network_Destination;

unsigned int mInstanceCounter = 0;
void initWinsocks();

void network_init(unsigned int address, unsigned int port) {
  if (mInstanceCounter == 0) {
    initWinsocks();
  }

  // Create socket
  network_Socket = socket(PF_INET, SOCK_DGRAM, 0);
  if (network_Socket == INVALID_SOCKET) {
    printf("[Network] construct: ERROR: Unable to create UDP socket\n\n");
  }

  // Set source address for datagram and bind to socket
  network_Source.sin_family = PF_INET;
  network_Source.sin_port = htons(0);
  network_Source.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(network_Socket, (struct sockaddr *)&network_Source, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
    printf("[Network] construct: ERROR: Unable to bind UDP socket\n\n\b");
  }

  // Set destination address for datagram
  network_Destination.sin_family = PF_INET;
  network_Destination.sin_port = htons(port);
  if (address != 0) {
    network_Destination.sin_addr.s_addr = htonl(address);
  } else {
    network_Destination.sin_addr.s_addr = INADDR_BROADCAST;
  }

  mInstanceCounter++;
}

void network_term(void) {
  if (network_Socket != INVALID_SOCKET) {
#ifdef WIN32
    closesocket(network_Socket);
#else
    close(network_Socket);
#endif
    network_Socket = INVALID_SOCKET;
  }

  mInstanceCounter--;
  if (mInstanceCounter == 0) {
    // On windows platform: Terminate winsocks
#ifdef WIN32
    WSACleanup();
#endif
  }
}

unsigned int network_sendData(const char* data, unsigned int length) {
  int addrLen = sizeof(struct sockaddr_in);
  int cnt;    // number of bytes sent

  // Send datagram
  cnt = sendto(network_Socket, data, length, 0, (struct sockaddr*)&network_Destination, addrLen);
  if (cnt < 0) {
    printf ("[Network] sendData: ERROR: Error in sendto\n\n\b");
    return 0;
  }
  return 1;
}

unsigned int network_receiveData(char* data, unsigned int maxLength, struct timeval *timeout, unsigned long *addr, unsigned int *port) {
  fd_set setRead;
  int notused = 63;
  int cnt;      // number of instance
  struct sockaddr_in sa;
  socklen_t sa_len = sizeof(sa);
  // UINT16 size;

  // Add application socket to list
  FD_ZERO(&setRead);
  FD_SET(network_Socket, &setRead);

  // Perform select operation to block until a port is ready
  cnt = select(notused, &setRead, NULL, NULL, timeout);
  if (cnt == SOCKET_ERROR) {
    printf("[Network] receiveData: ERROR: select returned an error!\r\n");
  }

  // Check if at least one port has valid data
  if (cnt > 0) {
    // Check if this socket is ready
    if (FD_ISSET(network_Socket, &setRead)) {
      // Read datagram
      memset(&sa, 0, sizeof(struct sockaddr_in));
      cnt = recvfrom(network_Socket, data, maxLength, 0, (struct sockaddr *)&sa, &sa_len);
      if (cnt > 0) {
        if (addr != NULL) {  // only perform assignment if a valid buffer was provided
          *addr = ntohl(sa.sin_addr.s_addr);
        }
        if (port != NULL) {  // only perform assignment if a valid buffer was provided
          *port = ntohs(sa.sin_port);
        }
        return cnt;
      }
    }
  } else {
    // No message!
    return 0;
  }
  return 0;
}

void initWinsocks() {
  // On windows platform: Init winsocks
#ifdef WIN32
  WSADATA wsaData;
  WORD    wVersionRequested;
  int     err;

  wVersionRequested = MAKEWORD(2, 0); // 2.0 and above version of WinSock
  err = WSAStartup(wVersionRequested, &wsaData);
  if (err != 0) {
    printf("[Network] initWinsock: ERROR: Could not find a usable WinSock DLL.\n\n");
  }
#endif
}

unsigned int network_resolveHost(char* host) {
    struct hostent *hp;
    struct sockaddr_in ip;

    if (mInstanceCounter == 0) {
      initWinsocks();
    }

    hp = gethostbyname(host);
    if(hp == NULL) {
    	printf("[Network] resolveHost: Could not resolve host.\n");
    	return 0;
    }
    memset(&ip, 0, sizeof(ip));
    memcpy(&ip.sin_addr, hp->h_addr, hp->h_length);

    if (mInstanceCounter == 0) {
      // On windows platform: Terminate winsocks
#ifdef WIN32
      WSACleanup();
#endif
    }

    return ntohl(ip.sin_addr.s_addr);
}

#endif
