#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdint.h>
#include <string>

#ifdef WIN32
  #include <winsock2.h>
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

class Network {
public:
  Network(uint32_t address, uint16_t port, uint32_t sourceAddress, uint16_t sourcePort);//!<Constructor call of the Private Construct Method
  Network(uint32_t address, uint16_t port, uint32_t sourceAddress);//!<Constructor call of the Private Construct Method
  Network(uint32_t address, uint16_t port);//!<Constructor call of the Private Construct Method
  Network(uint32_t address, uint16_t port, bool TCPMode);//!<Constructor call of the Private Construct Method
  ~Network();//!< Destructor close the sockets

  bool sendData(const char* data, size_t length);//!< Send Data to the target of the network.
  size_t receiveData(char* data, size_t maxLength);//!< getData from the Network(Blcoking).
  size_t receiveData(char* data, size_t maxLength, struct timeval* timeout, uint32_t *addr, uint16_t *port);//!< Waits a time for data on the network.
  uint32_t getLocalIP();//!< Returns the given ip.
  uint32_t getRemoteIP();//!< Returns the given target
  uint32_t getInterfaceIP() const;//!< Check the local ip and returns it.
  void setRemoteIP(uint32_t address);//!< Set the target ip to new one.
  static void printIP(uint32_t ip);//!< Print ip for debug.
  static uint32_t resolveHost(std::string host);//!< Check address for Hostname.
  uint8_t isConnected() const;//!<Check if this network has a connection.

private:
  void construct(uint32_t address, uint16_t port, uint32_t sourceAddress, uint16_t sourcePort, bool TCPMode);//<! Open the Socket.
  static void initWinsocks();//!<Init the Sockets under windows.

  SOCKET mSocket;//!< Socket of the network.
  struct sockaddr_in mSource;//!< Source address(Local) of the network.
  struct sockaddr_in mDestination;//!< Destination(Target) of the Network. If 0 -> Broadcast.

  static uint32_t mInstanceCounter;//!< A static field how much Networks are used.
};

#endif /*NETWORK_H_*/
