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
		if (item == "CONFI") {
			reply << Config::GetInstance()->getConfigAsString();
		}

		string replyp = reply.str();
		Debugger::GetInstance()->sendPacket(getPacketID(), replyp.c_str(), replyp.length());
	} else if (request.substr((size_t)0, (size_t)4) == "SET ") {
		string item = request.substr((size_t)4, (size_t)5);
		string value = request.substr((size_t)10, request.length() - (size_t)10);

		if (item == "CONFI") {
			if (value == "SAVE") {
				Config::GetInstance()->saveConfig();
			} else {
				vector<string> tokens;
				Tokenize(value.c_str(), tokens, ";");
				if (tokens.size() == (size_t)3) {
					Config::SetString(tokens.at((size_t)0), tokens.at((size_t)1), tokens.at((size_t)2));
					Debugger::INFO("BotControlService", "Changed config.ini setting '%s' -> '%s' to '%s'", tokens.at((size_t)0).c_str(), tokens.at((size_t)1).c_str(), tokens.at((size_t)2).c_str());
				}
			}
		}

		Debugger::GetInstance()->sendPacket(getPacketID(), "READY", (size_t)5);
	}
}

string BotControlService::getPacketID() {
	return "CON";
}
