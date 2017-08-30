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
    
    if (row_pointers == NULL) {
        printf("Why is row_pointers NULL? Sort it!");
    }
    
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

// Finds clusters in the initial D_seg image since there
// could be artifacts of the background in the

void find_clusters(double threshold) {

    
    // Using a map here and hoping LLVM optimises
    // insertion and finding because it will already be
    // sorted in numerical ascending order...
    
    std::map<int, std::vector<double>> euclidean_distances;
    std::vector<std::vector<Coordinate>> clusters;
    double this_distance = 0.0f;
    int num_clusters = 0; // is set to clusters.size() later on
    png_bytep row = NULL;
    png_bytep px = NULL;
    png_bytep row_ = NULL;
    png_bytep px_ = NULL;
    

    for (int y = 0; y < height; y++) {
        row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            px = &(row[x * 4]);
            
            // Only check non black pixels
            
            if ( !(px[0] == 0 && px[1] == 0 && px[2] == 0) ) {
                
                // And find the euclidean distance of this non black pixel
                // from all the other non-black pixels
                //
                // Maybe the most naive alogorithm ever written
                // At least it's thorough
                
                for (int y_ = 0; y_ < height; y_++) {
                    row_ = row_pointers[y_];
                    for (int x_ = 0; x_ < width; x_++) {
                    px_ = &(row_[x_ * 4]);
                    
                        if ( !(px_[0] == 0 && px_[1] == 0 && px_[2] == 0) ) {
                            // Simple pythagoras for (x,y) and (x_,y_)
                            
                            this_distance = sqrt( pow(x - x_, 2)
                                                 + pow(y - y_, 2) );
                            
                            // Remember this y+1 factor! It's very important
                            // later on. Without y+1 the first row of pixel
                            // distances would all have the key '0'
                            
                            euclidean_distances[int((y+1)*x)].push_back(this_distance);
                            
                        }
                    }
                }
            }
        }
    }
    
    // Iterate through euclidean_distances and find all points that are within
    // distance kEuclideanClusterSizeThreshold (see .hpp file). If they are, create
    // a new cluster or add to the same cluster. If not, of course don't add them to
    // the cluster!
    
    for (std::map<int, std::vector<double>>::iterator it = euclidean_distances.begin();
         it != euclidean_distances.end();
         ++it) {
        it->first;
        it->second;
    }
    
    
    
}

// This needs to generate the point cloud, and all sobel
// operations

void process_png_file(PngOperation operation, PngProcessResultData& result_data) {
    
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
            
            result_data.point_cloud = temp_point_cloud;
            break;
        }
        case kFindFloor: {
            // Remove the floor plane from the image using estimates
            // from the N images to leave behind only objects of interest
            //
            // Only objects that are above a certain footprint size are
            // considered for classification to help combat noisy data
            // and parts of the images that have been removed because their
            // normals are similar to the detected floor plane
            //
            // A hash table is used because of it's potentially fast
            // lookup complexity O(1). Given that we must analyse potentially
            // 1000's of images this is important
            
            std::unordered_map<RgbValue, Coordinate> normal_values;
            result_data.floor_estimate = new std::vector<Coordinate>;
            
            for (int y = kNormalYStart - 1; y < kNormalYEnd; y++) {
                png_bytep row = row_pointers[y];
                for (int x = 0; x < width; x++) {
                    png_bytep px = &(row[x * 4]);
                    
                    printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
                    
                    // Collect the rgb representations of the normals unless the pixel is black
                    if ( !(px[0] == 0 && px[1] == 0 && px[2] == 0) ) {
                        normal_values.insert( { {px[0], px[1], px[2]} , {x, y} } );
                        
                        // Blacken the pixel we have just scanned
                        // (it is assumed to be the floor)
                        px[0] = 0;
                        px[1] = 0;
                        px[2] = 0;
                        
                        // Save the floor pixels to the floor_estimate vector
                        result_data.floor_estimate->push_back( {x, y} );
                    }
                }
            }
            
            // Now iterate through the rest of the image and if the current rgb
            // representation of the normal is in the normal hash table, once
            // again blacken the pixel
            
            for (int y = 0; y < kNormalYStart - 1; y++) {
                png_bytep row = row_pointers[y];
                for (int x = 0; x < width; x++) {
                    png_bytep px = &(row[x * 4]);
                    if ( !(px[0] == 0 && px[1] == 0 && px[2] == 0) ) {
                        auto search = normal_values.find( {px[0], px[1], px[2]} );
                        if (search != normal_values.end()) {
                            // We have found a pixel which is in the range of normal values
                            // so make it all black RGB(0, 0, 0)
                            px[0] = 0;
                            px[1] = 0;
                            px[2] = 0;
                            
                            // Save the floor pixels to the floor_estimate vector
                            result_data.floor_estimate->push_back( {x, y} );
                        }
                    }
                }
            }
            break;
        }
        case kSegment: {
            // Remove the floor from a given image using the floor estimation
            // found in the kFindFloor step
            
            for (auto const& coord: *result_data.floor_estimate) {
                png_bytep row = row_pointers[coord.y];
                png_bytep px = &(row[coord.x * 4]);
                
                px[0] = 0;
                px[1] = 0;
                px[2] = 0;
            }
            
            // Using the same method as in kCluster, find clusters
            // below a certain threshold and blacken them - they are
            // assumed to be part of the floor plane
            
            find_clusters(kSmallClusterThreshold);
            
            break;
        }
        case kCluster: {
            // Find clusters via the method in Fehr et al - simple
            // Euclidean distance (see page 89 of the paper)
            //
            // Clusters below are certain threshold are removed from
            // the image - they are effectively considered as noise since
            // it is likely they are part of the floor plane or extraneous
            // objects not relevant to any of the four categories under test
            
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
