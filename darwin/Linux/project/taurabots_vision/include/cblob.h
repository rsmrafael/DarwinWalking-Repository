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

#ifndef CBLOB_H
#define CBLOB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define COLORMAP_SIZE 256*256*16
#define HUGE_NUMBER (uint32_t)0xfffffffe;
#define MAX_BLOBS HUGE_NUMBER
#define MIN_BLOB_AREA 300

/* Holds information about a blob */
typedef struct blob_t__ {
	uint32_t topleftx;
	uint32_t toplefty;
	uint32_t bottomrightx;
	uint32_t bottomrighty;
	uint32_t sumx;
	uint32_t sumy;
	uint32_t mass;
	uint32_t mergedto;
} blob_t;

/* Holds a list of blobs */
typedef struct blob_list_t__ {
	blob_t *blob_list;
	uint32_t total;
} blob_list_t;

/* Holds RGB 24BPP data of one color pixel */
typedef struct rgb_pixel_t__ {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_pixel_t;

/* Holds a list of color pixels */
typedef struct rgb_pixel_list_t__ {
	rgb_pixel_t *pixel_list;
	uint32_t total;
} rgb_pixel_list_t;

/* This function inverts the colormap provided */
void invert_colormap(uint16_t *colormap);

/* This function allocates memory for a blob image map */
uint32_t *allocate_blobmap(uint32_t width, uint32_t height);

/* This function allocates memory for a list of pixels */
rgb_pixel_list_t
allocate_rgb_pixel_list(uint32_t total);

/* This function allocates memory for a list of pixels */
blob_list_t
allocate_blob_list(uint32_t total);

/* This function de-allocates memory of a list of pixels */
void
deallocate_rgb_pixel_list(rgb_pixel_list_t pixel_list);

/* This function de-allocates memory of a list of blobs */
void deallocate_blob_list(blob_list_t blob_list);

/* This function resizes the amount of allocated memory
 * of a list of pixels
 */
int
resize_rgb_pixel_list(rgb_pixel_list_t *pixel_list, uint32_t total);

/* Sample pixels inside the given rectangle into a list */
rgb_pixel_list_t
pixel_list_from_rectangle(uint8_t *source_frame, uint32_t width, uint32_t height,
	uint32_t top_left_x, uint32_t top_left_y, uint32_t bottom_right_x, uint32_t bottom_right_y);

/* Sample pixels inside the given rectangle into a map */
void
add_from_rectangle_to_colormap(uint8_t *source_frame, uint32_t width, uint32_t height,
	uint32_t top_left_x, uint32_t top_left_y, uint32_t bottom_right_x, uint32_t bottom_right_y,
	uint16_t *map);

/* Allocate memory for the blob color map */
uint16_t *allocate_colormap();

/* De-allocate memory for the color map */
void deallocate_colormap(uint16_t *colormap);

/* De-allocate memory for the blob map */
void deallocate_blobmap(uint32_t *blobmap);

/* Add pixel to the blob color map */
void add_pixel_to_colormap(rgb_pixel_t pixel, uint16_t *colormap);

/* Remove pixel from the blob color map */
void remove_pixel_from_colormap(rgb_pixel_t pixel, uint16_t *colormap);

/* Tells if a pixel belongs to the blob color map */
uint16_t is_pixel_in_colormap(rgb_pixel_t pixel, uint16_t *colormap);

/* This function adds all RGB pixels from the list to the map
 */
void
from_pixel_list_to_colormap(rgb_pixel_list_t list, uint16_t *colormap);

/* This function adds all map pixels to a new list
 */
rgb_pixel_list_t
from_colormap_to_pixel_list(uint16_t *colormap);

/* Saves RGB pixel list to a file
 */
int from_pixel_list_to_file(rgb_pixel_list_t list, char *filename);

/* Reads a new RGB pixel list from a file
 */
rgb_pixel_list_t
pixel_list_from_file(char *filename);

/* Merge the data of two blobs from a blob_list
 */
void merge_blobs(blob_list_t blob_list, uint32_t merge_this, uint32_t into_this);

/* This function scans the whole image and returns a list of blobs matching the
 * given colormap
 */
uint32_t
scan_blobs_from_image_to_blobmap(uint8_t *image, uint32_t width, uint32_t height,
	uint32_t *blobmap, uint16_t *colormap, blob_list_t return_blob_list);

/* This function returns the real blob linked to the given index
 */
blob_t get_blob_from_blob_list(blob_list_t list, uint32_t i);

/* This function returns a new allocated and clean list with all real blobs
 */
blob_list_t new_clean_blob_list(blob_list_t list, uint32_t uptohere);

/* This function fills the blob list with the clean list with all real blobs
 * and returns the total of blobs found.
 */
uint32_t clean_blob_list(blob_list_t list, uint32_t uptohere, blob_list_t new_list);

/* This function initializes/resets a blob list
 */
void reset_blob_list(blob_list_t list);

/* Resets the blob map borders
 * (only borders have to be initialized)
 */
void reset_borders_blobmap(uint32_t *blobmap, uint32_t width, uint32_t height);

/* Resets the whole blob map
 */
void reset_blobmap(uint32_t *blobmap, uint32_t width, uint32_t height);

/* This function resizes the amount of allocated memory
 * of a list of blobs
 */
int
resize_blob_list(blob_list_t *blob_list, uint32_t total);

/* This function highlight a given blob in the image
 */
void
highlight_one_blob(uint32_t *blobmap, uint32_t blob_id, blob_list_t blob_fragments, rgb_pixel_t highlight_color,
		uint8_t *dest_rgb_image, uint32_t width, uint32_t height);

/* This function highlight all blobs in the image
 */
void
highlight_all_blobs(uint32_t *blobmap, blob_list_t blob_fragments, uint8_t *dest_rgb_image, uint32_t width, uint32_t height);

/* Fills return_id with the blob id associated to the pixel at the given position.
 * Returns -1 if there is no blob at the given position
 */
int blob_at_blobmap_position(uint32_t *blobmap, uint32_t width, uint32_t height, blob_list_t blob_fragments, uint32_t x, uint32_t y, uint32_t *return_id);

/* This function returns the average color of the color map
 */
rgb_pixel_t
average_color_of_colormap(uint16_t *colormap);

/* This function inflates the color map
 */
void
inflate_colormap(uint16_t *colormap);

#ifdef __cplusplus
}
#endif

#endif
