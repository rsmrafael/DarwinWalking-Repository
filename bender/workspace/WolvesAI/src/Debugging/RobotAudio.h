/*
 * RobotAudio.h
 *
 *  Created on: 07.04.2015
 *      Author: bombadil
 */
#ifdef ROBOTAUDIO
#ifndef ROBOTAUDIO_H_
#define ROBOTAUDIO_H_
#include <SFML/Audio.hpp>


#include <string>
#include "Debugger.h"

class RobotAudio {
public:

	static RobotAudio* GetInstance();
	static void shutdown();
	static void BIER();
	static void EXTERMINATE();

protected:
	RobotAudio();
	virtual ~RobotAudio();
private:
	static RobotAudio* mInstance;
	sf::Music music;
	void playASong(std::string path);
};

#endif /* ROBOTAUDIO_H_ */
#endif // ROBOTAUDIO
