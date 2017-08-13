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

void ImageProcessor::PrintProcessingPath() {
    std::cout << img_path_ << std::endl;
}

void ImageProcessor::Walk() {
    path current_dir(img_path_); //
    boost::regex pattern(".*\.png"); // list all files starting with a
    for (recursive_directory_iterator iter(current_dir), end;
         iter != end;
         ++iter)
    {
        std::string name = iter->path().filename().string();
        if (regex_match(name, pattern))
            std::cout << iter->path() << "\n";
    }
}
