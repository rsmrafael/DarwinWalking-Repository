/*
 * Debugger.cpp
 *
 *  Created on: 07.07.2009
 *      Author: Stefan
 */

#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <algorithm>
#include "Debugger.h"
#include "../Config.h"

using namespace std;

Debugger* Debugger::mInstance = 0;
bool Debugger::mSpeechDisabled = false;

Debugger* Debugger::GetInstance() {
	if (mInstance == 0) {
		mInstance = new Debugger();
	}
	return mInstance;
}

Debugger::Debugger() {
	string host = Config::GetString("Debugging", "address", DEBUGGER_DEFAULT_ADDRESS);
	uint32_t ip = Network::resolveHost(host);
	uint16_t port = (uint16_t)Config::GetInt("Debugging", "port", DEBUGGER_DEFAULT_PORT);
	cout << "[Debugger] Debugger will send data to ";
	Network::printIP(ip);
	cout << ":" << port << endl;
	mNetwork = new Network(ip, port, INADDR_ANY, port + 1);

	if(pthread_mutex_init(&mMutex, NULL)) {
		cout << "[Debugger] Could not initialize mutex!" << endl;
	}

	mTransmitBufferPos = 4;

	this->start(this);
}

Debugger::~Debugger() {
	printMetrics();
	saveMetrics();
	mMetrics.clear();

	Debugger::INFO("Debugger", "Shutting down debugging...");

	this->stop();
	pthread_mutex_destroy(&mMutex);
	map<string, set<PacketListener*>* >::iterator p;
	for (p = mListeners.begin(); p != mListeners.end(); ++p) {
		p->second->clear();
		delete p->second;
	}
	mListeners.clear();
	delete mNetwork;
}

void Debugger::shutdown() const {
	delete mInstance;
	mInstance = 0;
}

void Debugger::execute(void* arg) {
    char networkBuffer[1024];
    uint32_t fromAddr;
    struct timeval timeout;     // timeout for socket access

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;

	pthread_mutex_lock(&mMutex);
    mTransmitBufferPos = 4;
	pthread_mutex_unlock(&mMutex);
	do {
		if (mNetwork == NULL) {
			printf("<1> [Debugger] Network is null!");
			break;
		}
		size_t len = mNetwork->receiveData(&networkBuffer[0], (size_t)1024, &timeout, &fromAddr, NULL);
		if (len > 0) {
			if (networkBuffer[3] == ':') {
				networkBuffer[3] = '\0';
				string packetType = networkBuffer;
				notifyNewPacket(packetType, (uint8_t *)(networkBuffer + 4), len - 4);
				mNetwork->setRemoteIP(fromAddr);
			}
		}

		pthread_mutex_lock(&mMutex);
		if ((mTransmitBufferPos >= DEBUGGER_BUFFER_THRESHOLD) /*|| strchr((char *)&mTransmitBuffer[0], '\n')*/) {
			//cout << "[Debugger] execute: Sending data..." << endl;

			mTransmitBuffer[0] = 'L';
			mTransmitBuffer[1] = 'O';
			mTransmitBuffer[2] = 'G';
			mTransmitBuffer[3] = ':';
			//if (mNetwork->getRemoteIP() != ((127 << 24) | (0 << 16) | (0 << 8) | (1 << 0))) {
				if( !mNetwork->sendData((char *)&mTransmitBuffer[0], mTransmitBufferPos)) {
					printf("<2> [Debugger] Execute: Send error");
				}
			//}
			mTransmitBufferPos = 4;
		}
		pthread_mutex_unlock(&mMutex);

	} while (IsRunning());
}

void Debugger::DEBUG(string module, string text) {
#ifdef _DEBUG
	GetInstance()->addLog(4, module, text);
#endif
}

void Debugger::DEBUG(string module, const char* format, ...) {
#ifdef _DEBUG
	va_list args;
	char buffer[512];

	va_start(args, format);
	vsprintf(&buffer[0], format, args);
	va_end(args);
	GetInstance()->addLog(4, module, buffer);
#endif
}

void Debugger::INFO(string module, string text) {
	GetInstance()->addLog(3, module, text);
}

void Debugger::INFO(string module, const char* format, ...) {
	va_list args;
	char buffer[512];

	va_start(args, format);
	vsprintf(&buffer[0], format, args);
	va_end(args);
	GetInstance()->addLog(3, module, buffer);
}

void Debugger::WARN(string module, string text) {
	GetInstance()->addLog(2, module, text);
}

void Debugger::WARN(string module, const char* format, ...) {
	va_list args;
	char buffer[512];

	va_start(args, format);
	vsprintf(&buffer[0], format, args);
	va_end(args);
	GetInstance()->addLog(2, module, buffer);
}

void Debugger::ERR(string module, string text) {
	GetInstance()->addLog(1, module, text);
}

void Debugger::ERR(string module, const char* format, ...) {
	va_list args;
	char buffer[512];

	va_start(args, format);
	vsprintf(&buffer[0], format, args);
	va_end(args);
	GetInstance()->addLog(1, module, buffer);
}

void Debugger::announce(string text) {
	if (!mSpeechDisabled && Config::GetBoolean("Debugging", "talkingMode", false)) {
		replace(text.begin(), text.end(), '\'', ' ');
		int res = system(("espeak --stdout '" + text + "' | aplay -q &").c_str());
		if (res != 0) {
			Debugger::ERR("Debugger", "Failed to execute espeak, disabling speech announcements");
			mSpeechDisabled = true;
		}
	} else {
		Debugger::INFO("Debugger", "Would announce '%s', but I'm mute...", text.c_str());
	}
}

void Debugger::addMetric(string group, string name, double value) {
	GetInstance()->addMetricI(group, name, value);
}

void Debugger::addMetricI(string group, string name, double value) {
	pthread_mutex_lock(&mMutex);

	map<string, double>* grpptr;
	if (mMetrics.find(group) == mMetrics.end()) {
		map<string, double> grp = map<string, double>();
		mMetrics.insert(pair<string, map<string, double> >(group, grp));
	}
	grpptr = &mMetrics[group];

	map<string, double>::iterator end = grpptr->end();
	if (grpptr->find(name) == end) {
		grpptr->insert(pair<string, double>(name, value));
	} else {
		(*grpptr)[name] = value;
	}

	pthread_mutex_unlock(&mMutex);
}

void Debugger::printMetrics() {
	if (!mMetrics.empty()) {
		INFO("Debugger", "Metrics:");
		map<string, map<string, double> >::iterator p;
		for (p = mMetrics.begin(); p != mMetrics.end(); ++p) {
			INFO("Debugger", "  Group '%s':", p->first.c_str());

			map<string, double>::iterator p2;
			for (p2 = p->second.begin(); p2 != p->second.end(); ++p2) {
				INFO("Debugger", "    %s: %f", p2->first.c_str(), p2->second);
			}
		}
	} else {
		INFO("Debugger", "Metrics: No metrics available!");
	}
}

void Debugger::saveMetrics() {
	if (!mMetrics.empty()) {

		map<string, map<string, double> >::iterator p;
		for (p = mMetrics.begin(); p != mMetrics.end(); ++p) {
			fstream output(("metrics-" + p->first + ".csv").c_str(), ios::out | ios::trunc);
			if (!output.good()) {
				Debugger::ERR("Debugger", "Could not open output stream 'metrics-" + p->first + ".csv'");
				return;
			}
			map<string, double>::iterator p2;
			for (p2 = p->second.begin(); p2 != p->second.end(); ++p2) {
				if (p2 != p->second.begin()) {
					output << ",";
				}
				output << "\"" << p2->first << "\"";
			}
			output << "\n";
			for (p2 = p->second.begin(); p2 != p->second.end(); ++p2) {
				if (p2 != p->second.begin()) {
					output << ",";
				}
				output << p2->second;
			}
			output.flush();
			output.close();
		}
	}
}

void Debugger::announceOwnIP() const {
	if (mNetwork != NULL) {
		uint32_t ip = mNetwork->getInterfaceIP();
		char buffer[200];
		sprintf(&buffer[0], "My IP address is %u, %u, %u, %u", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, (ip & 0xFF));
		announce(string(buffer));
	}
}

void Debugger::sendPacket(string packetID, const char* data, size_t length) {
	char* senddata;

	size_t len = packetID.length() + 1 + length;
	senddata = (char*)malloc(len * sizeof(char));
	if (senddata == NULL) { return; }

	strcpy(senddata, packetID.c_str());
	senddata[3] = ':';
	memcpy((char *)(senddata + 4), data, length);

	if (mNetwork != NULL) {
		if( !mNetwork->sendData(senddata, len)) {
			printf("<2> [Debugger] sendPacket: error\n");
		}
	}

	free(senddata);
}

void Debugger::addPacketListener(PacketListener* listener) {
	string packetType = listener->getPacketID();

	if (mListeners.find(packetType) == mListeners.end()) {
		set<PacketListener*>* newSet = new set<PacketListener*>;
		mListeners.insert(pair<string, set<PacketListener*>* >(packetType, newSet));
		//lint -e{429} 'newSet' lost the custody
	}
	mListeners[packetType]->insert(listener);
	//lint -e{429} 'listener' lost the custody
}

void Debugger::notifyNewPacket(string packetType, uint8_t* packet, size_t size) {
	if (mListeners.find(packetType) != mListeners.end()) {
		std::set<PacketListener*>* listeners = mListeners[packetType];
		std::set<PacketListener*>::iterator iter;
		for (iter = listeners->begin(); iter != listeners->end(); ++iter) {
			(*iter)->newPacket(packet, size);
		}
	} else {
		WARN("Debugger", "No listener found for packet type %s!", packetType.c_str());
	}
}

void Debugger::addLog(uint16_t level, string module, string text) {
	/*timeval logTime;
	gettimeofday(&logTime, 0);*/

	ostringstream strout;
    string data;

    //strout << logTime.tv_sec << "." << (logTime.tv_usec / 1000);
    strout << "<" << level << "> [" << module << "] " << text << "\n";
    data = strout.str();

    pthread_mutex_lock(&mMutex);
    if (mTransmitBufferPos + data.length() <= DEBUGGER_BUFFER_SIZE) {
    	strcpy((char*)&mTransmitBuffer[mTransmitBufferPos], data.c_str());
    	mTransmitBufferPos += data.length();
    } else {
        printf("<2> [Debugger] addLog: Buffer is full for %i more bytes\n", (int)data.length());
        fflush(stdout);
    }
    pthread_mutex_unlock(&mMutex);

	cout << data << flush;
}



