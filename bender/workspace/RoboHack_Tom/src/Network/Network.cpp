#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#ifndef WIN32
#include <errno.h>
#else
#define WINVER 0x0501
#include <winsock2.h>
#include <Ws2tcpip.h>
#endif

#include "Network.h"
#include "../Debugging/Debugger.h"

using namespace std;

/**
 * This are the instances that are open used for WIN_32 to close all sockets.
 */
uint32_t Network::mInstanceCounter = 0;

/**
 * Open a Port with the parameter given over the private construct mehtod. Used no TCP connection.
 * @param address of the Target you want to connect with (IPv4).
 * @param port on with you want to connect with the target.
 * @param sourceAddress is the address of this robot/simulator (IPv4).
 * @param sourcePort is the Port of the source on the robot/simulator.
 */
Network::Network(uint32_t address, uint16_t port, uint32_t sourceAddress,
		uint16_t sourcePort) {
	construct(address, port, sourceAddress, sourcePort, false);
}

/**
 * Open a Port with the parameter given over the private construct mehtod. Used no TCP connection.
 * SourcePort is 0 for the construction (Standart).
 * @param address of the Target you want to connect with (IPv4).
 * @param port on with you want to connect with the target.
 * @param sourceAddress is the address of this robot/simulator (IPv4).
 */
Network::Network(uint32_t address, uint16_t port, uint32_t sourceAddress) {
	construct(address, port, sourceAddress, 0, false);
}

/**
 * Open a Port with the parameter given over the private construct mehtod. Used no TCP connection.
 * SourcePort is 0 for the construction (Standart).
 * sourceAddress is 0 standart using for local.
 * @param address of the Target you want to connect with (IPv4).
 * @param port on with you want to connect with the target.
 */
Network::Network(uint32_t address, uint16_t port) {
	construct(address, port, INADDR_ANY, 0, false);
}
/**
 * Open a Port with the parameter given over the private construct mehtod.
 * SourcePort is 0 for the construction (Standart).
 * sourceAddress is 0 standart using for local.
 * @param address of the Target you want to connect with (IPv4).
 * @param port on with you want to connect with the target.
 * @param TCPMode can be set here if you want a real tcp Connection.
 */
Network::Network(uint32_t address, uint16_t port, bool TCPMode) {
	construct(address, port, INADDR_ANY, 0, TCPMode);
}
/**
 * Open a Port with the parameter given. Inital the (WIN)Sockets.
 * Add one to mInstanceCounter to count the connections.
 * @param address of the Target you want to connect with (IPv4). If address is 0 try a Broadcast.
 * @param port on with you want to connect with the target.
 * @param sourceAddress is the address of this robot/simulator (IPv4).
 * @param sourcePort is the Port of the source on the robot/simulator.
 * @param TCPMode can be set here if you want a real tcp Connection.
 */
void Network::construct(uint32_t address, uint16_t port, uint32_t sourceAddress,
		uint16_t sourcePort, bool TCPMode) {
#ifdef WIN32
	if (mInstanceCounter == 0) {
		initWinsocks();
	}
#endif

	// Create socket
	//lint -e{641} Enum to int
	mSocket = socket(PF_INET, TCPMode ? SOCK_STREAM : SOCK_DGRAM, 0);
	if (mSocket == INVALID_SOCKET) {
		printf("<1> [Network] construct: Unable to create socket!\n");
	}

	// Set source address for datagram and bind to socket
	mSource.sin_family = PF_INET;
	mSource.sin_port = htons(sourcePort);
	mSource.sin_addr.s_addr = htonl(sourceAddress);
	if (!TCPMode) {
		if (bind(mSocket, (struct sockaddr *) &mSource,
				(socklen_t) sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
			printf("<1> [Network] construct: Unable to bind socket!\n");
		}
	}

	// Set destination address for datagram
	mDestination.sin_family = PF_INET;
	mDestination.sin_port = htons(port);
	if (address != 0) {
		mDestination.sin_addr.s_addr = htonl(address);
	} else {
		if (!TCPMode) {
			printf("<4> [Network] construct: Enabling broadcast\n");
			mDestination.sin_addr.s_addr = INADDR_BROADCAST;
			int so_broadcast = 1;
			if (setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST,
					(const char*) &so_broadcast,
					(socklen_t) sizeof(so_broadcast)) == SOCKET_ERROR) {
				printf("<1> [Network] construct: Could not set SO_BROADCAST option. Error: %d\n",
						errno);
			} else {
				printf("<2> [Network] construct: Successfully enabled SO_BROADCAST\n");
			}
		}
	}

	if (TCPMode) {
		if (connect(mSocket, (struct sockaddr *) &mDestination,
				(socklen_t) sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
			printf("<1> [Network] construct: Could not connect. Error: %d\n", errno);
#ifdef WIN32
			closesocket(mSocket);
#else
			close(mSocket);
#endif
			mSocket = INVALID_SOCKET;
		}
	}

	mInstanceCounter++;
}
/**
 * Destructor close socket under windows and reduce the mInstanceCounter.
 * If it is 0 WSACleanup is called to terminate winsocks.
 */
Network::~Network() {
	if (mSocket != INVALID_SOCKET) {
#ifdef WIN32
		closesocket(mSocket);
#else
		close(mSocket);
#endif
		mSocket = INVALID_SOCKET;
	}

	mInstanceCounter--;
#ifdef WIN32
	if (mInstanceCounter == 0) {
		// On windows platform: Terminate winsocks
		WSACleanup();
	}
#endif
}

/**
 * Send Data to the address of this network (Broadcast or address).
 * @param data is the data that should be send to the address.
 * @param length of the data in bytes.
 */
bool Network::sendData(const char* data, size_t length) {
	socklen_t addrLen = sizeof(struct sockaddr_in);
	ssize_t cnt; // number of bytes sent

	// Send datagram
	//lint --e(713)
	cnt = sendto(mSocket, data, length, 0, (struct sockaddr*) &mDestination,
			addrLen);
	if (cnt < 0) {
		printf("<1> [Network] sendData: Error in sendto: %d\n", errno);
		return false;
	}
	return true;
}

/**
 * Recive last message from the Network. Blocks until data is avaidable.
 * @param data reference where the data is stored.
 * @param maxLength of the data in bytes.
 * @return size of the data.
 */
size_t Network::receiveData(char* data, size_t maxLength) {
	struct sockaddr_in remoteAddress;
	socklen_t addressLen = sizeof(remoteAddress);
	return (size_t) recvfrom(mSocket, data, maxLength, 0,
			(struct sockaddr *) &remoteAddress, &addressLen);
}

/**
 * Recive last message from the Network. Dont Blocks get
 * @param data reference where the data is stored.
 * @param maxLength of the data in bytes.
 * @param timeout is the time he waits while he is waiting for data.
 * @param addr is a pointer where the methods saved the address where the message comes from.
 * @param port is a pointer where the methods saved the port on the message is addressing.
 * @return size of the data if avaidable. If Timeout or error return 0.
 */
//lint -efunc(818,Network::receiveData)
size_t Network::receiveData(char* data, size_t maxLength,
		struct timeval* timeout, uint32_t *addr, uint16_t *port) {
	fd_set setRead;
	int notused = 63;
	ssize_t cnt; // number of instance
	struct sockaddr_in sa;
	socklen_t sa_len = sizeof(sa);
	// UINT16 size;

	// Add application socket to list
	FD_ZERO(&setRead);
	FD_SET(mSocket, &setRead);

	// Perform select operation to block until a port is ready
	cnt = select(notused, &setRead, NULL, NULL, timeout);
	if (cnt == SOCKET_ERROR) {
		printf("<1> [Network] receiveData: select returned an error!\n");
	}

	// Check if at least one port has valid data
	if (cnt > 0) {
		// Check if this socket is ready
		if (FD_ISSET(mSocket, &setRead)) {
			// Read datagram
			memset(&sa, 0, sizeof(struct sockaddr_in));
			//lint --e(713)
			cnt = recvfrom(mSocket, data, maxLength, 0, (struct sockaddr *) &sa,
					&sa_len);
			if (cnt > 0) {
				if (addr != NULL) { // only perform assignment if a valid buffer was provided
					*addr = ntohl(sa.sin_addr.s_addr);
				}
				if (port != NULL) { // only perform assignment if a valid buffer was provided
					*port = ntohs(sa.sin_port);
				}
				return (size_t) cnt;
			}
		}
	} else {
		// No message!
		return (size_t) 0;
	}
	return (size_t) 0;
}

/**
 * Inital the Socks for windows.
 */
void Network::initWinsocks() {
	// On windows platform: Init winsocks
#ifdef WIN32
	WSADATA wsaData;
	WORD wVersionRequested;
	int err;

	wVersionRequested = 2; // 2.0 and above version of WinSock
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		printf("<1> [Network] initWinsock: Could not find a usable WinSock DLL!\n");
	}
#endif
}

/**
 * Check wich address is the Local given as param in contructor to this network and returns it.
 * @return the ip of this machine.
 */
uint32_t Network::getLocalIP() {
	uint32_t ip = ntohl(mSource.sin_addr.s_addr);
	if (ip == 0) {
		socklen_t size = sizeof(struct sockaddr_in);
		if (getsockname(mSocket, (struct sockaddr *) &mSource,
				&size) == SOCKET_ERROR) {
			return 0;
		}
		ip = ntohl(mSource.sin_addr.s_addr);
	}
	return ip;
}

/**
 * @return the destination ip address.
 */
uint32_t Network::getRemoteIP() {
	return ntohl(mDestination.sin_addr.s_addr);
}

/**
 * Get the interfaceIP, so basicly checks the local ip by himself. No need for a network.
 * Basiclly can be used to identify ip address.
 * @return the local IP address. If there are multiple posibilitys only the last one is returned.
 */
uint32_t Network::getInterfaceIP() const {
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		printf("<1> [Network] getInterfaceIP: Error getting local host name: %d\n", errno);
		return 0;
	}
	printf("<2> [Network] getInterfaceIP: Host name is %s\n", ac);

	struct addrinfo hints, *result, *res;
	memset(&hints, 0, sizeof(hints));
	//lint -e{641} Enum to int
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	if (getaddrinfo(ac, NULL, &hints, &result) != 0) {
		printf("<1> [Network] getInterfaceIP: Error getting addresses: %d\n",
				errno);
		return 0;
	}

	uint32_t ret = 0;
	for (res = result; res != NULL; res = res->ai_next) {
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		memcpy(&addr, res->ai_addr,
				static_cast<size_t>((uint16_t) (res->ai_addrlen)));
		printf("<2> [Network] getInterfaceIP: Address: %s\n",
				inet_ntoa(addr.sin_addr));
		ret = ntohl(addr.sin_addr.s_addr);
	}

	freeaddrinfo(result);

	return ret;
}

/**
 * Set the ip address used as target for this network.
 * @param address of the target(IPv4). If the ip is 0 the address is set to broadcast.
 */
void Network::setRemoteIP(uint32_t address) {
	if (address != 0) {
		mDestination.sin_addr.s_addr = htonl(address);
	} else {
		mDestination.sin_addr.s_addr = INADDR_BROADCAST;
	}
}

/**
 * Print out the ip used for debugging.
 * @param ip is the ipaddress that should be printed (ipv4).
 */
void Network::printIP(uint32_t ip) {
	printf("%u.%u.%u.%u", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF,
			(ip >> 8) & 0xFF, (ip & 0xFF));
}

/**
 * Find the ip address to the host name in this network.
 * @param host is the Name of the host.
 * @return ip of the host (ipv4)
 */
uint32_t Network::resolveHost(string host) {
	struct hostent *hp;
	struct sockaddr_in ip;

#ifdef WIN32
	if (mInstanceCounter == 0) {
		initWinsocks();
	}
#endif

	hp = gethostbyname(host.c_str());
	if (hp == NULL) {
		printf("<1> [Network] resolveHost: Could not resolve host!\n");
		return 0;
	}
	memset(&ip, 0, sizeof(ip));
	memcpy(&ip.sin_addr, hp->h_addr,
			static_cast<size_t>((uint16_t) (hp->h_length)));

	if (mInstanceCounter == 0) {
		// On windows platform: Terminate winsocks
#ifdef WIN32
		WSACleanup();
#endif
	}

	return ntohl(ip.sin_addr.s_addr);
}

/**
 * Checks if the socket used by the network is valid.
 * @return positive true, negative false.
 */
uint8_t Network::isConnected() const {
	return mSocket != INVALID_SOCKET;
}
