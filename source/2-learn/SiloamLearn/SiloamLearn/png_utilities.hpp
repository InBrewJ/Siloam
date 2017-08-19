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
#include <png.h>

// The width/height here are essentially arbitrary,
// but 400x400 is enough that I can pick out details
// myself on a screen and also enough detail for the
// covariance generation process

const int kPngWidth = 400;
const int kPngHeight = 400;

enum PngOperation {kPointCloud, kSobelX, kSobelY, kSobelMagnitude};

void read_png_file(const char *filename);

void write_png_file(const char *filename);

bool dead_png();

void process_png_file(PngOperation operation);

struct Voxel {
    int x;
    int y;
    int z;
};

#endif /* png_utilities_hpp */
