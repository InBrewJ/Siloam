//  NB: BUILT WITH XCODE 8.3.3 (8E3004b) on macOS Sierra 10.12.5
//
//
//  main.cpp
//  SiloamLearn
//
//  
//
//  Created by Jason  Brewer on 11/08/2017.
//  License review pending. But probably released under something
//  to do with BSD/Apache. Probably Apache. But now probably GNU
//  because of pyclustering
//

// sandbox directory:
// /Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/sandbox
// renders directory:
// /Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders

#include <iostream>
#include <string>
#include "image_processor.hpp"

int main(int argc, const char * argv[]) {
    
    // The path below will be defined as a command line argument in the future
    
    std::string renders_path = "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders";
    std::string labelled_path = "/Users/LordNelson/Dropbox/Work/LiverpoolUni/Modules/Dissertation/repo/source/2-learn/labelled.dat";
    
    ImageProcessor image_processor(renders_path, labelled_path);
    
    image_processor.GetDataset();
    image_processor.Segment();
    //image_processor.GeneratePointClouds();
    //image_processor.GenerateSobel();
    
    
    return 0;
}
