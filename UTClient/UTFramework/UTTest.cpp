//
//  UTTest.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/20/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "UTTest.h"

#include <string>
#include <sstream>

#include "ConsoleColors.h"


// ======  UTCondition  ================================================================================================
UTCondition::UTCondition(bool didPass, std::string name, std::string asertMessage) :
    DidPass(didPass),
    Name(name),
    AssertMessage(asertMessage) {
    
}

std::ostream& operator<< (std::ostream& stream, const UTCondition& utCondition) {
    if (utCondition.DidPass) {
        stream << Blue << "|   |   " << Green << "-" << utCondition.Name << std::endl;
    } else {
        stream << Blue << "|   |   " << Red << "-" << utCondition.Name << std::endl;
        stream << Blue << "|   |   |   " << Yellow << utCondition.AssertMessage << std::endl;
    }
    
    return stream;
}


// ======  UTTestConfiguration  ========================================================================================
UTTestConfiguration::UTTestConfiguration() :
    MinMemory(-1),
    MaxMemory(-1),
    LeakCheck(true) {
    
}


// ======  UTTest  =====================================================================================================
UTTest::UTTest()  :
    Name("unknown"),
    FatalMessage("") {

}

bool UTTest::DidPass()  {
    bool didPass = true;
    for (UTCondition condition : Conditions) {
        if (!condition.DidPass)
            didPass = false;
    }
    
    return didPass;
}

std::ostream& operator<< (std::ostream& stream, UTTest& utTest) {
    stream << Blue << "|   Test: " << utTest.Name << std::endl;
    
    // Print out all but the last conditions
    for (UTCondition condition : utTest.Conditions) {
        stream << condition;
    }

    if (utTest.FatalMessage == "") {
        if (utTest.DidPass()) {
            stream << Blue << "|   " << Green << "Passed!" << std::endl;
        } else {
            stream << Blue << "|   " << Red << "Failed!" << std::endl;
        }
    } else {
        std::istringstream iss(utTest.FatalMessage);
        for (std::string line; std::getline(iss, line); ) {
            stream << Blue << "|   " << Red << line << std::endl;
        }
        // stream << Blue << "|   " << Red << "Failed!" << std::endl;
    }
    
    stream << Blue << "|" << std::endl;
    
    return stream;
}