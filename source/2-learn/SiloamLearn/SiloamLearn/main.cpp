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
#include "image_processor.hpp"

int main(int argc, const char * argv[]) {
    
    // The path below will of course be defined on the command line in the future
    
    ImageProcessor image_processor("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders");
    
    //image_processor.GetDataset();
    //image_processor.PrintPngInfo();
    //image_processor.PrintRgbPaths();
    //image_processor.GeneratePointClouds();
    //image_processor.Segment();
    image_processor.GenerateSobel();
    
    
    return 0;
}
