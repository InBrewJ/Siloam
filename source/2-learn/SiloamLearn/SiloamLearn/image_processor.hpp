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
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace boost::filesystem;

class ImageProcessor {
public:
    ImageProcessor(std::string img_path) {
        img_path_ = img_path;
    }
    
    inline void PrintProcessingPath() {
        std::cout << img_path_ << std::endl;
    }
    
    void Walk() {
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
    
private:
    std::string img_path_;
    
    
};

#endif /* image_processor_hpp */
