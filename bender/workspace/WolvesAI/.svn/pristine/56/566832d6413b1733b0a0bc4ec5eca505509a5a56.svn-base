/*
 * RobotAudio.cpp
 *
 *  Created on: 07.04.2015
 *      Author: bombadil
 */
#ifdef ROBOTAUDIO
#include "RobotAudio.h"

RobotAudio* RobotAudio::mInstance = 0;



RobotAudio* RobotAudio::GetInstance(){
	if(mInstance==0){
		mInstance= new RobotAudio();
	}
	return mInstance;
}


RobotAudio::RobotAudio() {


}


RobotAudio::~RobotAudio() {

}
void RobotAudio::shutdown() {
	delete mInstance;
	mInstance = 0;
}
void RobotAudio::playASong(std::string path){
	if(music.getStatus()==music.Stopped){
		if (!music.openFromFile(path)){
			Debugger::ERR("RobotAudio","Cant Read File");
		}else{
			music.play();
		}
	}
}
void RobotAudio::BIER(){
	GetInstance()->playASong("Audio/Bier.ogg");
}
void RobotAudio::EXTERMINATE(){
	GetInstance()->playASong("Audio/Exterminate.ogg");
}
#endif // ROBOTAUDIO
