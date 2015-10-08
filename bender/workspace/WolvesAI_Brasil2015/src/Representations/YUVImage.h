/*
 * YUVImage.h
 *
 *  Created on: 21.08.2012
 *      Author: Stefan Krupop
 */

#ifndef YUVIMAGE_H_
#define YUVIMAGE_H_

#include "../Blackboard/Serializer.h"

//lint -e1711

using namespace std;

/**
 * Representation of YUVImage
 */
class YUVImage {
public:

	/**
	 * YUV data
	 */
	struct YUVData {
		int Y;		//!< Y channel
		int U;		//!< U channel
		int V;		//!< V channcel

		/**
		 * Constructor
		 */
		YUVData() {
			Y = 0 ;
			U = 0;
			V = 0;
		}

		/**
		 * Constructor
		 * @param y		y channel
		 * @param u		u channel
		 * @param v		v channel
		 */
		YUVData(int y, int u, int v) {
			Y = y;
			U = u;
			V = v;
		}

		virtual ~YUVData() {
		}
	};

	/**
	 * Constructor
	 */
	YUVImage();

	/**
	 * Constructor
	 * @param imageData		data of image
	 * @param width			width of image in pixel
	 * @param height		height of image in pixel
	 */
	YUVImage(const char *imageData, int width, int height);

	/**
	 * Destructor
	 */
	virtual ~YUVImage();

	/**
	 * update image with a pointer
	 * @param imageData		data of image
	 * @param width			width of image in pixel
	 * @param height		height of image in pixel
	 */
	void updateImage(char* imageData, int width, int height);

	/**
	 * copy image from a source
	 * @param imageData		source data of image
	 * @param width			width of image in pixel
	 * @param height		height of image in pixel
	 */
	void copyImage(const char* imageData, int width, int height);

	/**
	 * get YUV data value to position
	 * @param x		given x coordinate
	 * @param y 	given y coordinate
	 * @return YUVData at given position
	 */
	struct YUVData getValue(int x, int y) const;

	/**
	 * get the width of image
	 * @return width of image
	 */
	int getWidth() const {
		return mWidth;
	}

	/**
	 * get the height of image
	 * @return height of image
	 */
	int getHeight() const {
		return mHeight;
	}

	/**
	 * get image data
	 * @return pointer to image data
	 */
	const char* getImage() const {
		return mImageData;
	}

private:
	int mWidth;			//!< width of image in pixel
	int mHeight;		//!< height of image in pixel
	char* mImageData;	//!< pointer to image data
	char* mImageBuffer;	//!< pointer to image buffer
};

/**
 * spezialization of Serializer for YUVImage
 */
template<>
class Serializer<YUVImage> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const YUVImage& representation, ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, YUVImage& representation);
};

#endif /* YUVIMAGE_H_ */
