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
    
    PointCloud* point_cloud = new PointCloud[kPngHeight];
    
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
    // Just generates a simple x y z point cloud at the moment
    point_cloud = process_png_file(kPointCloud);
    write_png_file("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/sandbox/test.png");
    
    for (int y = 0; y < kPngHeight; y++) {
        for (int x = 0; x < kPngWidth; x++) {
            printf("%4d, %4d = Voxel(%3d, %3d, %3d)\n", x, y,
                   point_cloud[y][x]->x,
                   point_cloud[y][x]->y,
                   point_cloud[y][x]->z);
        }
    }
}

// Scan the 100x50 pixel portion of each 400x400 N image
// and find a range of values that represent the normals of
// the "floor" plane. Scan the rest of the image and remove
// any other pixels that lie in this range. Pixels we don't use
// are marked as all black RGB(0, 0, 0)

void ImageProcessor::Segment() {
    std::vector<std::string> normal_files;
    
    for (auto const& path: train_png_paths_) {
        if ( path.find("N.png") != std::string::npos ) {
            normal_files.push_back(path);
        }
    }
    
    //std::cout << "N file under test: " << std::endl;
    //std::cout << normal_files.back() << std::endl;
    
    // forwards13 is also good for test here
    
    read_png_file("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamSee/SiloamSee-170811-155553-train/forwards9/N.png");
    process_png_file(kSegment);
    write_png_file("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/sandbox/test.png");
    
}
