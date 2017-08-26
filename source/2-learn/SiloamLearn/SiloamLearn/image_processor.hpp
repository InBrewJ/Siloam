//
//  image_processor.hpp
//  SiloamLearn
//
//  Only supports PNGs generated from the SiloamLearn blender
//  workflow for now
//
//  Created by Jason  Brewer on 12/08/2017.
//  Copyright © 2017 University of Liverpool. All rights reserved.
//

#ifndef image_processor_hpp
#define image_processor_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <cstring>
#include "png_utilities.hpp"

using namespace boost::filesystem;

class ImageProcessor {
public:
    
    ImageProcessor(std::string img_path);
    
    inline void PrintProcessingPath() {
        std::cout << img_path_ << std::endl;
    };
    
    void GetDataset();
    
    void PrintPngInfo();
    
    void GeneratePointClouds();
    
    void Segment();
    
    void PrintRgbPaths();
    
private:
    std::string img_path_;
    std::vector<std::string> train_png_paths_;
    std::vector<std::string> test_png_paths_;
    std::vector<std::string> ground_truth_png_paths_;
    
};

#endif /* image_processor_hpp */
