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
    
    ImageProcessor(std::string img_path);
    
    inline void PrintProcessingPath();
    
    void Walk();
    
private:
    std::string img_path_;
    
};

#endif /* image_processor_hpp */
