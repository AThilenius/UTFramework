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
#include <unordered_map>


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



// ======  MemoryAllocation  ===========================================================================================
class MemoryAllocation {
public:
    MemoryAllocation(void* address, size_t size, std::string file, size_t line);
    
public:
    void* Address;
    size_t Size;
    std::string File;
    size_t Line;
};



// ======  UTTest  =====================================================================================================
class UTTest {
    
public:
    UTTest();
    bool DidPass();
    void RegisterAllocation (MemoryAllocation allocation);
    void RegisterFree (void* ptr);
    
    friend std::ostream& operator<< (std::ostream& stream, UTTest& utTest);
    
public:
    std::string Name;
    std::string FatalMessage;
    UTTestConfiguration Configuration;
    std::vector<UTCondition> Conditions;
    std::unordered_map<void*, MemoryAllocation> OutstandingAllocations;
    std::unordered_map<void*, MemoryAllocation> OutstandingFrees;
    std::vector<MemoryAllocation> ClosedAllocations;
};