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

void ImageProcessor::GetDataset() {
    path current_dir(img_path_);
    
    boost::regex train_pattern(".*train.*forwards.*\.png");
    boost::regex test_pattern(".*test.*forwards.*\.png");
    boost::regex ground_truth_pattern(".*gt.*forwards.*\.png");
    boost::regex siloamlearn_pattern(".*SiloamLearn.*\.png");
    
    for (recursive_directory_iterator iter(current_dir), end;
         iter != end;
         ++iter)
    {
        std::string name = iter->path().string();
        if (regex_match(name, siloamlearn_pattern) && regex_match(name, train_pattern)) {
            read_png_file(iter->path().string().c_str());
            
            if( !dead_png() ) {
                train_png_paths_.push_back(iter->path().string());
            } else {
                std::cout << iter->path() << " is a dead PNG!" << "\n";
            }
            
        } else if (regex_match(name, train_pattern)) {
            train_png_paths_.push_back(iter->path().string());
            
        } else if (regex_match(name, test_pattern)) {
            test_png_paths_.push_back(iter->path().string());
            
        } else if (regex_match(name, ground_truth_pattern)) {
            ground_truth_png_paths_.push_back(iter->path().string());
            
       }
    }
}

void ImageProcessor::PrintPngInfo() {
    std::cout << train_png_paths_.size() + test_png_paths_.size() + ground_truth_png_paths_.size()  << " PNGs found" << std::endl;
    
    std::cout << "\n\tTrain PNGs: " << train_png_paths_.size()
              << "\n\tTest PNGs: " << test_png_paths_.size()
              << "\n\tGround Truth PNGs: " << ground_truth_png_paths_.size()
              << "\n\n";
    
}

void ImageProcessor::PrintRgbPaths() {
    
    boost::regex rgb_pattern(".*RGB\.png");
    
    for (auto const& path: train_png_paths_) {
        if ( regex_match(path, rgb_pattern) ) {
            std::cout << path << std::endl;
        }
    }
    
    for (auto const& path: test_png_paths_) {
        if ( regex_match(path, rgb_pattern) ) {
            std::cout << path << std::endl;
        }
    }
    
    for (auto const& path: ground_truth_png_paths_) {
        if ( regex_match(path, rgb_pattern) ) {
            std::cout << path << std::endl;
        }
    }
    
}

void ImageProcessor::GeneratePointClouds() {
    
    // Use std::string::find to get the I.png!
    
    std::vector<std::string> intensity_files;
    
    for (auto const& path: train_png_paths_) {
        if ( path.find("I.png") != std::string::npos ) {
            intensity_files.push_back(path);
        }
    }
    
    std::cout << "I file under test: " << std::endl;
    std::cout << intensity_files.back() << std::endl;
    
    read_png_file(intensity_files.back().c_str());
    // Just makes everything yellow at the moment
    process_png_file(kPointCloud);
    write_png_file("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/sandbox/test.png");
    
                  
    
}
