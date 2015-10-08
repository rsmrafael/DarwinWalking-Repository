/*
 * MJPEGStreamer.h
 *
 *  Created on: 16.01.2013
 *      Author: Stefan Krupop
 */

#ifndef MJPEGSTREAMER_H_
#define MJPEGSTREAMER_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Network/HTTPServer.h"
#include "../../ConfigChangeListener.h"
#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#include "../../Representations/OpenCVImage.h"
#include "../../Representations/OpenCVMat.h"
#endif

#ifdef WIN32
#define HAVE_BOOLEAN
#define XMD_H
#endif
#include <jpeglib.h>

BEGIN_DECLARE_MODULE(MJPEGStreamer)
	REQUIRE(YUVImage,Image)
	//REQUIRE(YUVImage,CuttetImage)
#ifdef USE_OPENCV
	REQUIRE(OpenCVImage,OpenCVImage)
	REQUIRE(OpenCVMat,CannyMat)
	REQUIRE(OpenCVMat,ContourMat)
	REQUIRE(OpenCVMat,CircleMat)
	REQUIRE(OpenCVMat,EasyContour)
#endif
END_DECLARE_MODULE(MJPEGStreamer)

/**
 * MJPEGStreamer provides a motion-jpeg-stream via http service
 */
class MJPEGStreamer: public MJPEGStreamerBase, public ConfigChangeListener {
public:
	MJPEGStreamer();
	virtual ~MJPEGStreamer();
	void configChanged();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

private:
	//lint -e(1704)
	MJPEGStreamer(const MJPEGStreamer& cSource);
	MJPEGStreamer& operator=(const MJPEGStreamer& cSource);

	size_t compress_yuyv_to_jpeg(const YUVImage& src, uint8_t* buffer, size_t size, int quality) const;
	void dest_buffer(j_compress_ptr cinfo, uint8_t *buffer, size_t size, size_t *written) const;

    typedef struct {
      struct jpeg_destination_mgr pub; /* public fields */

      JOCTET * buffer;    /* start of buffer */

      uint8_t *outbuffer;
      size_t outbuffer_size;
      uint8_t *outbuffer_cursor;
      size_t *written;

    } mjpg_destination_mgr;

    typedef mjpg_destination_mgr * mjpg_dest_ptr;

    static void init_destination(j_compress_ptr cinfo);
    static boolean empty_output_buffer(j_compress_ptr cinfo);
    static void term_destination(j_compress_ptr cinfo);

    HTTPServer* mServer;
    int mSendModi;
};

#endif /* MJPEGSTREAMER_H_ */
