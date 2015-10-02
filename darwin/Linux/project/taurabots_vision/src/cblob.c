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

#include "cblob.h"
#include <stdlib.h>
#include <stdio.h>

#define COLORMAP_BYTE(m,r,g,b) *(m + (r/16)*256*256 + g*256 + b)
#define COLORMAP_SHIFT(r) (0x0001 << (r%16))

/* This function allocates memory for a list of pixels */
rgb_pixel_list_t
allocate_rgb_pixel_list(uint32_t total)
{
	rgb_pixel_list_t pixel_list;
	pixel_list.pixel_list = malloc(total * sizeof(rgb_pixel_t));
	if (pixel_list.pixel_list == NULL)
	{
		fprintf(stderr, "Could not allocate memory for pixel list\n");
		exit(1);
	}
	pixel_list.total = total;
	return pixel_list;
}

/* This function allocates memory for a list of pixels */
blob_list_t
allocate_blob_list(uint32_t total)
{
	blob_list_t blob_list;
	blob_list.blob_list = malloc(total * sizeof(blob_t));
	if (blob_list.blob_list == NULL)
	{
		fprintf(stderr, "Could not allocate memory for blob list\n");
		exit(1);
	}
	blob_list.total = total;
	return blob_list;
}

/* This function initializes/resets a blob list
 */
void reset_blob_list(blob_list_t list)
{
	uint32_t i;
	for (i = 0 ; i < list.total ; ++i)
	{
		list.blob_list[i].mergedto = i; //not merged to anything
		list.blob_list[i].topleftx = UINT16_MAX; // anything is less than this
		list.blob_list[i].toplefty = UINT16_MAX; // anything is less than this
		list.blob_list[i].bottomrightx = 0; //anything is more than this
		list.blob_list[i].bottomrighty = 0; //anything is more than this
		list.blob_list[i].mass += 0;
		list.blob_list[i].sumx += 0;
		list.blob_list[i].sumy += 0;
	}
}

/* This function resizes the amount of allocated memory
 * of a list of pixels
 */
int
resize_rgb_pixel_list(rgb_pixel_list_t *pixel_list, uint32_t total)
{
	(*pixel_list).pixel_list = realloc((*pixel_list).pixel_list, total * sizeof(rgb_pixel_t));
	if ((*pixel_list).pixel_list == NULL)
	{
		fprintf(stderr, "Could not reallocate the memory for pixel list\n");
		return -1;
	}
	(*pixel_list).total = total;
	return 0;
}

/* Sample pixels inside the given rectangle */
rgb_pixel_list_t
pixel_list_from_rectangle(uint8_t *source_frame, uint32_t width, uint32_t height,
	uint32_t top_left_x, uint32_t top_left_y, uint32_t bottom_right_x, uint32_t bottom_right_y)
{
	rgb_pixel_list_t sampled_pixels; // list of sampled pixels
	uint32_t x, y; // coordinates in the image
	uint32_t i = 0; // index latest sampled pixel
	uint32_t j = 0; // index for searching the pixels sampled so far

	/* Allocates memory enough for holding the pixels inside the rectangle */
	sampled_pixels = allocate_rgb_pixel_list((bottom_right_x - top_left_x + 1) * (bottom_right_y - top_left_y + 1));

	/* Sample the pixels inside the rectangle */
	for (x = top_left_x; x <= bottom_right_x ; ++x)
	{
		for (y = top_left_y; y <= bottom_right_y ; ++y)
		{
			/* Get the pixel */
			sampled_pixels.pixel_list[i] = *((rgb_pixel_t *)(source_frame + (x*3) + ((y*3) * width)));
			/* Check for duplicated pixels */
			for (j = 0 ; j < i ; ++j)
			{
				if ((sampled_pixels.pixel_list[j].r == sampled_pixels.pixel_list[i].r) &&
				    (sampled_pixels.pixel_list[j].g == sampled_pixels.pixel_list[i].g) &&
				    (sampled_pixels.pixel_list[j].b == sampled_pixels.pixel_list[i].b))
				{
					break;
				}
			}
			if (j == i) i++; // Only increments index if not repeated
		}
	}
	if (i != sampled_pixels.total)
	{
		if (resize_rgb_pixel_list(&sampled_pixels, i) < 0)
		{
			fprintf(stderr,"There has been a problem with resizing the pixel list\n");
		}
	}

	//Returns the list
	return sampled_pixels;
}

/* Sample pixels inside the given rectangle into the map */
void
add_from_rectangle_to_colormap(uint8_t *source_frame, uint32_t width, uint32_t height, uint32_t top_left_x, uint32_t top_left_y, uint32_t bottom_right_x, uint32_t bottom_right_y, uint16_t *colormap)
{
	uint32_t x, y; // coordinates in the image

	/* Sample the pixels inside the rectangle */
	for (x = top_left_x; x <= bottom_right_x ; ++x)
	{
		for (y = top_left_y; y <= bottom_right_y ; ++y)
		{
			/* Adds the pixel to the map */
			add_pixel_to_colormap(*((rgb_pixel_t *)(source_frame + (x*3) + ((y*3) * width))), colormap);
			/* Check for duplicated pixels */
		}
	}
}

/* Inverts the blob color map */
void invert_colormap(uint16_t *colormap)
{
	uint32_t i;
	for (i = 0 ; i < COLORMAP_SIZE ; ++i) colormap[i] = ~colormap[i];
}

/* Allocate memory for the blob color map */
uint16_t *allocate_colormap()
{
	uint16_t *colormap;
	uint32_t i;
	colormap = malloc(COLORMAP_SIZE * sizeof(uint16_t));
	for (i = 0 ; i < COLORMAP_SIZE ; ++i) colormap[i] = 0x0000;
	return colormap;
}

/* De-allocate memory for the color map */
void deallocate_colormap(uint16_t *colormap)
{
	free(colormap);
}

/* De-allocate memory for the blob map */
void deallocate_blobmap(uint32_t *blobmap)
{
	free(blobmap);
}

/* Add pixel to the blob color map */
void add_pixel_to_colormap(rgb_pixel_t pixel, uint16_t *colormap)
{
	COLORMAP_BYTE(colormap, pixel.r, pixel.g, pixel.b) |= COLORMAP_SHIFT(pixel.r);
}

/* Remove pixel from the blob color map */
void remove_pixel_from_colormap(rgb_pixel_t pixel, uint16_t *colormap)
{
	COLORMAP_BYTE(colormap, pixel.r, pixel.g, pixel.b) &= ~(COLORMAP_SHIFT(pixel.r));
}

/* Tells if a pixel belongs to the blob color map */
uint16_t is_pixel_in_colormap(rgb_pixel_t pixel, uint16_t *colormap)
{
	if ((COLORMAP_BYTE(colormap,pixel.r,pixel.g,pixel.b) & COLORMAP_SHIFT(pixel.r)) != 0x0000)
	{
		return 1;
	} else {
		return 0;
	}
}

/* This function de-allocates memory of a list of pixels */
void
deallocate_rgb_pixel_list(rgb_pixel_list_t pixel_list)
{
	free(pixel_list.pixel_list);
}

/* This function de-allocates memory of a list of blobs */
void
deallocate_blob_list(blob_list_t blob_list)
{
	free(blob_list.blob_list);
}

/* This function adds all RGB pixels from the list to the map
 */
void
from_pixel_list_to_colormap(rgb_pixel_list_t list, uint16_t *colormap)
{
	uint32_t i;
	for (i = 0 ; i < list.total ; ++i)
	{
		add_pixel_to_colormap(list.pixel_list[i],colormap);
	}
}

/* This function adds all map pixels to a new list
 */
rgb_pixel_list_t
from_colormap_to_pixel_list(uint16_t *colormap)
{
	uint16_t r, g, b;
	uint32_t total = 0;
	rgb_pixel_t pixel;
	rgb_pixel_list_t list;

	// Checks how many pixels are in the map
	for (r = 0 ; r < 256 ; ++r)
	{
		for (g = 0 ; g < 256 ; ++g)
		{
			for (b = 0 ; b < 256 ; ++b)
			{
				pixel.r = (uint8_t)r;
				pixel.g = (uint8_t)g;
				pixel.b = (uint8_t)b;
				if (is_pixel_in_colormap(pixel, colormap))
				{
					// Counts
					total++;
				}
			}
		}
	}

	// Allocates memory for the pixel list
	list = allocate_rgb_pixel_list(total);

	// Adds all pixels to the list
	for (r = 0 ; r < 256 ; ++r)
	{
		for (g = 0 ; g < 256 ; ++g)
		{
			for (b = 0 ; b < 256 ; ++b)
			{
				pixel.r = (uint8_t)r;
				pixel.g = (uint8_t)g;
				pixel.b = (uint8_t)b;
				if (is_pixel_in_colormap(pixel, colormap))
				{
					total--;
					list.pixel_list[total] = pixel;
				}
			}
		}
	}

	// Return the new list
	return list;
}

/* Saves RGB pixel list to a file
 */
int
from_pixel_list_to_file(rgb_pixel_list_t list, char *filename)
{
	FILE *output_file;
	uint32_t i;
	output_file = fopen(filename, "w");

	if (output_file != NULL)
	{
		for (i = 0 ; i < list.total ; ++i)
			fprintf(output_file,"%u,%u,%u\n",list.pixel_list[i].r,list.pixel_list[i].g,list.pixel_list[i].b);
	} else {
		fprintf(stderr,"Could not open file %s for writing a pixel list.\n",filename);
		return -1;
	}

	// Close the file and return 0 on success
	fclose(output_file);
	return 0;
}

/* Reads a new RGB pixel list from a file
 */
rgb_pixel_list_t
pixel_list_from_file(char *filename)
{
	FILE *input_file;
	uint8_t r, g, b;
	uint32_t i;
	rgb_pixel_list_t list;
	list.total = 0;
	list.pixel_list = NULL;
	input_file = fopen(filename, "r");

	if (input_file != NULL)
	{
		// First count how many pixels to know how much memory
		// should be allocated
		list.total = 0;
		while(fscanf(input_file,"%hhu,%hhu,%hhu\n",&r,&g,&b) != EOF)
		{
			list.total++;
		}

		// Allocate the memory
		list = allocate_rgb_pixel_list(list.total);

		// Then read again all pixels to the list
		rewind(input_file);
		for (i = 0 ; i < list.total ; ++i)
		{
			if (fscanf(input_file,"%hhu,%hhu,%hhu\n",
				&list.pixel_list[i].r,
				&list.pixel_list[i].g,
				&list.pixel_list[i].b) == 0)
            {
                fprintf(stderr,"File %s seems to be corrupted.",filename);
            }
		}
	} else {
		fprintf(stderr,"Could not open file %s for reading a pixel list.\n",filename);
		return list;
	}

	// Close the file and return the new list on success
	fclose(input_file);
	return list;
}

/* This function allocates memory for a blob image map */
uint32_t *allocate_blobmap(uint32_t width, uint32_t height)
{
	return (uint32_t *)malloc(width*height*sizeof(uint32_t));
}

// Merge the data of two blobs from a blob_list
void merge_blobs(blob_list_t blob_list, uint32_t merge_this, uint32_t into_this)
{
	while (blob_list.blob_list[merge_this].mergedto != merge_this) merge_this = blob_list.blob_list[merge_this].mergedto;
	while (blob_list.blob_list[into_this].mergedto != into_this) into_this = blob_list.blob_list[into_this].mergedto;
	if (merge_this == into_this) return;
	
	// Mark the merged blob
	blob_list.blob_list[merge_this].mergedto = into_this;

	// Find left border
	if (blob_list.blob_list[merge_this].topleftx < blob_list.blob_list[into_this].topleftx)
	{
		blob_list.blob_list[into_this].topleftx = blob_list.blob_list[merge_this].topleftx;
	}
	// Find top border
	if (blob_list.blob_list[merge_this].toplefty < blob_list.blob_list[into_this].toplefty)
	{
		blob_list.blob_list[into_this].toplefty = blob_list.blob_list[merge_this].toplefty;
	}
	// Find right border
	if (blob_list.blob_list[merge_this].bottomrightx > blob_list.blob_list[into_this].bottomrightx)
	{
		blob_list.blob_list[into_this].bottomrightx = blob_list.blob_list[merge_this].bottomrightx;
	}
	// Find bottom border
	if (blob_list.blob_list[merge_this].bottomrighty > blob_list.blob_list[into_this].bottomrighty)
	{
		blob_list.blob_list[into_this].bottomrighty = blob_list.blob_list[merge_this].bottomrighty;
	}
	// Merge the mass
	blob_list.blob_list[into_this].mass += blob_list.blob_list[merge_this].mass;
	// Merge center of gravity
	blob_list.blob_list[into_this].sumx += blob_list.blob_list[merge_this].sumx;
	blob_list.blob_list[into_this].sumy += blob_list.blob_list[merge_this].sumy;
}

/* Resets the blob map borders
 * (only borders have to be initialized)
 */
void
reset_borders_blobmap(uint32_t *blobmap, uint32_t width, uint32_t height)
{
	uint32_t i;
	uint32_t offset;
	// reset topmost and bottommost rows
	offset = (uint32_t)(height-1) * (uint32_t)width;
	for (i = 0 ; i < width ; ++i)
	{
		*(blobmap + i) = 0;
		*(blobmap + i + offset) = 0;
	}
	// reset leftmost and rightmmost columns
	offset = width-1;
	for (i = 0 ; i < height ; ++i)
	{
		*(blobmap + 0 + i*width) = 0;
		*(blobmap + offset + i*width) = 0;
	}
}

/* Resets the whole blob map
 */
void
reset_blobmap(uint32_t *blobmap, uint32_t width, uint32_t height)
{
	uint32_t i;
	for (i = 0 ; i < width*height ; ++i)
		blobmap[i] = 0;
}

/* This function scans the whole image and returns a list of blobs matching the
 * given colormap
 */
uint32_t
scan_blobs_from_image_to_blobmap(uint8_t *image, uint32_t width, uint32_t height,
	uint32_t *blobmap, uint16_t *colormap, blob_list_t return_blob_list)
{
	uint32_t i, j, offset, n1, n2, n3, n4, blob, total_blobs;

	total_blobs = 0;

	for (j = 1 ; j < height ; ++j)
	{
		for (i = 1 ; i < width-1 ; ++i)
		{
			// Test if the pixel at i,j belongs to the colormap
			if (is_pixel_in_colormap(*((rgb_pixel_t *)(image + (i*3) + ((j*3) * width))), colormap))
			{
				// Yes, then let's check if any previous scanned pixel
				// of the 8-neighborhood also belongs to the colormap
				//
				// (check only previously visited neighbors,
				//  marked n1,n2,n3,n4 below)
				//
				//     n1 n2 n3
				//     n4    n5
				//     n6 n7 n8
				//

				// previous line
				offset = (j-1) * width;
				n1 = *(blobmap + i-1 + offset);
				n2 = *(blobmap + i   + offset);
				n3 = *(blobmap + i+1 + offset);
				// current line
				offset += width;
				n4 = *(blobmap + i-1 + offset);

				// printf("Scan found one pixel at %d,%d. ", i, j);
				// printf("The neighbours are %d,%d,%d,%d\n", n1, n2, n3, n4);

				// Because of previous scan, and only if n2
				// does not belong to any blob then either
				// n1 and n3 or n4 and n3 may belong to
				// different blobs.
				//
				// The current pixel merges the two blobs into
				// one
				if (n1 != 0)
				{
					blob = n1-1;
					if (n3 != 0 && n1 != n3)
					{
						// merge corresponding blobs of n1 and n3
						merge_blobs(return_blob_list,n3-1,n1-1);
					}
					goto gotprevious;
				} else {
					if (n3 != 0)
					{
						blob = n3-1;
						if (n4 != 0 && n4 != n3)
						{
							// merge corresponding blobs of n3 and n4
							merge_blobs(return_blob_list,n4-1,n3-1);
						}
						//finding this is enough
						goto gotprevious;
					}
					if (n2 != 0)
					{
						blob = n2-1;
						// found enough
						goto gotprevious;
					}
					if (n4 != 0)
					{
						blob = n4-1;
						// found enough
						goto gotprevious;
					}
				}

				// No previous blob, so let's generate a new one
				// printf("New blob fragment at %d,%d\n", i, j);
				blob = total_blobs;
				total_blobs++; //counts one more
				if (total_blobs >= return_blob_list.total)
				{
					fprintf(stderr,
						"Scanning of blobs could not be completed. Blob list of %d is too small.\n",
						return_blob_list.total);
					return total_blobs-1;
				}
				return_blob_list.blob_list[blob].mergedto = blob; //merged to itself
				return_blob_list.blob_list[blob].topleftx = i; //i is leftmost
				return_blob_list.blob_list[blob].toplefty = j; //j is topmost
				return_blob_list.blob_list[blob].bottomrightx = i; //i is also rightmost
				return_blob_list.blob_list[blob].bottomrighty = j; //j is also bottommost
				return_blob_list.blob_list[blob].sumx = i; //only i for now...
				return_blob_list.blob_list[blob].sumy = j; //only j for now...
				return_blob_list.blob_list[blob].mass = 1; //only one pixel
				// Mark the blob id into the blobmap
				*(blobmap + i + offset) = blob + 1;
				//printf("Blob fragment at %d,%d is of id %d.\n",i,j,*(blobmap + i + offset));
				continue;

				gotprevious:
				// Find the real blob
				while (return_blob_list.blob_list[blob].mergedto != blob) blob = return_blob_list.blob_list[blob].mergedto;
				// We found previous blob, so let's update the info
				if (i < return_blob_list.blob_list[blob].topleftx)
					return_blob_list.blob_list[blob].topleftx = i; //if i is new leftmost
				if (j < return_blob_list.blob_list[blob].toplefty)
					return_blob_list.blob_list[blob].toplefty = j; //if j is new topmost
				if (i > return_blob_list.blob_list[blob].bottomrightx)
					return_blob_list.blob_list[blob].bottomrightx = i; //if i is new rightmost
				if (j > return_blob_list.blob_list[blob].bottomrighty)
					return_blob_list.blob_list[blob].bottomrighty = j; //if j is new bottommost
				return_blob_list.blob_list[blob].sumx += i; //sum the x coordinate
				return_blob_list.blob_list[blob].sumy += j; //sum the y coordinate
				return_blob_list.blob_list[blob].mass++; //adds one pixel to the mass
				// Mark the blob id into the blobmap
				*(blobmap + i + offset) = blob + 1;
			} else {
				// This pixel does not belong to the blob
				*(blobmap + i + (j * width)) = 0;
			}
		}
	}

	return total_blobs;
}

/* This function returns the real blob linked to the given index
 */
blob_t get_blob_from_blob_list(blob_list_t list, uint32_t i)
{
	if (list.blob_list[list.blob_list[i].mergedto].mergedto != list.blob_list[i].mergedto)
		return get_blob_from_blob_list(list, list.blob_list[i].mergedto);
	else
		return list.blob_list[list.blob_list[i].mergedto];
}

/* This function fills the blob list with the clean list with all real blobs
 * and returns the total of blobs found.
 */
uint32_t clean_blob_list(blob_list_t fragment_list, uint32_t uptohere, blob_list_t new_list)
{
	uint32_t i, total = 0;

	// Copy only the real blobs to the new list
	for (i = 0 ; i < uptohere ; ++i)
	{
		if (fragment_list.blob_list[i].mergedto == i && fragment_list.blob_list[i].mass >= MIN_BLOB_AREA)
		{
			new_list.blob_list[total] = fragment_list.blob_list[i];
			// Careful! In the new list the member mergedto
			// actually maps the index from the new list
			// to the old one. This is a different interpretation
			// than when the list is scanned
			new_list.blob_list[total].mergedto = i;
			total++;
		}
	}

	//return the total amount of real blobs found
	return total;
}

/* This function returns a clean list with all real blobs
 */
blob_list_t new_clean_blob_list(blob_list_t list, uint32_t uptohere)
{
	uint32_t i, total;
	blob_list_t new_list;

	// Count how many real blobs
	total = 0;
	for (i = 0 ; i < uptohere ; ++i)
	{
		if (list.blob_list[i].mergedto == i && list.blob_list[i].mass >= MIN_BLOB_AREA)
			total++;
	}

	// Create a new list
	new_list = allocate_blob_list(total);

	// Copy only the real blobs to the new list
	for (i = 0 ; i < uptohere ; ++i)
	{
		if (list.blob_list[i].mergedto == i && list.blob_list[i].mass >= MIN_BLOB_AREA)
		{
			total--;
			new_list.blob_list[total] = list.blob_list[i];
			// Careful! In the new list the member mergedto
			// actually maps the index from the new list
			// to the old one. This is a different interpretation
			// than when the list is scanned
			new_list.blob_list[total].mergedto = i;
		}
	}

	//return the new blob list
	return new_list;
}

/* This function resizes the amount of allocated memory
 * of a list of blobs
 */
int
resize_blob_list(blob_list_t *blob_list, uint32_t total)
{
	(*blob_list).blob_list = realloc((*blob_list).blob_list, total * sizeof(blob_t));
	if ((*blob_list).blob_list == NULL)
	{
		fprintf(stderr, "Could not reallocate the memory for blob list\n");
		return -1;
	}
	(*blob_list).total = total;
	return 0;
}

/* This function highlight all blobs in the image
 */
void
highlight_all_blobs(uint32_t *blobmap, blob_list_t blob_fragments, uint8_t *dest_rgb_image, uint32_t width, uint32_t height)
{
	uint32_t i,j,id,offset,rgb_offset;

	// Scan the whole image
	for (j = 0 ; j < height ; ++j)
	{
		offset = j*width;
		rgb_offset = 3*offset;
		for (i = 0 ; i < width ; ++i)
		{
			id = *(blobmap + i + offset);
			// If the pixel does not belong to any blob
			if (id == 0)
			{
				// Decrease brightness of the pixel
				dest_rgb_image[rgb_offset + 3*i    ] >>= 2;
				dest_rgb_image[rgb_offset + 3*i + 1] >>= 2;
				dest_rgb_image[rgb_offset + 3*i + 2] >>= 2;
			}
		}
	}
}

/* This function highlight a given blob in the image
 */
void
highlight_one_blob(uint32_t *blobmap, uint32_t blob_id, blob_list_t blob_fragments, rgb_pixel_t highlight_color,
		uint8_t *dest_rgb_image, uint32_t width, uint32_t height)
{
	uint32_t i,j,id,offset,rgb_offset;

	// Make sure we are looking for the real blob
	while (blob_fragments.blob_list[blob_id].mergedto != blob_id) blob_id = blob_fragments.blob_list[blob_id].mergedto;

	// Scan only the bounding box within which the blob should be
	for (j = blob_fragments.blob_list[blob_id].toplefty ; j < blob_fragments.blob_list[blob_id].bottomrighty ; ++j)
	{
		offset = j*width;
		rgb_offset = 3*offset;
		for (i = blob_fragments.blob_list[blob_id].topleftx ; i < blob_fragments.blob_list[blob_id].bottomrightx ; ++i)
		{
			id = *(blobmap + i + offset);
			// If there really is a blob in there
			if (id != 0)
			{
				id--;
				// Make sure we are comparing real blobs
				while (blob_fragments.blob_list[id].mergedto != id) id = blob_fragments.blob_list[id].mergedto;
				if (id == blob_id)
				{
					// Found it here, so let's highlight
					(*(rgb_pixel_t *)(dest_rgb_image + rgb_offset + 3*i)) = highlight_color;
				}
			}
		}
	}
}

/* Fills return_id with the blob id associated to the pixel at the given position.
 * Returns -1 if there is no blob at the given position
 */
int blob_at_blobmap_position(uint32_t *blobmap, uint32_t width, uint32_t height, blob_list_t blob_fragments, uint32_t x, uint32_t y, uint32_t *return_id)
{
	uint32_t id;
	// Get the id in the position x,y in the map
	id = blobmap[x + y*width];
	// do we have a blob?
	if (id != 0)
	{
		// This is the id in the blob list
		id--;
		// Make sure we have a real blob
		while (blob_fragments.blob_list[id].mergedto != id) id = blob_fragments.blob_list[id].mergedto;
		// fill the return id
		*return_id = id;
	} else {
		// No blob there, return -1
		return -1;
	}
	// Return 0 if a blob was found
	return 0;
}


/* This function returns the average color of the color map
 */
rgb_pixel_t
average_color_of_colormap(uint16_t *colormap)
{
	uint16_t r, g, b;
	uint32_t sum_r = 0, sum_g = 0, sum_b = 0, total = 0;
	rgb_pixel_t pixel;

	// Find the RGB combinations that belong to the map
	for (r = 0 ; r < 256 ; ++r)
	{
		for (g = 0 ; g < 256 ; ++g)
		{
			for (b = 0 ; b < 256 ; ++b)
			{
				pixel.r = (uint8_t)r;
				pixel.g = (uint8_t)g;
				pixel.b = (uint8_t)b;
				if (is_pixel_in_colormap(pixel, colormap))
				{
					// Sums
					sum_r += r;
					sum_g += g;
					sum_b += b;
					// Counts
					total++;
				}
			}
		}
	}

	// Calculates the averaged color
	pixel.r = (uint8_t)(sum_r / total);
	pixel.g = (uint8_t)(sum_g / total);
	pixel.b = (uint8_t)(sum_b / total);

	// Return the averaged color
	return pixel;
}

/* This function inflates the color map
 */
void
inflate_colormap(uint16_t *colormap)
{
	uint16_t r, g, b, *newcolormap;
	uint32_t i;
	rgb_pixel_t pixel;

	// Allocate memory for a new colormap
	newcolormap = allocate_colormap();

	// Find the RGB combinations that belong to the map
	for (r = 0 ; r < 256 ; ++r)
	{
		for (g = 0 ; g < 256 ; ++g)
		{
			for (b = 0 ; b < 256 ; ++b)
			{
				pixel.r = (uint8_t)r;
				pixel.g = (uint8_t)g;
				pixel.b = (uint8_t)b;
				if (is_pixel_in_colormap(pixel, colormap))
				{
					pixel.r = (uint8_t)r;
					pixel.g = (uint8_t)g;
					pixel.b = (uint8_t)b;
					add_pixel_to_colormap(pixel, newcolormap);
					pixel.r = (uint8_t)r-1;
					pixel.g = (uint8_t)g;
					pixel.b = (uint8_t)b;
					add_pixel_to_colormap(pixel, newcolormap);
					pixel.r = (uint8_t)r+1;
					pixel.g = (uint8_t)g;
					pixel.b = (uint8_t)b;
					add_pixel_to_colormap(pixel, newcolormap);
					pixel.r = (uint8_t)r;
					pixel.g = (uint8_t)g-1;
					pixel.b = (uint8_t)b;
					add_pixel_to_colormap(pixel, newcolormap);
					pixel.r = (uint8_t)r;
					pixel.g = (uint8_t)g+1;
					pixel.b = (uint8_t)b;
					add_pixel_to_colormap(pixel, newcolormap);
					pixel.r = (uint8_t)r;
					pixel.g = (uint8_t)g;
					pixel.b = (uint8_t)b-1;
					add_pixel_to_colormap(pixel, newcolormap);
					pixel.r = (uint8_t)r;
					pixel.g = (uint8_t)g;
					pixel.b = (uint8_t)b+1;
					add_pixel_to_colormap(pixel, newcolormap);
				}
			}
		}
	}

	// Copy
	for (i = 0 ; i < COLORMAP_SIZE ; ++i) colormap[i] = newcolormap[i];

	// Deallocates new color map
	deallocate_colormap(newcolormap);
}

