//
//  image_processor.cpp
//  SiloamLearn
//
//  Created by Jason  Brewer on 12/08/2017.
//  Copyright © 2017 University of Liverpool. All rights reserved.
//

#include "image_processor.hpp"

ImageProcessor::ImageProcessor(std::string img_path) {
    img_path_ = img_path;
}

void ImageProcessor::Walk() {
    path current_dir(img_path_); //
    boost::regex pattern(".*png"); // list all files starting with a
    for (recursive_directory_iterator iter(current_dir), end;
         iter != end;
         ++iter)
    {
        std::string name = iter->path().filename().string();
        if (regex_match(name, pattern)) {
            std::cout << iter->path() << "\n";
            png_paths_.push_back(iter->path().string());
            read_png_file(png_paths_.back().c_str());
            if(dead_png() == true) {
                std::cout << iter->path() << " is a dead PNG!" << "\n";
            }
        }
    }
}

void ImageProcessor::PrintPngInfo() {
    std::cout << png_paths_.size() << std::endl;
    std::cout << png_paths_.back() << std::endl;
    
    // Read the png file and print out the contents of each pixel
    
    read_png_file(png_paths_.back().c_str());
    process_png_file();
                  
}
