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
                // TODO: this needs testing...
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
    
    PngProcessResultData results;
    results.point_cloud = new PointCloud[kPngHeight];
    
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
    process_png_file(kPointCloud, results);
    write_png_file("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/sandbox/test.png");
    
    for (int y = 0; y < kPngHeight; y++) {
        for (int x = 0; x < kPngWidth; x++) {
            printf("%4d, %4d = Voxel(%3d, %3d, %3d)\n", x, y,
                   results.point_cloud[y][x]->x,
                   results.point_cloud[y][x]->y,
                   results.point_cloud[y][x]->z);
        }
    }
}

// Scan the kPngWidthx(kNormalYEnd - kNormalYStart) pixel
// portion of each 400x400 N image
// and find a range of values that represent the normals of
// the "floor" plane. Scan the rest of the image and remove
// any other pixels that lie in this range. Pixels we don't use
// are marked as all black RGB(0, 0, 0)

void ImageProcessor::Segment() {
    
    // forwards 9 for the bin
    // forwards13 is also good for test here
    // Door with bin central teaching hub
    std::string path0 = "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamSee/SiloamSee-170811-155553-train/forwards9/N.png";
    // Sample crossing
    std::string path1 = "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamSee/SiloamSee-170811-162826-test/forwards3/N.png";
    // Just wall and floor planes
    std::string path2 = "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamSee/SiloamSee-170811-145932-train/forwards0/N.png";
    
    PngProcessResultData results;
    results.floor_estimate = new std::vector<Coordinate>;
    std::vector<std::string> normal_files;
    
    for (auto const& path: train_png_paths_) {
        if ( path.find("N.png") != std::string::npos ) {
            normal_files.push_back(path);
        }
    }
    
    for (auto const& path: test_png_paths_) {
        if ( path.find("N.png") != std::string::npos ) {
            normal_files.push_back(path);
        }
    }
    
    for (auto const& path: ground_truth_png_paths_) {
        if ( path.find("N.png") != std::string::npos ) {
            normal_files.push_back(path);
        }
    }
    
    // Detect the floor from the normals images
    
    read_png_file(path0.c_str());
    
    process_png_file(kFindFloor, results);
    write_png_file("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/sandbox/test.png");
    
    printf("floor_estimate front: %d, %d\n", results.floor_estimate->front().x,
                                             results.floor_estimate->front().y);
    
    printf("floor_estimate back: %d, %d\n", results.floor_estimate->back().x,
                                            results.floor_estimate->back().y);
    
    std::cout << "floor_estimate size:  " << results.floor_estimate->size()
              << std::endl;
    
//    for (auto const& coord: *results.floor_estimate) {
//            printf("Floor coord at x: %d y: %d\n", coord.x, coord.y);
//    }
    
    // Remove the floor from the RGB, I and D images too!
    
    // Get the parent directory
    path current_image_subset(path0);
    std::cout << current_image_subset.parent_path() << std::endl;
    
    // Get all images in the parent directory apart from the already
    // processes N.png. Just do a [^N] regex since there could be other
    // images in the renders folder in the future
    
    boost::regex no_n_pattern(".*forwards.*[^N]\.png");
    boost::regex rgb_pattern(".*forwards.*[RGB]\.png");
    boost::regex i_pattern(".*forwards.*[I]\.png");
    boost::regex d_pattern(".*forwards.*[D]\.png");
    
    
    // Remove the floor pixels based on floor_estimation for
    // all images apart from N.png, since this has already been
    // processed
    
    std::string output_path;
    std::string final_filename;
    
    // We're only looking inside one directory with no subdirectories so
    // this recursive_directory_iterator should be pretty fast
    
    for (recursive_directory_iterator iter(current_image_subset.parent_path()),
         end;
         iter != end;
         ++iter)
    {
        std::string name = iter->path().string();
        if (regex_match(name, no_n_pattern)) {
            read_png_file(name.c_str());
            process_png_file(kSegment, results);
            
            // Craft the ouput path
            
            if (regex_match(name, rgb_pattern)) {
                final_filename = "RGB_seg.png";
            } else if (regex_match(name, i_pattern)) {
                final_filename = "I_seg.png";
            } else if (regex_match(name, d_pattern)) {
                final_filename = "D_seg.png";
            }
            
            output_path = current_image_subset.parent_path().string()
                            + "/"
                            + final_filename;
            
            write_png_file(output_path.c_str());
            
        }
        
    }
    
}
