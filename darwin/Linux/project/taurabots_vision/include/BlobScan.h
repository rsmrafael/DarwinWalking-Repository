/*
 * Written by Rodrigo da Silva Guerra <tioguerra@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef BLOBSCAN_H
#define BLOBSCAN_H

#include "cblob.h"

class BlobScan
{
	public:

	/* Constructor
	 */
	BlobScan(uint8_t *pBuffer, int pWidth, int pHeight);

	/* Destructor
	 */
	~BlobScan();

	/* Scan the color segments in the image and stores internally
	 * the list of color blobs and their information
	 */
	void Scan();

	/* Gets the color values that are part of the blob from
	 * inside the given rectangle
	 */
	void GetColorFromRectangle(uint32_t pTopLeftx, uint32_t pTopLefty, uint32_t pBottomRightx, uint32_t pBottomRighty);

	/* Resets the color
	 */
	void ResetColor();

    /* Inverts the color map
     */
    void InvertColor();

	/* Inflate color region. The color region is defined by the bits of
	 * a RGB cube. Inflating make neighbour bits be included. The resulting
	 * effect is that the scanned blob start including colors slightly
	 * different from the originally defined. Blobs look cleaner, with less
	 * noise.
	 */
	void InflateColor();

	/* Returns the blob in the given coordinates
	 */
	int BlobAtPosition(uint32_t x, uint32_t y, blob_t &pReturnBlob);
	uint32_t BlobAtPosition(uint32_t x, uint32_t y);

	/* Returns the blob corresponding to the given index
	 */
	int BlobFromId(uint32_t pId, blob_t &pReturnBlob);

	/* Returns the blob of bigger mass
	 */
	int BiggerBlob(blob_t &pReturnBlob);

	/* Returns the blob of smaller mass
	 */
	int SmallerBlob(blob_t &pReturnBlob);

	/* Returns the blob most to the left
	 */
	int LeftMostBlob(blob_t &pReturnBlob);

	/* Returns the blob most to the right
	 */
	int RightMostBlob(blob_t &pReturnBlob);

	/* Return the blob most to the top
	 */
	int TopMostBlob(blob_t &pReturnBlob);

	/* Return the blob most to the bottom
	 */
	int BottomMostBlob(blob_t &pReturnBlob);

	/* Highlights all blobs (actually just makes darker
	 * the pixels that do not belong to any blob
	 */
	void HighlightAllBlobs(uint8_t *pRgbBuffer);
	void HighlightAllBlobs() {HighlightAllBlobs(mRgbBuffer);}

	/* Highlights one blob
	 */
	void HighlightOneBlob(uint8_t *pRgbBuffer, uint32_t pBlobId);
	void HighlightOneBlob(uint32_t pBlobId) {HighlightOneBlob(mRgbBuffer, pBlobId);}
	void HighlightOneBlob(uint8_t *pRgbBuffer, blob_t pBlob);
	void HighlightOneBlob(blob_t pBlob) {HighlightOneBlob(mRgbBuffer, pBlob);}

	/* Returns the real blob corresponding to the given fragment id
	 */
	int RealBlobFromFragment(uint32_t pFragmentId, uint32_t &pReturnId);

	/* Returns the total of real blobs found
	 */
	unsigned int TotalFound() {return mTotal;}

	/* Saves the blob color map pixel list to a file
	 */
	int ColorToFile(char *pFilename);

	/* Reads the blob color map pixel list from a file
	 */
	int ColorFromFile(char *pFilename);

    uint16_t *GetColorMap() { return mColorMap; }

    void SetColorMap(uint16_t *colorMap) { mColorMap = colorMap; }

	protected:

	/* Find extremes
	 */
	void FindExtremes();

	/* This is a pointer to the RGB image that will be scanned
	 */
	uint8_t *mRgbBuffer;

	/* The dimentions of the image.
	 * */
	uint32_t mWidth;
	uint32_t mHeight;

	/* This a color map. It is a cube where each of the three 
	 * dimensions represent the R, G, B values. When a bit in
	 * the cube is one this means the respective R, G, B values
	 * belong to the color map. If bit is zero then the
	 * respective R, G, B values do not belong to the map
	 */
	uint16_t *mColorMap;

	/* This is a blob map. The blob map is a matrix of the same
	 * dimensions as the scanned image. Each position of the matrix
	 * has a value which represents the blob to witch the respective
	 * pixel in the image belongs. If the value is zero this means the
	 * respective pixel does not belong to this blob.
	 */
	uint32_t *mBlobMap;

	/* This holds raw data about all blobs scanned. The index i in
	 * this array maps to the index i+1 in the blob map.
	 */
	blob_list_t mBlobFragments;

	/* Total of fragments
	 */
	uint32_t mTotalFragments;

	/* Total of blobs
	 */
	uint32_t mTotal;

	/* This array holds data about the actual blobs found
	 */
	blob_list_t mBlobs;

	/* Average color of the color map
	 */
	rgb_pixel_t mAverageColor;

	/* Found extremes?
	 */
	bool mHasFoundExtremes;

	/* blob of bigger mass
	 */
	uint32_t mBiggerBlob;

	/* blob of smaller mass
	 */
	uint32_t mSmallerBlob;

	/* Returns the blob most to the left
	 */
	uint32_t mLeftMostBlob;

	/* Returns the blob most to the right
	 */
	uint32_t mRightMostBlob;

	/* Return the blob most to the top
	 */
	uint32_t mTopMostBlob;

	/* Return the blob most to the bottom
	 */
	uint32_t mBottomMostBlob;

};

#endif

