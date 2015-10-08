/*
 * ControlService.cpp
 *
 *  Created on: 08.06.2010
 *      Author: Stefan
 */

#include <iostream>
#include <sstream>
#include <ios>
#include <iomanip>
#include "BotControlService.h"
#include "../Debugger.h"
#include "../../Config.h"

using namespace std;

BotControlService::BotControlService(Game* game) {
	mGame = game;
}

BotControlService::~BotControlService() {
	mGame = NULL;
}

void BotControlService::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ") {
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, (size_t)0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos) {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void BotControlService::newPacket(uint8_t* packet, size_t size) {
	string request = (char*)(packet);
	Debugger::DEBUG("BotControlService", "New packet: %s", request.c_str());

	if (request.substr((size_t)0, (size_t)4) == "GET ") {
		string item = request.substr((size_t)4, (size_t)5);

		ostringstream reply;
		reply << "VALUE " << item << " ";
		reply << std::fixed;
		if(item == "CONFI") {
			reply << Config::getInstance()->getConfigAsString();
		}
		if(item == "SETTI") {
			reply << Config::getInstance()->getConfigAsString();
		}

		string replyp = reply.str();
		Debugger::GetInstance()->sendPacket(getPacketID(), replyp.c_str(), replyp.length());
	} else if (request.substr((size_t)0, (size_t)4) == "SET ") {
		string item = request.substr((size_t)4, (size_t)5);
		string value = request.substr((size_t)10, request.length() - (size_t)10);

		if (item == "CONFI") {
			if (value == "SAVE") {
				Config::getInstance()->save();
			} else if (value.substr(0, 3) == "ALL") {
				string content = value.substr(4, value.length() - 4);
				Config::getInstance()->setConfig(content);
			} else {
				vector<string> tokens;
				Tokenize(value, tokens, ";");
				/* if (tokens.size() == (size_t)3) {
				 * 	string section 	= tokens.at((size_t)0);
					string key 		= tokens.at((size_t)1);
					string value 	= tokens.at((size_t)2);
					Config::getInstance()->set(section, key, value);
					Debugger::INFO("BotControlService", "Changed config entry [%s] %s = %s",
							section.c_str(), key.c_str(), value.c_str());
					}
				 */
				size_t i = 0;
				while (tokens.size() >= (size_t)(3 + i)) {
					string section 	= tokens.at((size_t)(i + 0));
					string key 		= tokens.at((size_t)(i + 1));
					string value 	= tokens.at((size_t)(i + 2));
					Config::getInstance()->set(section, key, value);
					Debugger::DEBUG("BotControlService", "Changed config entry [%s] %s = %s",
							section.c_str(), key.c_str(), value.c_str());
					i += 3;
				}
			}
		}

		Debugger::GetInstance()->sendPacket(getPacketID(), "READY", (size_t)5);
	}
}

string BotControlService::getPacketID() {
	return "CON";
}
