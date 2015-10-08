/*
 * CaptureV4L1.cpp
 *
 *  Created on: 21.08.2012
 *      Author: Stefan Krupop
 */

#include "CaptureV4L1.h"
#include "../../Debugging/Debugger.h"
#include "../../Blackboard/DataHolder.h"
#include "../../ModuleManager/ModuleManager.h"

#ifndef WIN32
#ifndef __APPLE__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#endif
#endif

//lint -e722 -e835

CaptureV4L1::CaptureV4L1(string devicePath) {
	mDevicePath = devicePath;
	mWidth = 0;
	mHeight = 0;
	mVideoBuffer = NULL;

#ifndef WIN32
#ifndef __APPLE__
	mDeviceDescriptor = 0;
	mFrameID = 0;
	mVmmap.format = 0;
	mVmbuf.frames = 0;
	if (!openCam()) {
		Debugger::ERR("CaptureV4L1", "Could not open camera!");
	}
#endif
#endif
}

CaptureV4L1::~CaptureV4L1() {
#ifndef WIN32
#ifndef __APPLE__
	closeCam();
#endif
#endif
	mVideoBuffer = NULL;
}

bool CaptureV4L1::execute() {
#ifndef WIN32
#ifndef __APPLE__
	char *ptr;

	if (mDeviceDescriptor <= 0) {
		Debugger::WARN("CaptureV4L1", "Camera not open, disabling module");
		ModuleManager::GetInstance()->disableModule(this);
		return false;
	}
	while (ioctl(mDeviceDescriptor, VIDIOCSYNC, &mFrameID) < 0 && (errno == EAGAIN || errno == EINTR));
	ptr = mVideoBuffer + mVmbuf.offsets[mFrameID];

	// this is now the grabbing frame
	mFrameID = (mFrameID + 1) % ((uint32_t)mVmbuf.frames);

	// setup to capture the next frame
	mVmmap.frame = mFrameID;
	if (ioctl(mDeviceDescriptor, VIDIOCMCAPTURE, &mVmmap) < 0) {
		Debugger::ERR("CaptureV4L1", "%s: VIDIOCMCAPTURE error", mDevicePath.c_str());
		return false;
	}

	getImage().updateImage(ptr, mWidth, mHeight);
#endif
#endif
	return true;
}

#ifndef WIN32
#ifndef __APPLE__
bool CaptureV4L1::openCam() {
	struct video_capability vidcap;
	struct video_picture pict;
	int err;

	Debugger::INFO("CaptureV4L1", "Opening %s...",  mDevicePath.c_str());
	mDeviceDescriptor = open(mDevicePath.c_str(), O_RDWR);
	if (mDeviceDescriptor < 0) {
		Debugger::ERR("CaptureV4L1", "error, cannot open: %s...", mDevicePath.c_str());
		return false;
	}
	err = ioctl(mDeviceDescriptor, VIDIOCGCAP, &vidcap);
	if (err != 0) {
		Debugger::ERR("CaptureV4L1", "error, cannot get device capabilities: %s", mDevicePath.c_str());
		close(mDeviceDescriptor);
		mDeviceDescriptor = 0;
		return false;
	}
	Debugger::INFO("CaptureV4L1", "Found Device: %s...", mDevicePath.c_str());
	// get picture properties
	err = ioctl(mDeviceDescriptor, VIDIOCGPICT, &pict);
	if (err < 0) {
		Debugger::ERR("CaptureV4L1", "error, cannot get picture properties: %s", mDevicePath.c_str());
		close(mDeviceDescriptor);
		mDeviceDescriptor = 0;
		return false;
	}
	pict.brightness = ((8 << 3) & 0xf8) | (2 & 0x07);
	pict.colour = 2;
	pict.contrast = 2;
	pict.whiteness = 3;
	if (!tryFormat(mDeviceDescriptor, &pict, DEFAULT_FMT, DEFAULT_LEN)) {
		close(mDeviceDescriptor);
		mDeviceDescriptor = 0;
		Debugger::ERR("CaptureV4L1", "unsupported video pixel format: %s", mDevicePath.c_str());
		return false;
	}
	mWidth = getCameraSettings().width;
	mHeight = getCameraSettings().height;
	if (!trySize(mDeviceDescriptor, (uint32_t)mWidth, (uint32_t)mHeight)) {
		close(mDeviceDescriptor);
		mDeviceDescriptor = 0;
		return false;
	}

	if (doMmap(mDeviceDescriptor, &mVmbuf, &mVideoBuffer)) {
		Debugger::ERR("CaptureV4L1", "error, cannot mmap: %s", mDevicePath.c_str());
		close(mDeviceDescriptor);
		mDeviceDescriptor = 0;
		return false;
	}
	mFrameID = 0;

	// start to grab
	mVmmap.height = mHeight;
	mVmmap.width = mWidth;
	mVmmap.format = pict.palette;

	for (int i = 0; i < mVmbuf.frames; i++) {
		mVmmap.frame = (uint32_t)i;
		ioctl(mDeviceDescriptor, VIDIOCMCAPTURE, &mVmmap);
	}

	return true;
}

bool CaptureV4L1::closeCam() {
	if (mDeviceDescriptor > 0) {
		// unmap the capture memory
		munmap(mVideoBuffer, (size_t)(uint32_t)(mVmbuf.size));
		close(mDeviceDescriptor);
		mDeviceDescriptor = 0;
	}

	return true;
}

int CaptureV4L1::tryFormat(int fd, struct video_picture *pict, uint16_t palette, uint16_t depth) const {
	int err;
	pict->palette = palette;
	pict->depth = depth;
	err = ioctl(fd, VIDIOCSPICT, pict);
	if (err < 0) {
		Debugger::ERR("CaptureV4L1", "could not set picture properties: %s", strerror(errno));
		return 0;
	}
	return 1;
}


int CaptureV4L1::trySize(int fd, uint32_t width, uint32_t height) const {
	struct video_window win;
	int err;
	memset(&win, 0, sizeof(win));

	//set picture size
	win.x = win.y = 0;
	win.width = width;
	win.height = height;
	win.flags = 0;
	win.clips = NULL;
	win.clipcount = 0;

	Debugger::INFO("CaptureV4L1", "trying to set capture size to %ix%i", width, height);
	err = ioctl(fd, VIDIOCSWIN, &win);
	if (err < 0) {
		Debugger::ERR("CaptureV4L1", "could not set window size: %s", strerror(errno));
		return 0;
	}

	err = ioctl(fd, VIDIOCGWIN, &win);
	if (err < 0) {
		Debugger::ERR("CaptureV4L1", "could not get window size: %s", strerror(errno));
		return 0;
	}
	if (width != win.width || height != win.height) {
		Debugger::ERR("CaptureV4L1", "capture size is not what we expected: asked for %ix%i and got %ix%i", width, height, win.width, win.height);
		return 0;
	}
	Debugger::INFO("CaptureV4L1", "capture size set to %ix%i", width, height);
	return 1;
}

int CaptureV4L1::doMmap(int fd, struct video_mbuf * pvmbuf, char ** pbuf) const {
	int err;
	memset((void*) pvmbuf, 0, sizeof(*pvmbuf));
	// try to get mmap properties
	err = ioctl(fd, VIDIOCGMBUF, pvmbuf);
	if (err < 0) {
		Debugger::ERR("CaptureV4L1", "could not get mmap properties: %s", strerror(errno));
		return -1;
	}

	*pbuf = (char*) mmap(NULL, (size_t)(uint32_t)(pvmbuf->size), PROT_READ, MAP_PRIVATE, fd, (__off_t)0);
	if (*pbuf == (void*) -1) {
		Debugger::ERR("CaptureV4L1", "could not mmap: %s", strerror(errno));
		return -1;
	}
	return 0;
}
#endif
#endif

