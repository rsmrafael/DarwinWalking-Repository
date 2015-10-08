/*
 * CaptureV4L2.cpp
 *
 *  Created on: 12.11.2012
 *      Author: Stefan Krupop
 */

#include "CaptureV4L2.h"
#include "../../Debugging/Debugger.h"
#include "../../Blackboard/DataHolder.h"
#include "../../ModuleManager/ModuleManager.h"

#ifndef WIN32
#ifndef __APPLE__
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <errno.h>
#include <sys/mman.h>
#include <malloc.h>
#include <string.h>
#endif
#endif

#define CLEAR(x) memset(&(x), 0, sizeof (x))

//lint -e835 -e845

CaptureV4L2::CaptureV4L2(string devicePath) {
	mDevicePath = devicePath;
	mVideoDevice = 0;
	mWidth = getCameraSettings().width;
	mHeight = getCameraSettings().height;
	mFrameIntervalNumerator = getCameraSettings().frameIntervalNumerator;
	mFrameIntervalDenominator = getCameraSettings().frameIntervalDenominator;
	mOldWasChanged = getCameraSettings().changedStatus;
	mBuffers = NULL;
	mBufferCount = 0;
#ifndef WIN32
#ifndef __APPLE__
	// open and initialize device
	if (deviceOpen()) {
		if (deviceInit()) {
			setCameraParameters();
			// start capturing
			captureStart();
		}
	}
#endif
#endif
}

CaptureV4L2::~CaptureV4L2() {
#ifndef WIN32
#ifndef __APPLE__
	// stop capturing
	captureStop();

	// close device
	deviceUninit();
	deviceClose();
#endif
#endif
	free(mBuffers);
}

bool CaptureV4L2::execute() {
#ifndef WIN32
#ifndef __APPLE__
	fd_set fds;
	struct timeval tv;
	int r;

	// Check if camera settings changed
	if (getCameraSettings().changedStatus != mOldWasChanged) {
		if (mWidth != getCameraSettings().width || mHeight != getCameraSettings().height ||
				mFrameIntervalNumerator != getCameraSettings().frameIntervalNumerator ||
				mFrameIntervalDenominator != getCameraSettings().frameIntervalDenominator) {
			captureStop();
			deviceUninit();
			deviceClose();
			mWidth = getCameraSettings().width;
			mHeight = getCameraSettings().height;
			mFrameIntervalNumerator = getCameraSettings().frameIntervalNumerator;
			mFrameIntervalDenominator = getCameraSettings().frameIntervalDenominator;
			if (deviceOpen()) {
				if (deviceInit()) {
					setCameraParameters();
					captureStart();
				}
			}
		} else {
			setCameraParameters();
		}
		mOldWasChanged = getCameraSettings().changedStatus;
	}

	FD_ZERO(&fds);
	//lint --e(713)
	FD_SET((uint32_t)mVideoDevice, &fds);

	/* Timeout. */
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	r = select(mVideoDevice + 1, &fds, NULL, NULL, &tv);

	if (r <= 0) {
		Debugger::ERR("CaptureV4L2", "select: %d, %s", errno, strerror(errno));
		return false;
	}

	struct v4l2_buffer buf;
	CLEAR (buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(mVideoDevice, VIDIOC_DQBUF, &buf)) {
		switch (errno) {
			case EAGAIN:
			return false;
			case EIO:
			// Could ignore EIO, see spec
			// fall through
			default:
			Debugger::ERR("CaptureV4L2", "VIDIOC_DQBUF: %d, %s", errno, strerror(errno));
			return false;
		}
	}

	//assert (buf.index < n_buffers);

	getImage().updateImage((char*)mBuffers[buf.index].start, mWidth, mHeight);

	if (-1 == xioctl(mVideoDevice, VIDIOC_QBUF, &buf)) {
		Debugger::ERR("CaptureV4L2", "VIDIOC_QBUF: %d, %s", errno, strerror(errno));
		return false;
	}
#endif
#endif
	return true;
}

#ifndef WIN32
#ifndef __APPLE__
int CaptureV4L2::xioctl(int fd, unsigned long request, void* argp) const {
	int r;

	do r = ioctl(fd, request, argp);
	while (-1 == r && EINTR == errno);

	return r;
}

bool CaptureV4L2::setCameraParameters() {
	const CameraSettings& settings = getCameraSettings();

	setControl(V4L2_CID_BRIGHTNESS, settings.brightness);
	setControl(V4L2_CID_CONTRAST, settings.contrast);
	setControl(V4L2_CID_SATURATION, settings.saturation);
	//setControl(V4L2_CID_HUE_AUTO, settings.autohue);
	setControl(V4L2_CID_HUE, settings.hue);
	setControl(V4L2_CID_AUTO_WHITE_BALANCE, settings.autowhitebalance);
	setControl(V4L2_CID_RED_BALANCE, settings.redbalance);
	setControl(V4L2_CID_BLUE_BALANCE, settings.bluebalance);
	setControl(V4L2_CID_GAMMA, settings.gamma);
	setControl(V4L2_CID_EXPOSURE, settings.exposure);
	setControl(V4L2_CID_AUTOGAIN, settings.autogain);
	setControl(V4L2_CID_GAIN, settings.gain);
#ifdef V4L2_CID_POWER_LINE_FREQUENCY
	setControl(V4L2_CID_POWER_LINE_FREQUENCY, settings.powerlinefreq);
#endif
#ifdef V4L2_CID_WHITE_BALANCE_TEMPERATURE
	setControl(V4L2_CID_WHITE_BALANCE_TEMPERATURE, settings.whitebalancetemp);
#endif
#ifdef V4L2_CID_SHARPNESS
	setControl(V4L2_CID_SHARPNESS, settings.sharpness);
#endif
#ifdef V4L2_CID_BACKLIGHT_COMPENSATION
	setControl(V4L2_CID_BACKLIGHT_COMPENSATION, settings.backlightcompensation);
#endif
#ifdef V4L2_CID_EXPOSURE_AUTO
	setControl(V4L2_CID_EXPOSURE_AUTO, settings.autoexposure);
#endif
#ifdef V4L2_CID_EXPOSURE_ABSOLUTE
	setControl(V4L2_CID_EXPOSURE_ABSOLUTE, settings.exposure);
#endif

	return true;
}

bool CaptureV4L2::setControl(uint32_t controlID, double value) {
	struct v4l2_queryctrl queryctrl;
	struct v4l2_control control;

	CLEAR(queryctrl);
	queryctrl.id = controlID;

	if (-1 == xioctl(mVideoDevice, VIDIOC_QUERYCTRL, &queryctrl)) {
		if (errno != EINVAL) {
			Debugger::ERR("CaptureV4L2", "VIDIOC_QUERYCTRL: %d, %s", errno, strerror(errno));
		} else {
			Debugger::WARN("CaptureV4L2", "Control 0x%x is not supported", controlID);
		}
		return false;
	} else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
		Debugger::WARN("CaptureV4L2", "Control '%s' is not supported", queryctrl.name);
	} else {
		memset(&control, 0, sizeof(control));
		control.id = controlID;
		if (value == -1) {
			control.value = queryctrl.default_value;
		} else {
			control.value = queryctrl.minimum + (signed int)(((double)(queryctrl.maximum - queryctrl.minimum) / 1.0) * value);
		}

		Debugger::INFO("CaptureV4L2", "Setting control '%s' (%li to %li, default %li) to %li", queryctrl.name, queryctrl.minimum, queryctrl.maximum, queryctrl.default_value, control.value);

		if (-1 == xioctl(mVideoDevice, VIDIOC_S_CTRL, &control)) {
			Debugger::ERR("CaptureV4L2", "VIDIOC_S_CTRL: %d, %s", errno, strerror(errno));
			return false;
		}
	}
	return true;
}

bool CaptureV4L2::captureStop() {
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(mVideoDevice, VIDIOC_STREAMOFF, &type)) {
		Debugger::ERR("CaptureV4L2", "VIDIOC_STREAMOFF: %d, %s", errno, strerror(errno));
		return false;
	}

	return true;
}

bool CaptureV4L2::captureStart() {
	uint32_t i;
	enum v4l2_buf_type type;

	for (i = 0; i < mBufferCount; ++i) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == xioctl(mVideoDevice, VIDIOC_QBUF, &buf)) {
			Debugger::ERR("CaptureV4L2", "VIDIOC_QBUF: %d, %s", errno, strerror(errno));
			return false;
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(mVideoDevice, VIDIOC_STREAMON, &type)) {
		Debugger::ERR("CaptureV4L2", "VIDIOC_STREAMON: %d, %s", errno, strerror(errno));
		return false;
	}
	return true;
}

bool CaptureV4L2::deviceUninit() {
	if (mBuffers != NULL) {
		uint32_t i;

		for (i = 0; i < mBufferCount; ++i) {
			if (-1 == munmap(mBuffers[i].start, mBuffers[i].length)) {
				Debugger::ERR("CaptureV4L2", "munmap: %d, %s", errno, strerror(errno));
				return false;
			}
		}
	}

	free(mBuffers);
	mBuffers = NULL;
	return true;
}

bool CaptureV4L2::mmapInit() {
	struct v4l2_requestbuffers req;

	CLEAR (req);

	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(mVideoDevice, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			Debugger::ERR("CaptureV4L2", "%s does not support memory mapping", mDevicePath.c_str());
			return false;
		} else {
			Debugger::ERR("CaptureV4L2", "VIDIOC_REQBUFS: %d, %s", errno, strerror(errno));
			return false;
		}
	}

	if (req.count < 2) {
		Debugger::ERR("CaptureV4L2", "Insufficient buffer memory on %s", mDevicePath.c_str());
		return false;
	}

	mBuffers = (struct buffer*)calloc((size_t)(req.count), sizeof(*mBuffers));

	if (!mBuffers) {
		Debugger::ERR("CaptureV4L2", "Out of memory");
		return false;
	}

	for (mBufferCount = 0; mBufferCount < req.count; ++mBufferCount) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = mBufferCount;

		if (-1 == xioctl(mVideoDevice, VIDIOC_QUERYBUF, &buf)) {
			Debugger::ERR("CaptureV4L2", "VIDIOC_QUERYBUF: %d, %s", errno, strerror(errno));
			return false;
		}

		mBuffers[mBufferCount].length = buf.length;
		mBuffers[mBufferCount].start = mmap(NULL /* start anywhere */, (size_t)buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, mVideoDevice, (size_t)buf.m.offset);

		if (MAP_FAILED == mBuffers[mBufferCount].start) {
			Debugger::ERR("CaptureV4L2", "mmap: %d, %s", errno, strerror(errno));
			return false;
		}
	}
	return true;
}

bool CaptureV4L2::deviceInit() {
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	struct v4l2_streamparm streamparm;
	struct v4l2_fract *tpf;
	uint32_t min_val;

	if (-1 == xioctl(mVideoDevice, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			Debugger::ERR("CaptureV4L2", "%s is no V4L2 device", mDevicePath.c_str());
			return false;
		} else {
			Debugger::ERR("CaptureV4L2", "VIDIOC_QUERYCAP: %d, %s", errno, strerror(errno));
			return false;
		}
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		Debugger::ERR("CaptureV4L2", "%s is no video capture device", mDevicePath.c_str());
		return false;
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		Debugger::ERR("CaptureV4L2", "%s does not support streaming i/o", mDevicePath.c_str());
		return false;
	}

	// Select video input, video standard and tune here.
	CLEAR(cropcap);

	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (0 == xioctl(mVideoDevice, VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect; // reset to default

		if (-1 == xioctl(mVideoDevice, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
				case EINVAL:
				// Cropping not supported.
				break;
				default:
				// Errors ignored.
				break;
			}
		}
	} else {
		// Errors ignored.
	}

	CLEAR (fmt);

	// v4l2_format
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = (uint32_t)mWidth;
	fmt.fmt.pix.height = (uint32_t)mHeight;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl(mVideoDevice, VIDIOC_S_FMT, &fmt)) {
		Debugger::ERR("CaptureV4L2", "VIDIOC_S_FMT: %d, %s", errno, strerror(errno));
		return false;
	}

	// Note VIDIOC_S_FMT may change width and height.
	if (mWidth != (int)fmt.fmt.pix.width) {
		mWidth = (int)fmt.fmt.pix.width;
		Debugger::WARN("CaptureV4L2", "Image width set to %i by device %s.", mWidth, mDevicePath.c_str());
	}
	if (mHeight != (int)fmt.fmt.pix.height) {
		mHeight = (int)fmt.fmt.pix.height;
		Debugger::WARN("CaptureV4L2", "Image height set to %i by device %s.", mHeight, mDevicePath.c_str());
	}

	/* Buggy driver paranoia. */
	min_val = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min_val)
		fmt.fmt.pix.bytesperline = min_val;
	min_val = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min_val)
		fmt.fmt.pix.sizeimage = min_val;

	/* set frame rate */
	CLEAR(streamparm);
	streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	tpf = &streamparm.parm.capture.timeperframe;
	tpf->numerator = (uint32_t)mFrameIntervalNumerator;
	tpf->denominator = (uint32_t)mFrameIntervalDenominator;
	if (-1 == xioctl(mVideoDevice, VIDIOC_S_PARM, &streamparm)) {
		Debugger::ERR("CaptureV4L2", "VIDIOC_S_PARM: %d, %s", errno, strerror(errno));
		return false;
	}

	if (mFrameIntervalDenominator != (int)tpf->denominator || mFrameIntervalNumerator != (int)tpf->numerator) {
		Debugger::WARN("CaptureV4L2", "The V4L2 driver changed the time per frame to %d/%d\n", tpf->numerator, tpf->denominator);
		mFrameIntervalNumerator = (int)tpf->numerator;
		mFrameIntervalDenominator = (int)tpf->denominator;
	}

	return mmapInit();
}

bool CaptureV4L2::deviceClose() {
	if (-1 == close(mVideoDevice)) {
		Debugger::ERR("CaptureV4L2", "close: %d, %s", errno, strerror(errno));
		return false;
	}

	mVideoDevice = -1;
	return true;
}

bool CaptureV4L2::deviceOpen() {
	struct stat st;

	Debugger::INFO("CaptureV4L2", "Opening %s...",  mDevicePath.c_str());

	// stat file
	if (-1 == stat(mDevicePath.c_str(), &st)) {
		Debugger::ERR("CaptureV4L2", "Cannot identify '%s': %d, %s", mDevicePath.c_str(), errno, strerror(errno));
		return false;
	}

	// check if its device
	if (!S_ISCHR(st.st_mode)) {
		Debugger::ERR("CaptureV4L2", "%s is no device", mDevicePath.c_str());
		return false;
	}

	// open device
	mVideoDevice = open(mDevicePath.c_str(), O_RDWR /* required */| O_NONBLOCK, 0);

	// check if opening was successful
	if (-1 == mVideoDevice) {
		Debugger::ERR("CaptureV4L2", "Cannot open '%s': %d, %s", mDevicePath.c_str(), errno, strerror(errno));
		return false;
	}

	return true;
}
#endif
#endif
