/*
 * YUVImage.cpp
 *
 *  Created on: 21.08.2012
 *      Author: Stefan Krupop
 */

#include "YUVImage.h"
#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"
#include <sstream>

#define SIZE_OF_YUV422_PIXEL	2

YUVImage::YUVImage() {
	// Default size
	mWidth = 320;
	mHeight = 240;
	// No image data given - create own buffer so that read/write access works
	mImageBuffer = (char*) malloc(
			(size_t) (uint32_t) (mWidth * mHeight * SIZE_OF_YUV422_PIXEL));
	mImageData = mImageBuffer;
}

YUVImage::YUVImage(const char *imageData, int width, int height) {
	mWidth = width;
	mHeight = height;
	mImageBuffer = (char*) malloc(
			(size_t) (uint32_t) (mWidth * mHeight * SIZE_OF_YUV422_PIXEL));
	if (mImageBuffer != NULL && imageData != NULL) {
		memcpy(mImageBuffer, imageData,
				(size_t) (uint32_t) (mWidth * mHeight * SIZE_OF_YUV422_PIXEL));
	}
	mImageData = mImageBuffer;
}

YUVImage::~YUVImage() {
	mImageData = NULL;
	free(mImageBuffer);
}

void YUVImage::copyImage(const char* imageData, int width, int height) {
	if (imageData != NULL) {
		if (width != mWidth || height != mHeight) {
			// Dimensions have changed, re-alloc
			free(mImageBuffer);
			mWidth = width;
			mHeight = height;
			mImageBuffer = (char*) malloc(
					(size_t) (uint32_t) (mWidth * mHeight
							* SIZE_OF_YUV422_PIXEL));
			mImageData = mImageBuffer;
		}
		if (mImageBuffer != NULL) {
			memcpy(mImageBuffer, imageData,
					(size_t) (uint32_t) (mWidth * mHeight
							* SIZE_OF_YUV422_PIXEL));
		}
	}
}

void YUVImage::updateImage(char* imageData, int width, int height) {
	if (imageData != NULL) {
		mImageData = imageData;
		mWidth = width;
		mHeight = height;
		if (mImageBuffer != NULL) {
			free(mImageBuffer);
			mImageBuffer = NULL;
		}
	}
}

void Serializer<YUVImage>::serialize(const YUVImage& representation,
		ostream& stream) {
	if (representation.getImage() != NULL) {
		protobuf::YUVImage img;

		img.set_height(representation.getHeight());
		img.set_width(representation.getWidth());
		img.set_data(representation.getImage(),
				(size_t) (uint32_t) (representation.getWidth()
						* representation.getHeight() * SIZE_OF_YUV422_PIXEL));

		OstreamOutputStream buf(&stream);
		img.SerializeToZeroCopyStream(&buf);
	}
}

void Serializer<YUVImage>::deserialize(istream& stream,
		YUVImage& representation) {
	protobuf::YUVImage img;

	IstreamInputStream buf(&stream);
	img.ParseFromZeroCopyStream(&buf);

	int width = img.width();
	int height = img.height();
	representation.updateImage(NULL, width, height);
}

struct YUVImage::YUVData YUVImage::getValue(int x, int y) const {
	struct YUVData ret;

	if (mImageData == NULL) {
		ret.Y = 0;
		ret.U = 0;
		ret.V = 0;
		return ret;
	}

	if (x >= mWidth) {
		x = mWidth - 1;
	} else if (x < 0) {
		x = 0;
	}
	if (y >= mHeight) {
		y = mHeight - 1;
	} else if (y < 0) {
		y = 0;
	}

	register uint32_t yOffset = 2 * (uint32_t) (y * mWidth + x);
	//lint --e{661} --e{662}
	ret.Y = (uint8_t) mImageData[yOffset];
	// ((x & 1)<<1) = 2 if x is odd and 0 if it's even
	ret.U = (uint8_t) mImageData[yOffset + 1 - ((x & 1) << 1)];
	ret.V = (uint8_t) mImageData[yOffset + 3 - ((x & 1) << 1)];
	return ret;
}

