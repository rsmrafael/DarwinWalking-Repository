/*
 * CaptureV4L1.h
 *
 *  Created on: 21.08.2012
 *      Author: Stefan Krupop
 */

#ifndef CAPTUREV4L1_H_
#define CAPTUREV4L1_H_

#include <string>
#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/CameraSettings.h"

#ifndef WIN32
#ifndef __APPLE__
#include <linux/videodev.h>
//#include <libv4l1-videodev.h>
#endif
#endif

#define DEFAULT_FMT VIDEO_PALETTE_YUV422
#define DEFAULT_LEN 16

using namespace std;

BEGIN_DECLARE_MODULE(CaptureV4L1)
	REQUIRE(CameraSettings,CameraSettings)
	PROVIDE(YUVImage,Image)
END_DECLARE_MODULE(CaptureV4L1)

/**
 * Capture with Video4Linux (V4L1)
 */
class CaptureV4L1 : public CaptureV4L1Base {
public:

	/**
	 * Constructor for V4L1 capturing
	 * @param devicePath	path to device, e.g. "/dev/video0"
	 */
	CaptureV4L1(string devicePath);

	/**
	 * Destructor
	 */
	virtual ~CaptureV4L1();

	/**
	 * update image
	 */
	virtual bool execute();

private:
#ifndef WIN32
#ifndef __APPLE__
	bool openCam();
	bool closeCam();
	int tryFormat(int fd, struct video_picture *pict, uint16_t palette, uint16_t depth) const;
	int trySize(int fd, uint32_t width, uint32_t height) const;
	int doMmap(int fd, struct video_mbuf * pvmbuf, char ** pbuf) const;
#endif
#endif
	string mDevicePath;
	int mWidth;
	int mHeight;

    char *mVideoBuffer;
#ifndef WIN32
#ifndef __APPLE__
    int mDeviceDescriptor;
    struct video_mmap mVmmap;
    struct video_mbuf mVmbuf;
    uint32_t mFrameID;
#endif
#endif
};

#endif /* CAPTUREV4L1_H_ */
