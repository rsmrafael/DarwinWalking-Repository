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

#include "BlobScan.h"

/* Constructor takes a base buffer pointer as argument
 */
BlobScan::BlobScan(uint8_t *pBuffer, int pWidth, int pHeight)
{
	/* Inititalize the info about the data source */
	mRgbBuffer = pBuffer;
	mWidth = pWidth;
	mHeight = pHeight;

	/* Creates a new colormap:
	 * Allocate space for a color map that will
	 * represent the desired color region
	 */
	mColorMap = allocate_colormap();
	
	/* Allocate sufficient memory for the blob map */
	mBlobMap = allocate_blobmap(mWidth,mHeight);
	/* Initialize the blobmap */
	reset_borders_blobmap(mBlobMap, mWidth, mHeight);

	/* Allocate sufficient memory for the worst case
	 * that is one new blob fragment at every 2 pixels
	 * (Actually not even that is possible, but it
	 *  does not matter as long as it is big enough)
	 */
	mBlobFragments = allocate_blob_list(mWidth * mHeight / 2);
	/* And much less actual blobs...
	 */
	mBlobs = allocate_blob_list(mWidth * mHeight / 5);

	/* Has to find extremes again
	 */
	mHasFoundExtremes = false;

	/* Reset totals
	 */
	mTotalFragments = mTotal = 0;
}

/* Resets the color map
 */
void
BlobScan::ResetColor()
{
	// Resets the color map
	for (unsigned int i = 0 ; i < COLORMAP_SIZE ; ++i) mColorMap[i] = 0x0000;
}

/* Inverts the color map
 */
void
BlobScan::InvertColor()
{
    invert_colormap(mColorMap);
}

/* Destructor
 */
BlobScan::~BlobScan()
{
	/* Free colormap */
	deallocate_colormap(mColorMap);

	/* Free blobmap */
	deallocate_blobmap(mBlobMap);

	/* Free blob lists */
	deallocate_blob_list(mBlobFragments);
	deallocate_blob_list(mBlobs);
}

/* Perform the scanning
 */
void
BlobScan::Scan()
{
	/* This command scan the blobs giving back a list with the blob
	 * fragments (some were merged and remain there). This fragment
	 * list is kept like that for reference to the blobmap. It is
	 * too time consuming to fix the whole blobmap for every frame
	 * so we just keep like that and map it back later on...
	 */
	mTotalFragments =
		scan_blobs_from_image_to_blobmap(mRgbBuffer, mWidth, mHeight, mBlobMap, mColorMap, mBlobFragments);

	/* If we have some fragments */
	if (mTotalFragments > 0)
	{
		/* Gets the list of real clean blobs from the fragments
		 * The member variable "mergedto" maps back to the original
		 * index in the fragments array. This is used to find
		 * which pixel in the blob map belongs to which blob.
		 */
		mTotal = clean_blob_list(mBlobFragments, mTotalFragments, mBlobs);
	}

	/* Has to find extremes again
	 */
	mHasFoundExtremes = false;
}

/* Gets the color values that are part of the blob from
 * inside the given rectangle
 */
void
BlobScan::GetColorFromRectangle(uint32_t pTopLeftx, uint32_t pTopLefty, uint32_t pBottomRightx, uint32_t pBottomRighty)
{
	/* Sample color of pixels inside the selected rectangle */
	add_from_rectangle_to_colormap(mRgbBuffer,mWidth,mHeight,
		pTopLeftx,pTopLefty,pBottomRightx,pBottomRighty, mColorMap);

	/* Get the average color of the selected region */
	mAverageColor = average_color_of_colormap(mColorMap);
}

/* Inflate color region. The color region is defined by the bits of
 * a RGB cube. Inflating make neighbour bits be included. The resulting
 * effect is that the scanned blob start including colors slightly
 * different from the originally defined. Blobs look cleaner, with less
 * noise.
 */
void
BlobScan::InflateColor()
{
	/* This command inflates the colormap */
	inflate_colormap(mColorMap);

	/* Get the average color of the selected region */
	mAverageColor = average_color_of_colormap(mColorMap);
}

/* Returns the real blob corresponding to the given fragment id
 */
int
BlobScan::RealBlobFromFragment(uint32_t pFragmentId, uint32_t &pReturnId)
{
	// If there are no blobs to search we cannot find
	if (mTotal == 0) return -1;

	// Try to avoid segmentation fault
	if (pFragmentId >= mTotalFragments) return -1;

	// Make sure we have not a merged fragment but a real one
	while (mBlobFragments.blob_list[pFragmentId].mergedto != pFragmentId)
		pFragmentId = mBlobFragments.blob_list[pFragmentId].mergedto;

	// Search the real blob list
	for (uint32_t i = 0 ; i < mTotal ; ++i)
	{
		// Check if the real blob points to the desired
		// fragment
		if (mBlobs.blob_list[i].mergedto == pFragmentId)
		{
			// Ok, it seems we found!
			pReturnId = i;
			return 0;
		}
	}

	// Bad luck... nothing found.
	return -1;
}

/* Returns the blob in the given coordinates
 */
int
BlobScan::BlobAtPosition(uint32_t x, uint32_t y, blob_t &pReturnBlob)
{
	uint32_t fragment_blob, real_blob;
	int return_value = blob_at_blobmap_position(mBlobMap, mWidth, mHeight, mBlobFragments, x, y, &fragment_blob);
	if (return_value < 0)
	{
		// No blob there
		return -1;
	} else {
		return_value = RealBlobFromFragment(fragment_blob,real_blob);
		if (return_value < 0)
		{
			// Perhaps that blob was too small...
			return -1;
		} else {
			pReturnBlob = mBlobs.blob_list[real_blob];
			return 0;
		}
	}
}

/* Returns the blob in the given coordinates
 */
uint32_t
BlobScan::BlobAtPosition(uint32_t x, uint32_t y)
{
	uint32_t fragment_blob, real_blob;
	int return_value = blob_at_blobmap_position(mBlobMap, mWidth, mHeight, mBlobFragments, x, y, &fragment_blob);
	if (return_value < 0)
	{
		// No blob there
		return -1;
	} else {
		return_value = RealBlobFromFragment(fragment_blob,real_blob);
		if (return_value < 0)
		{
			// Perhaps that blob was too small...
			return -1;
		} else {
			return real_blob;
		}
	}
}

/* Find extremes
 */
void
BlobScan::FindExtremes()
{
	unsigned int bigger_mass, smaller_mass, bigger_x, smaller_x, bigger_y, smaller_y;
	blob_t blob;

	// If there are no blobs in the list then we have nothing
	// to do here...
	if (mTotal == 0) return;

	// Let's be optimist.
	mBiggerBlob = mSmallerBlob = mLeftMostBlob = mRightMostBlob = mTopMostBlob = mBottomMostBlob = 0;
	bigger_mass = smaller_mass = mBlobs.blob_list[0].mass;
	smaller_x = mBlobs.blob_list[0].topleftx;
	smaller_y = mBlobs.blob_list[0].toplefty;
	bigger_x = mBlobs.blob_list[0].bottomrightx;
	bigger_y = mBlobs.blob_list[0].bottomrighty;

	// Loop accross the whole list checking for the
	// extreme values
	for (unsigned int i = 1 ; i < mTotal ; ++i)
	{
		blob = mBlobs.blob_list[i];
		if (blob.mass > bigger_mass)
		{
			bigger_mass = blob.mass;
			mBiggerBlob = i;
		}
		if (blob.mass < smaller_mass)
		{
			smaller_mass = blob.mass;
			mSmallerBlob = i;
		}
		if (blob.topleftx < smaller_x)
		{
			smaller_x = blob.topleftx;
			mLeftMostBlob = i;
		}
		if (blob.toplefty < smaller_y)
		{
			smaller_y = blob.toplefty;
			mTopMostBlob = i;
		}
		if (blob.bottomrightx > bigger_x)
		{
			bigger_x = blob.bottomrightx;
			mRightMostBlob = i;
		}
		if (blob.bottomrighty < bigger_y)
		{
			bigger_y = blob.bottomrighty;
			mBottomMostBlob = i;
		}
	}

	// Okay, let's not do it redundantly again
	// by setting this flag
	mHasFoundExtremes = true;
}

/* Returns the blob of bigger mass
 */
int
BlobScan::BiggerBlob(blob_t &pReturnBlob)
{
	// If already know then just return the value
	if (mHasFoundExtremes)
	{
		pReturnBlob = mBlobs.blob_list[mBiggerBlob];
		return 0;
	} else {
		if (mTotal == 0)
		{
			// If there are no blobs in the list then we have nothing
			// to do here...
			return -1;
		} else {
			FindExtremes();
			pReturnBlob = mBlobs.blob_list[mBiggerBlob];
			return 0;
		}
	}
}

/* Returns the blob of smaller mass
 */
int
BlobScan::SmallerBlob(blob_t &pReturnBlob)
{
	// If already know then just return the value
	if (mHasFoundExtremes)
	{
		pReturnBlob = mBlobs.blob_list[mSmallerBlob];
		return 0;
	} else {
		if (mTotal == 0)
		{
			// If there are no blobs in the list then we have nothing
			// to do here...
			return -1;
		} else {
			FindExtremes();
			pReturnBlob = mBlobs.blob_list[mSmallerBlob];
			return 0;
		}
	}
}


/* Returns the blob most to the left
 */
int
BlobScan::LeftMostBlob(blob_t &pReturnBlob)
{
	// If already know then just return the value
	if (mHasFoundExtremes)
	{
		pReturnBlob = mBlobs.blob_list[mLeftMostBlob];
		return 0;
	} else {
		if (mTotal == 0)
		{
			// If there are no blobs in the list then we have nothing
			// to do here...
			return -1;
		} else {
			FindExtremes();
			pReturnBlob = mBlobs.blob_list[mLeftMostBlob];
			return 0;
		}
	}
}


/* Returns the blob most to the right
 */
int
BlobScan::RightMostBlob(blob_t &pReturnBlob)
{
	// If already know then just return the value
	if (mHasFoundExtremes)
	{
		pReturnBlob = mBlobs.blob_list[mRightMostBlob];
		return 0;
	} else {
		if (mTotal == 0)
		{
			// If there are no blobs in the list then we have nothing
			// to do here...
			return -1;
		} else {
			FindExtremes();
			pReturnBlob = mBlobs.blob_list[mRightMostBlob];
			return 0;
		}
	}
}


/* Return the blob most to the top
 */
int
BlobScan::TopMostBlob(blob_t &pReturnBlob)
{
	// If already know then just return the value
	if (mHasFoundExtremes)
	{
		pReturnBlob = mBlobs.blob_list[mTopMostBlob];
		return 0;
	} else {
		if (mTotal == 0)
		{
			// If there are no blobs in the list then we have nothing
			// to do here...
			return -1;
		} else {
			FindExtremes();
			pReturnBlob = mBlobs.blob_list[mTopMostBlob];
			return 0;
		}
	}
}


/* Return the blob most to the bottom
 */
int
BlobScan::BottomMostBlob(blob_t &pReturnBlob)
{
	// If already know then just return the value
	if (mHasFoundExtremes)
	{
		pReturnBlob = mBlobs.blob_list[mBottomMostBlob];
		return 0;
	} else {
		if (mTotal == 0)
		{
			// If there are no blobs in the list then we have nothing
			// to do here...
			return -1;
		} else {
			FindExtremes();
			pReturnBlob = mBlobs.blob_list[mBottomMostBlob];
			return 0;
		}
	}
}


/* Highlights all blobs (actually just makes darker
 * the pixels that do not belong to any blob
 */
void
BlobScan::HighlightAllBlobs(uint8_t *pRgbBuffer)
{
			
	/* Highlight all blobs */
	highlight_all_blobs(mBlobMap, mBlobFragments, pRgbBuffer, mWidth, mHeight);

}

void
BlobScan::HighlightOneBlob(uint8_t *pRgbBuffer, blob_t pBlob)
{
	/* Highlight that one blob
	 * (actually has to pass the corresponding fragment)
	 */
	highlight_one_blob(mBlobMap, pBlob.mergedto, mBlobFragments,
			mAverageColor, mRgbBuffer, mWidth, mHeight);
}

/* Highlights one blob
 */
void
BlobScan::HighlightOneBlob(uint8_t *pRgbBuffer, uint32_t pBlobId)
{
	/* Check if the id is within the limits */
	if (pBlobId >= mTotal) return;
	
	/* Highlight that one blob
	 * (actually has to pass the corresponding fragment)
	 */
	highlight_one_blob(mBlobMap, mBlobs.blob_list[pBlobId].mergedto, mBlobFragments,
			mAverageColor, mRgbBuffer, mWidth, mHeight);
}

/* Returns the blob corresponding to the given index
 */
int
BlobScan::BlobFromId(uint32_t pId, blob_t &pReturnBlob)
{
	// Check limits
	if (pId > mTotal) return -1;
	// Get corresponding blob
	pReturnBlob = mBlobs.blob_list[pId];
	// Return zero on success
	return 0;
}

/* Saves the blob color map pixel list to a file
 */
int
BlobScan::ColorToFile(char *pFilename)
{
	int return_value;
	/* List the pixels inluded in the color map */
	rgb_pixel_list_t pixel_list = from_colormap_to_pixel_list(mColorMap);
	/* Save the list into a file */
	return_value = from_pixel_list_to_file(pixel_list, pFilename);
	/* Free memory occupied by the pixel list */
	deallocate_rgb_pixel_list(pixel_list);
	/* Return the success or fail result */
	return return_value;
}

/* Reads the blob color map pixel list from a file
 */
int
BlobScan::ColorFromFile(char *pFilename)
{
	/* List to store the pixel data */
	rgb_pixel_list_t pixel_list = pixel_list_from_file(pFilename);
	/* Reads the list from a file */
	if (pixel_list.total == 0)
	{
		// Failed to read the file
		return -1;
	} else {
		/* Add the pixels form the list into the color map */
		from_pixel_list_to_colormap(pixel_list, mColorMap);
		/* Free memory occupied by the pixel list */
		deallocate_rgb_pixel_list(pixel_list);
		/* Return the success result */
		return 0;
	}
}

