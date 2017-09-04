//
//  image_processor.cpp
//  SiloamLearn
//
//  Created by Jason  Brewer on 12/08/2017.
//  Copyright © 2017 University of Liverpool. All rights reserved.
//

#include "image_processor.hpp"

using namespace cv;

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
    
    // regexes used to generate segmented image output filenames
    // Get all images in the parent directory apart from the already
    // processes N.png. Just do a [^N] regex since there could be other
    // images in the renders folder in the future
    
    boost::regex n_pattern(".*forwards.*[N]\.png");
    boost::regex rgb_pattern(".*forwards.*[RGB]\.png");
    boost::regex i_pattern(".*forwards.*[I]\.png");
    boost::regex d_pattern(".*forwards.*[D]\.png");
    std::string output_path = "";
    std::string final_filename = "";
    
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

    
    // Remove the floor from the RGB, I and D images too!
    
    // Remove the floor pixels based on floor_estimation
    
    for (auto const& normal_image: normal_files) {
        
        // Detect the floor from the normals images
        
        read_png_file(normal_image.c_str());
        process_png_file(kFindFloorNormals, results);
        path current_image_subset(normal_image);
        
        std::cout << "Removing floors from: "
                    << current_image_subset.parent_path()
                    << std::endl;
        
        for (recursive_directory_iterator iter(current_image_subset.parent_path()),
             end;
             iter != end;
             ++iter)
        {
            std::string name = iter->path().string();
            
            read_png_file(name.c_str());
            
            // Only find the final floor estimation
            // based on the D image to remove odd
            // artifacts that might be on surfaces
            // behind objects of interest
            
            if (regex_match(name, d_pattern)) {
                process_png_file(kSegment, results);
            } else {
                process_png_file(kRemoveFloor, results);

            }
            
            // Craft the ouput path
            
            if (regex_match(name, n_pattern)) {
                final_filename = "N_seg.png";
            } else if (regex_match(name, rgb_pattern)) {
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

// Generates the results of the Sobel filter
// convoluted over the segmented I and D images as follows:
// Ix    -> output along the X axis on I
// Iy    -> output along the Y axis on I
// IMag  -> magnitude of the gradient of I at each point
// Ixx   -> output along the X axis on Ix
// Iyy   -> output along the Y axis on Iy
// Ixy   -> output along the Y axis on Ix
// Dx    -> output along the X axis on D
// Dy    -> output along the Y axis on D
// DMag  -> magnitude of the gradient of D at each point
// Gaussian blur is first added to each image to reduce noise
// at the edges

void ImageProcessor::GenerateSobel() {
    
    boost::regex i_pattern(".*forwards.*[I]\.png");
    boost::regex d_pattern(".*forwards.*[D]\.png");
    std::string output_path = "";
    std::string final_filename = "";
    Mat current_image;
    Mat grad;
    Mat grad_x, grad_y, grad_xx, grad_yy, grad_xy;
    Mat abs_grad_x, abs_grad_y, abs_grad_xx, abs_grad_yy, abs_grad_xy;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    
    // Concatenate all the png paths together so we can
    // find the I and D images in one loop
    
    all_png_paths_ = train_png_paths_;
    
    all_png_paths_.insert(all_png_paths_.end(), test_png_paths_.begin()
                                              , test_png_paths_.end()
                          );
    
    all_png_paths_.insert(all_png_paths_.end(), ground_truth_png_paths_.begin()
                                              , ground_truth_png_paths_.end()
                          );
    std::cout << "Size of all_png_paths_: " << all_png_paths_.size() << std::endl;
    
    for (auto const& path: all_png_paths_) {
        if ( regex_match(path, i_pattern) || regex_match(path, d_pattern) ) {
            // Generate Ix, Iy, Imag, Ixx, Iyy, Ixy
            // Dx, Dy and DMag
            current_image = imread(path.c_str());
            class path current_image_subset(path);
            
            GaussianBlur( current_image,
                          current_image,
                          Size(3,3),
                          0,
                          0,
                          BORDER_DEFAULT
                         );
            
            // Ix or Dx
            Sobel( current_image, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
            convertScaleAbs( grad_x, abs_grad_x );
            
            // Iy or Dy
            Sobel( current_image, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
            convertScaleAbs( grad_y, abs_grad_y );
            
            // Imag or Dmag (approximation)
            addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
            
            // Write I or D files
            //imwrite( "path", grad_x );
            //imwrite( "path", grad_y );
            //imwrite( "path", grad);
            
            
            if ( regex_match(path, i_pattern) ) {
            
                // Ixx
                
                Sobel( grad_x, grad_xx, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
                convertScaleAbs( grad_x, abs_grad_x );
                
                // Iyy
                
                Sobel( grad_y, grad_yy, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
                convertScaleAbs( grad_x, abs_grad_x );
                
                // Ixy
                
                Sobel( grad_x, grad_xy, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
                convertScaleAbs( grad_x, abs_grad_x );
                
                // Write I specific files HERE
                // imwrite( "path", gray_image );
            
            }
        }
    }
}
