/*
 * Debugger.h
 *
 *  Created on: 07.07.2009
 *      Author: Stefan
 */

#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#define DEBUGGER_DEFAULT_ADDRESS	"192.168.23.92"
#define DEBUGGER_DEFAULT_PORT		3745
#define DEBUGGER_BUFFER_SIZE		4096
#define DEBUGGER_BUFFER_THRESHOLD	 512//1024

#include <string>
#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <set>
#include <map>
#include "../Thread.h"
#include "../Network/Network.h"
#include "PacketListener.h"

class Debugger : public Thread {
public:
    static Debugger* GetInstance();
    static void DEBUG(std::string module, std::string text);
    static void DEBUG(std::string module, const char* format, ...);
    static void INFO(std::string module, std::string text);
    static void INFO(std::string module, const char* format, ...);
    static void WARN(std::string module, std::string text);
    static void WARN(std::string module, const char* format, ...);
    static void ERR(std::string module, std::string text);
    static void ERR(std::string module, const char* format, ...);

    static void announce(std::string text);

    static void addMetric(std::string group, std::string name, double value);
    void printMetrics();

    void announceOwnIP() const;
    void sendPacket(std::string packetID, const char* data, size_t length);
	void addPacketListener(PacketListener* listener);
	void shutdown(void) const;

protected:
	Debugger();
	virtual ~Debugger();
	std::map<std::string, std::set<PacketListener*>* > mListeners;

private:
	//lint -e(1704)
    Debugger(const Debugger& cSource);
    Debugger& operator=(const Debugger& cSource);

	void execute(void* arg);
	void addLog(uint16_t level, std::string module, std::string text);
	void notifyNewPacket(std::string packetType, uint8_t* packet, size_t size);
    void addMetricI(std::string group, std::string name, double value);
    void saveMetrics();

	static Debugger* mInstance;
	Network* mNetwork;

	uint8_t mTransmitBuffer[DEBUGGER_BUFFER_SIZE];
	size_t mTransmitBufferPos;

	pthread_mutex_t mMutex;

	static bool mSpeechDisabled;

	std::map<std::string, std::map<std::string, double> > mMetrics;
};

#endif /* DEBUGGER_H_ */
