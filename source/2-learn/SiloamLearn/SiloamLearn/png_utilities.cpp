//
//  png_utilities.cpp
//  SiloamLearn
//
//  credit to user niw on GitHubGist 
//
//  Created by Jason  Brewer on 16/08/2017.
//  Copyright © 2017 University of Liverpool. All rights reserved.
//

// TODO: All of this can be folded into image_processor, duh

#include <stdio.h>
#include "png_utilities.hpp"

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;

void read_png_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();
    
    png_infop info = png_create_info_struct(png);
    if(!info) abort();
    
    if(setjmp(png_jmpbuf(png))) abort();
    
    png_init_io(png, fp);
    
    png_read_info(png, info);
    
    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);
    
    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt
    
    if(bit_depth == 16)
        png_set_strip_16(png);
    
    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    
    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    
    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    
    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    
    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);
    
    png_read_update_info(png, info);
    
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }
    
    png_read_image(png, row_pointers);
    
    fclose(fp);
    
    // Clean up
    
    png_destroy_read_struct(&png, &info, NULL);
    png = NULL;
    info = NULL;
    free(row_pointers);
}

void write_png_file(const char *filename) {
    
    FILE *fp = fopen(filename, "wb");
    if(!fp) abort();
    
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();
    
    png_infop info = png_create_info_struct(png);
    if (!info) abort();
    
    if (setjmp(png_jmpbuf(png))) abort();
    
    png_init_io(png, fp);
    
    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
                 png,
                 info,
                 width, height,
                 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT
                 );
    png_write_info(png, info);
    
    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);
    
    png_write_image(png, row_pointers);
    png_write_end(png, NULL);
    
    for(int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    
    // Clean up
    if (png && info)
        png_destroy_write_struct(&png, &info);
    
    fclose(fp);
}

// This needs to generate the point cloud, and all sobel
// operations

PointCloud* process_png_file(PngOperation operation) {
    
    switch(operation) {
        case kPointCloud: {
            PointCloud* temp_point_cloud = new PointCloud[kPngHeight];
            
                for (int y = 0; y < height; y++) {
                    png_bytep row = row_pointers[y];
                    for (int x = 0; x < width; x++) {
                        png_bytep px = &(row[x * 4]);
                        
                        if (px[0] != 0) {
                            temp_point_cloud[y][x]->x = x;
                            temp_point_cloud[y][x]->y = y;
                            temp_point_cloud[y][x]->z = px[0];
                        } else {
                            temp_point_cloud[y][x]->x = -1;
                            temp_point_cloud[y][x]->y = -1;
                            temp_point_cloud[y][x]->z = -1;
                        }
                        
                        //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
                    }
                }
            return temp_point_cloud;
            break;
        }
        case kSegment: {
            // Remove the floor plane from the image using estimates
            // from the N images to leave behind only objects of interest
            //
            // Only objects that are above a certain footprint size are
            // considered for classification to help combat noisy data
            // and parts of the images that have been removed because their
            // normals are similar to the detected floor plane
            //
            // A hash table is used because of it's potentially fast
            // lookup complexity O(1)
            
            std::unordered_map<RgbValue, Coordinate> normal_values;
            
            for (int y = kNormalYStart - 1; y >= kNormalYEnd - 1; y--) {
                png_bytep row = row_pointers[y];
                for (int x = 0; x < width; x++) {
                    png_bytep px = &(row[x * 4]);
                    
                    // Collect the rgb representations of the normals
                    normal_values.insert( { {px[0], px[1], px[2]}, {x, y} } );
                    
                    // Remove the portion of the image we have just scanned
                    // (it is assumed to be the floor)
                    px[0] = 0;
                    px[1] = 0;
                    px[2] = 0;
                    
                    //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
                }
            }
            
            // Now iterate through the rest of the image and if the current rgb
            // representation of the normal
            
            for (int y = 0; y < kNormalYEnd - 1; y++) {
                png_bytep row = row_pointers[y];
                for (int x = 0; x < width; x++) {
                    png_bytep px = &(row[x * 4]);
                    
                    auto search = normal_values.find( {px[0], px[1], px[2]} );
                    if (search != normal_values.end()) {
                        // We have found a pixel which is in the range of normal values
                        // so make it all black RGB(0, 0, 0)
                        px[0] = 0;
                        px[1] = 0;
                        px[2] = 0;
                    }
                    
                }
            }
            
            break;
        }
        case kSobelX: {
            // Implement sobel in X direction
            break;
        }
        case kSobelY: {
            // Implement sobel in Y direction
            break;
        }
        case kSobelMagnitude: {
            // Find magnitude of gradient (full sobel?)
            break;
        }
    }
    return nullptr;
}

// Start from the bottom left of the PNG and traverse upwards.
// We are likely to find a non-black pixel lower down the image
// because of the floor region

bool dead_png() {
    for (int y = height - 1; y >= 0; y--) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            // Check if any pixels are non-black
            
            // Definitely not the most efficient way
            // to do it, but there we are
            
            if (px[0] != 0 || px[1] != 0 || px[2] != 0) {
                return false;
            }
        }
    }
    return true;
}
