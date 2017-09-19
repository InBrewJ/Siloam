//
//  label_parser.hpp
//  SiloamLearn
//
//  Created by Jason  Brewer on 14/09/2017.
//  Copyright © 2017 University of Liverpool. All rights reserved.
//

#ifndef label_parser_hpp
#define label_parser_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>

// Used to parse "labelled.dat"

class LabelParser {
public:
    LabelParser(std::string label_file);
    
    // checks if the current line is a comment
    // (i.e. does it start with a # character)
    
    inline bool IsComment(std::string path) {
        return path.find("#") != std::string::npos;
    }
    
    // Put all those paths that have
    // an object of interest in them into
    // the labels_ map
    
    std::vector<std::string> GetUsableData();
    
    // maybe this should return an "label" type
    // "label" would be a struct of four bools
    // for each of the categories. Nah it should just
    // return a string probably. But the "label" type
    // could be used to make the binary flag parsing more
    // readable. Remember the labels only correspond to the
    // RGB pngs
    
    std::string GetLabel(std::string);
    
private:
    struct Label {
        bool bus_stop;
        bool crossing;
        bool door;
        bool steps;
    };
    
    std::string label_file_;
    
    // TODO: some sort of map to relate each path to a label
    // so that the label can be looked up quickly (with O(1)
    // again hopefully - there's definitely a string hashing
    // function already) - call it labels_

};

#endif /* label_parser_hpp */
