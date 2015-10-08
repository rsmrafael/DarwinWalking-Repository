/*
 * CaptureSimulator.h
 *
 *  Created on: 17.07.2013
 *      Author: Stefan Krupop
 */

#ifndef CAPTURESIMULATOR_H_
#define CAPTURESIMULATOR_H_

#include <string>
#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"

using namespace std;

BEGIN_DECLARE_MODULE(CaptureSimulator)
	PROVIDE(YUVImage,Image)
END_DECLARE_MODULE(CaptureSimulator)

/**
 * Capture Simulator
 */
class CaptureSimulator : public CaptureSimulatorBase {
public:

	/**
	 * Constructor for simulator capturing
	 */
	CaptureSimulator();

	/**
	 * Destructor
	 */
	virtual ~CaptureSimulator();

	/**
	 * update image
	 */
	virtual bool execute();

private:
	/**
	 * convert a BGR-24 to a YUV-444 image
	 * @param BGR		original BGR image
	 * @param YUV		YUV image
	 * @param pixels 	number of pixels
	 */
	void ConvertBGR24BitToYUV444(const uint8_t *BGR, char *YUV, size_t pixels) const;
};

#endif /* CAPTURESIMULATOR_H_ */
