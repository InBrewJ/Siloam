/*
 * A simple libpng example program
 * http://zarb.org/~gc/html/libpng.html
 *
 * Modified by Yoshimasa Niwa to make it much simpler
 * and support all defined color_type.
 *
 * Extended for use in SiloamLearn by Jason Brewer, 2017
 * Also, there are now no globals
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */


#ifndef png_utilities_hpp
#define png_utilities_hpp

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <png.h>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>

// The width/height here are essentially arbitrary,
// but 400x400 is enough that I can pick out details
// myself on a screen and also enough detail for the
// covariance generation process

const int kPngWidth = 400;
const int kPngHeight = 400;
const int kNormalYStart = 300;
const int kNormalYEnd = 400;
const int kNormalNoiseThreshold = 2000;
const double kEuclideanClusterRadiusThreshold = 1.0f;

enum PngOperation {kPointCloud,
                   kFindFloorNormals,
                   kSegment,
                   kCluster,
                   kSobelX,
                   kSobelY,
                   kSobelMagnitude};

struct SimpleVoxel {
    int x;
    int y;
    int z;
};

struct RgbValue {
    int r;
    int g;
    int b;
    
    bool operator==(const RgbValue &other) const {
        return (r == other.r && g == other.g &&
                b == other.b);
    }
};

struct Coordinate {
    int x;
    int y;
};

struct ClusterCoordinate {
    int x;
    int y;
    int cluster_number;
};

typedef SimpleVoxel PointCloud[kPngHeight][kPngWidth];

struct PngProcessResultData {
    PointCloud* point_cloud;
    std::vector<Coordinate>* floor_estimate;
};

void read_png_file(const char *filename);

void write_png_file(const char *filename);

bool dead_png();

void find_clusters(int threshold, PngProcessResultData& result_data);

void process_png_file(PngOperation operation, PngProcessResultData& result_data);

// Custom but simple hash function for the normals hash table
// It doesn't particularly matter if there are hash collisions
// because we only want to store unique values anyway

namespace std {
    
    template<>
    struct hash<RgbValue> {
        std::size_t operator()(const RgbValue& rgb_value) const {
            using std::size_t;
            using std::hash;
            
            // Compute the hash values of each of the r, g and b
            // values, XOR and bit shift them together
            
            return ((hash<int>()(rgb_value.r)
                    ^ (hash<int>()(rgb_value.g) << 1)) >> 1)
                    ^ (hash<int>()(rgb_value.b) << 1);
        }
    };
}

#endif /* png_utilities_hpp */
