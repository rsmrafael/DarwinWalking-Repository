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

class YUVImage {
public:

	struct YUVData {
		int Y;
		int U;
		int V;

		YUVData() {
			Y = 0 ;
			U = 0;
			V = 0;
		}

		YUVData(int y, int u, int v) {
			Y = y;
			U = u;
			V = v;
		}

		virtual ~YUVData() {
		}
	};

	YUVImage();
	YUVImage(const char *imageData, int width, int height);
	virtual ~YUVImage();

	void updateImage(char* imageData, int width, int height);
	void copyImage(const char* imageData, int width, int height);
	struct YUVData getValue(int x, int y) const;

	int getWidth() const {
		return mWidth;
	}
	int getHeight() const {
		return mHeight;
	}
	const char* getImage() const {
		return mImageData;
	}

private:
	int mWidth;
	int mHeight;
	char* mImageData;
	char* mImageBuffer;
};

template<>
class Serializer<YUVImage> {
public:
	static void serialize(const YUVImage& representation, ostream& stream);
	static void deserialize(istream& stream, YUVImage& representation);
};

#endif /* YUVIMAGE_H_ */
