/*
 * BodyDataService.cpp
 *
 *  Created on: 12.04.2010
 *      Author: Stefan
 */

#include <iostream>
#include <sstream>
#include <ios>
#include <iomanip>
#include "BodyDataService.h"
#include "../Debugger.h"

BodyDataService::BodyDataService(Body* body, RemoteBehavior* remoteAI) {
	mBody = body;
	mRemoteAI = remoteAI;
}

BodyDataService::~BodyDataService() {
	mBody = NULL;
	mRemoteAI = NULL;
}

void BodyDataService::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ") const {
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

void BodyDataService::newPacket(uint8_t* packet, size_t size) {
	string request = (char*)(packet);
	Debugger::DEBUG("BodyDataService", "New packet: %s", request.c_str());

	if (request.substr((size_t)0, (size_t)4) == "GET ") {
		string item = request.substr((size_t)4, (size_t)3);

		ostringstream reply;
		reply << "VALUE " << item << " ";
		reply << std::fixed;
		if (item == "IMU") {
			int pitch = 0;
			int roll = 0;
			mBody->GetIMUData(&pitch, &roll);
			reply << pitch << "," << roll << ends;
		} else if (item == "BAT") {
			double batVoltage = mBody->GetBatteryVoltage();
			reply << batVoltage << ends;
		}

		string replyp = reply.str();
		Debugger::GetInstance()->sendPacket(getPacketID(), replyp.c_str(), replyp.length());
	}
#ifdef _DEBUG
	else if (request.substr((size_t)0, (size_t)4) == "SET ") {
		string item = request.substr((size_t)4, (size_t)3);

		if (item == "WLK") {
			string cmd = request.substr((size_t)8, request.length() - (size_t)8);
			int16_t x = 0;
			int16_t y = 0;
			int16_t r = 0;
			bool turningAim = false;

			vector<string> tokens;
			Tokenize(cmd.c_str(), tokens, ",");
			if (tokens.size() == 4) {
				sscanf(tokens.at((size_t)0).c_str(), "%6hd", &x);
				sscanf(tokens.at((size_t)1).c_str(), "%6hd", &y);
				sscanf(tokens.at((size_t)2).c_str(), "%6hd", &r);
				int temp;
				sscanf(tokens.at((size_t)3).c_str(), "%6d", &temp);
				turningAim = (temp == 1);
			}

			mRemoteAI->SetWalkerCommand((int8_t)x, (int8_t)y, (int8_t)r, turningAim);
		} else if (item == "KCK") {
			string cmd = request.substr((size_t)8, request.length() - (size_t)8);
			vect3d_t ball;
			ball.x = 0;
			ball.y = 0;
			ball.z = 0;

			vector<string> tokens;
			Tokenize(cmd.c_str(), tokens, ",");

			sscanf(tokens.at((size_t)0).c_str(), "%6hd", &ball.x);
			sscanf(tokens.at((size_t)1).c_str(), "%6hd", &ball.y);
			sscanf(tokens.at((size_t)2).c_str(), "%6hd", &ball.z);
			if (tokens.size() >= 6) {
				vect3d_t target;
				target.x = 0;
				target.y = 0;
				target.z = 0;
				sscanf(tokens.at((size_t)3).c_str(), "%6hd", &target.x);
				sscanf(tokens.at((size_t)4).c_str(), "%6hd", &target.y);
				sscanf(tokens.at((size_t)5).c_str(), "%6hd", &target.z);
				if( tokens.size() == 6) {
					mRemoteAI->SetKickCommand( ball, target);
				} else {
					int16_t kicktype = 0;
					sscanf(tokens.at((size_t)6).c_str(), "%6hd", &kicktype);
					mRemoteAI->SetKickCommand( ball, target, (uint8_t)kicktype);
				}
			} else if (tokens.size() == 3) {
				mRemoteAI->SetKickCommand( ball);
			}
		} else if (item == "MOV") {
			string cmd = request.substr((size_t)8, request.length() - (size_t)8);
			mRemoteAI->SetBodyCommand(cmd);
		} else if (item == "P/T") {
			string cmd = request.substr((size_t)8, request.length() - (size_t)8);
			int16_t pan = 0;
			int16_t tilt = 0;

			vector<string> tokens;
			Tokenize(cmd.c_str(), tokens, ",");
			if (tokens.size() == 2) {
				sscanf(tokens.at((size_t)0).c_str(), "%6hd", &pan);
				sscanf(tokens.at((size_t)1).c_str(), "%6hd", &tilt);
			}

			mRemoteAI->SetPanTiltPosition(pan, tilt);
		} else if (item == "ACT") {
			string cmd = request.substr((size_t)8, request.length() - (size_t)8);
			if (cmd == "ENABLE") {
				mRemoteAI->SetActive(true);
			} else {
				mRemoteAI->SetActive(false);
			}
		}
	}
#endif
}

string BodyDataService::getPacketID() {
	return "BDA";
}
