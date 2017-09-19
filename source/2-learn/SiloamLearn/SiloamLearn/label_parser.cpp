//
//  label_parser.cpp
//  SiloamLearn
//
//  Created by Jason  Brewer on 14/09/2017.
//  Copyright © 2017 University of Liverpool. All rights reserved.
//

#include "label_parser.hpp"

LabelParser::LabelParser(std::string label_file) {
    label_file_ = label_file;
}

std::vector<std::string> LabelParser::GetUsableData() {
    
    boost::regex even_camera_pattern(".*[02468]\/.*\.png");
    std::vector<std::string> usable_data;
    std::ifstream label_file (label_file_);
    std::string this_path = "";
    std::string this_line = "";
    Label this_label;
    
    // TODO: Have a regex here for even cameras!!!
    
    for (int line_number = 1; std::getline(label_file, this_line); line_number++) {
        if (!IsComment(this_line)) {
            std::stringstream(this_line) >> this_path
                                    >> this_label.bus_stop
                                    >> this_label.crossing
                                    >> this_label.door
                                    >> this_label.steps;
            
            if ( this_label.bus_stop | this_label.crossing | this_label.door
                | this_label.steps ) {
                if ( regex_match(this_path, even_camera_pattern) ) {
                    usable_data.push_back(this_path);
                }
            } else {
                std::cout << "Unusable datum found on line " << line_number << std::endl;
            }
            
        } else {
            std::cout << "Comment found on line " << line_number << std::endl;
        }
    }
    
    return usable_data;
}
