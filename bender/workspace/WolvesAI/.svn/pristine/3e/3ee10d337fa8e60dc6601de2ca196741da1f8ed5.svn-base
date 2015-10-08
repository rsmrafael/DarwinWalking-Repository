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

class CaptureSimulator : public CaptureSimulatorBase {
public:
	CaptureSimulator();
	virtual ~CaptureSimulator();

	virtual bool execute();

private:
	void ConvertBGR24BitToYUV444(const uint8_t *BGR, char *YUV, size_t pixels) const;
};

#endif /* CAPTURESIMULATOR_H_ */
