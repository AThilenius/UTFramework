//
//  UTTest.h
//  UTFramework
//
//  Created by Alec Thilenius on 4/20/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#pragma once

#include <iostream>
#include <vector>


// ======  UTCondition  ================================================================================================
class UTCondition {
    
public:
    UTCondition (bool didPass, std::string name, std::string asertMessage);
    
private:
    friend std::ostream& operator<< (std::ostream& stream, const UTCondition& utCondition);
    
public:
    bool DidPass;
    std::string Name;
    std::string AssertMessage;
};


// ======  UTTestConfiguration  ========================================================================================
class UTTestConfiguration {
    
public:
    UTTestConfiguration ();
    
public:
    int MinMemory;
    int MaxMemory;
    bool LeakCheck;
};


// ======  UTTest  =====================================================================================================
class UTTest {
    
public:
    UTTest();
    bool DidPass();
    
    friend std::ostream& operator<< (std::ostream& stream, UTTest& utTest);
    
public:
    std::string Name;
    std::string FatalMessage;
    UTTestConfiguration Configuration;
    std::vector<UTCondition> Conditions;
};