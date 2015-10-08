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

/**
 * Tool for sending debug informations
 */
class Debugger : public Thread {
public:
	/**
	 * get the instance of Debugger (singleton)
	 */
    static Debugger* GetInstance();

    /**
	 * shutdown the instance and clean up
	 */
	static void shutdown();

    /**
     * create a simple debug message (low priority)
     * @param module	the name of the module where the message comes from
     * @param text		the text message
     */
    static void DEBUG(std::string module, std::string text);

    /**
	 * create a simple debug message (low priority)
	 * @param module	the name of the module where the message comes from
	 * @param format	the text message with optional format specifiers (subsequences beginning with %, see printf)
	 * @param ...		additional arguments
	 */
    static void DEBUG(std::string module, const char* format, ...);

    /**
	 * create an information debug message (medium priority)
	 * @param module	the name of the module where the message comes from
	 * @param text		the text message
	 */
    static void INFO(std::string module, std::string text);

    /**
	 * create an information debug message (medium priority)
	 * @param module	the name of the module where the message comes from
	 * @param format	the text message with optional format specifiers (subsequences beginning with %, see printf)
	 * @param ...		additional arguments
	 */
    static void INFO(std::string module, const char* format, ...);

    /**
	 * create a warning debug message (high priority)
	 * @param module	the name of the module where the message comes from
	 * @param text		the text message
	 */
    static void WARN(std::string module, std::string text);

    /**
	 * create a warning debug message (high priority)
	 * @param module	the name of the module where the message comes from
	 * @param format	the text message with optional format specifiers (subsequences beginning with %, see printf)
	 * @param ...		additional arguments
	 */
    static void WARN(std::string module, const char* format, ...);

    /**
	 * create an error debug message (highest priority)
	 * @param module	the name of the module where the message comes from
	 * @param text		the text message
	 */
    static void ERR(std::string module, std::string text);

    /**
	 * create an error debug message (highest priority)
	 * @param module	the name of the module where the message comes from
	 * @param format	the text message with optional format specifiers (subsequences beginning with %, see printf)
	 * @param ...		additional arguments
	 */
    static void ERR(std::string module, const char* format, ...);

    /**
     * announce a text message
     * @param text		text to be announced
     */
    static void announce(std::string text);

    /**
     * add a metric to debugger
     * @param group		the group name
     * @param name		the name of the metric
     * @param value		the value for the metric
     */
    static void addMetric(std::string group, std::string name, double value);

    /**
     * print all the metrics
     */
    void printMetrics();

    /**
     * announce the own IP address
     */
    void announceOwnIP() const;

    /**
     * send a packet to debugger
     * @param packetID		the identifier of the packet
     * @param data			the data of the packet
     * @param length		the length of the packet
     */
    void sendPacket(std::string packetID, const char* data, size_t length);

    /**
     * add a packet listener
     * @param listener		the new listener
     */
	void addPacketListener(PacketListener* listener);

protected:
	Debugger();
	virtual ~Debugger();
	std::map<std::string, std::set<PacketListener*>* > mListeners;	//!< packet listeners

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
