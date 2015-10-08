/*
 * MJPEGStreamer.cpp
 *
 *  Created on: 16.01.2013
 *      Author: Stefan Krupop
 */

#include "MJPEGStreamer.h"
#include <stdlib.h>
#include <string.h>
#include "../../Config.h"

#define OUTPUT_BUF_SIZE  4096

MJPEGStreamer::MJPEGStreamer():mSendModi(0) {
    mServer = new HTTPServer(8080, (size_t)(800*600*2), "./");
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

MJPEGStreamer::~MJPEGStreamer() {
	Config::getInstance()->removeConfigChangeHandler(this);
	delete mServer;
}

bool MJPEGStreamer::execute() {
	pthread_mutex_lock(&(mServer->Mutex));

	if(mServer->getClientRequest()) {
#ifdef USE_OPENCV
		if(mSendModi == 0) {
			mServer->setDataBufferSize(compress_yuyv_to_jpeg(getImage(), mServer->getDataBuffer(), (size_t)(800*600*2), 80));
		} else if(mSendModi==1){
			vector<uchar> buf;
			cv::Mat in;

			cvtColor(getCannyMat().getMat(), in, CV_GRAY2BGR);
			cv::imencode(".jpg",in,buf);
			mServer->setBuffer(buf.data());

			mServer->setDataBufferSize(buf.size());
		}else if(mSendModi==2||mSendModi==3){
			vector<uchar> buf;
			cv::Mat in;
			if(mSendModi==2){
				cvtColor(getContourMat().getMat(), in, CV_YUV2BGR);
			}else{
				cvtColor(getCircleMat().getMat(), in, CV_YUV2BGR);
			}
			cv::imencode(".jpg",in,buf);
			mServer->setBuffer(buf.data());

			mServer->setDataBufferSize(buf.size());
		}
#else
		mServer->setDataBufferSize(compress_yuyv_to_jpeg(getImage(), mServer->getDataBuffer(), (size_t)(800*600*2), 80));
#endif
	}

	mServer->notifyDataBufferUpdate();
	mServer->setClientRequest(false);
	pthread_mutex_unlock(&(mServer->Mutex));

	return true;
}

size_t MJPEGStreamer::compress_yuyv_to_jpeg(const YUVImage& src, uint8_t* buffer, size_t size, int quality) const {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    uint8_t *line_buffer;
    const uint8_t *yuyv;
    int z;
    static size_t written;

    line_buffer = (uint8_t*)calloc((size_t)(uint32_t)(src.getWidth() * 3), (size_t)1);
    if (line_buffer == NULL) {
    	return 0;
    }
    yuyv = (const uint8_t*)src.getImage();

    cinfo.err = jpeg_std_error (&jerr);
    jpeg_create_compress (&cinfo);
    /* jpeg_stdio_dest (&cinfo, file); */
    dest_buffer(&cinfo, buffer, size, &written);

    cinfo.image_width = (uint32_t)src.getWidth();
    cinfo.image_height = (uint32_t)src.getHeight();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults (&cinfo);
    jpeg_set_quality (&cinfo, quality, TRUE);

    jpeg_start_compress (&cinfo, TRUE);

    z = 0;
    while (cinfo.next_scanline < (uint32_t)src.getHeight()) {
        int x;
        uint8_t *ptr = line_buffer;

        for (x = 0; x < src.getWidth(); x++) {
            int r, g, b;
            int y, u, v;

            if (!z)
                y = yuyv[0] << 8;
            else
                y = yuyv[2] << 8;
            u = yuyv[1] - 128;
            v = yuyv[3] - 128;

            //lint -e{702}
            r = (y + (359 * v)) >> 8;
            //lint -e{702}
            g = ((y - (88 * u)) - (183 * v)) >> 8;
            //lint -e{702}
            b = (y + (454 * u)) >> 8;

            *(ptr++) = (uint8_t)((r > 255) ? 255 : ((r < 0) ? 0 : r));
            *(ptr++) = (uint8_t)((g > 255) ? 255 : ((g < 0) ? 0 : g));
            *(ptr++) = (uint8_t)((b > 255) ? 255 : ((b < 0) ? 0 : b));

            if (z++) {
                z = 0;
                yuyv += 4;
            }
        }

        row_pointer[0] = line_buffer;
        jpeg_write_scanlines (&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress (&cinfo);
    jpeg_destroy_compress (&cinfo);

    free (line_buffer);

    return (written);
}

/******************************************************************************
Description.: Prepare for output to a stdio stream.
Input Value.: buffer is the already allocated buffer memory that will hold
              the compressed picture. "size" is the size in bytes.
Return Value: -
******************************************************************************/
void MJPEGStreamer::dest_buffer(j_compress_ptr cinfo, uint8_t *buffer, size_t size, size_t *written) const {
  mjpg_dest_ptr dest;

  if (cinfo->dest == NULL) {
    cinfo->dest = (struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(mjpg_destination_mgr));
  }

  //lint -e{826} Pointer conversion ok
  dest = (mjpg_dest_ptr) cinfo->dest;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
  dest->outbuffer = buffer;
  dest->outbuffer_size = size;
  dest->outbuffer_cursor = buffer;
  dest->written = written;
}

/******************************************************************************
Description.:
Input Value.:
Return Value:
******************************************************************************/
void MJPEGStreamer::init_destination(j_compress_ptr cinfo) {
  //lint -e{826} Pointer conversion ok
  mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;

  /* Allocate the output buffer --- it will be released when done with image */
  dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE, OUTPUT_BUF_SIZE * sizeof(JOCTET));

  *(dest->written) = 0;

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

/******************************************************************************
Description.: called whenever local jpeg buffer fills up
Input Value.:
Return Value:
******************************************************************************/
//lint -e{818} cinfo can't be declared as const because of given function pointer
boolean MJPEGStreamer::empty_output_buffer(j_compress_ptr cinfo) {
  //lint -e{826} Pointer conversion ok
  mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;

  memcpy(dest->outbuffer_cursor, dest->buffer, (size_t)OUTPUT_BUF_SIZE);
  dest->outbuffer_cursor += OUTPUT_BUF_SIZE;
  *(dest->written) += OUTPUT_BUF_SIZE;

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

  return TRUE;
}

/******************************************************************************
Description.: called by jpeg_finish_compress after all data has been written.
              Usually needs to flush buffer.
Input Value.:
Return Value:
******************************************************************************/
//lint -e{818} cinfo can't be declared as const because of given function pointer
void MJPEGStreamer::term_destination(j_compress_ptr cinfo) {
  //lint -e{826} Pointer conversion ok
  mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
  size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

  /* Write any data remaining in the buffer */
  memcpy(dest->outbuffer_cursor, dest->buffer, datacount);
  dest->outbuffer_cursor += datacount;
  *(dest->written) += datacount;
}

void MJPEGStreamer::configChanged() {
	ConfigFile *config = Config::getInstance();
	mSendModi = config->get<int>("Debugging", "imgSendModi", 0);
}
