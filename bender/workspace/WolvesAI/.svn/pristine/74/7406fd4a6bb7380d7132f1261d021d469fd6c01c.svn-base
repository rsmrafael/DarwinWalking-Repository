/*
 * CaptureV4L2.h
 *
 *  Created on: 12.11.2012
 *      Author: Stefan Krupop
 */

#ifndef CAPTUREV4L2_H_
#define CAPTUREV4L2_H_

#include <string>
#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/CameraSettings.h"

using namespace std;

BEGIN_DECLARE_MODULE(CaptureV4L2)
	REQUIRE(CameraSettings,CameraSettings)
	PROVIDE(YUVImage,Image)
END_DECLARE_MODULE(CaptureV4L2)

class CaptureV4L2 : public CaptureV4L2Base {
public:
	CaptureV4L2(string devicePath);
	virtual ~CaptureV4L2();

	virtual bool execute();

private:
#ifndef WIN32
#ifndef __APPLE__
	int xioctl(int fd, unsigned long request, void* argp) const;
	bool setCameraParameters();
	bool setControl(uint32_t controlID, double value);
	bool captureStop();
	bool captureStart();
	bool deviceUninit();
	bool mmapInit();
	bool deviceInit();
	bool deviceClose();
	bool deviceOpen();
#endif
#endif
    struct buffer {
		void* start;
		size_t length;
    };

	string mDevicePath;
	int mWidth;
	int mHeight;
	int mFrameIntervalNumerator;
	int mFrameIntervalDenominator;
	bool mOldWasChanged;

    int mVideoDevice;

    struct buffer* mBuffers;
    uint32_t mBufferCount;
};

#endif /* CAPTUREV4L2_H_ */
