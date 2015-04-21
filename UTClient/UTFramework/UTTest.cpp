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


// ======  MemoryAllocation  ===========================================================================================
MemoryAllocation::MemoryAllocation(void* address, size_t size, std::string file, size_t line) :
    Address(address),
    Size(size),
    File(file),
    Line(line) {
    
    std::vector<std::string> tokens;
    std::istringstream stringStream (File);
    std::string token;
    while (std::getline(stringStream, token, '\\')) {
        tokens.push_back(token);
    }
        
        if (tokens.size() > 1) {
            File = tokens[tokens.size() - 1];
        } else {
            // Split on /
            stringStream = std::istringstream (File);
            tokens.clear();
            while (std::getline(stringStream, token, '/')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() > 1) {
                File = tokens[tokens.size() - 1];
            }
        }
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
    
    size_t closedSize = 0;
    for (MemoryAllocation allocation : ClosedAllocations) {
        closedSize += allocation.Size;
    }
    
    size_t ftcSize = 0;
    for (auto kvp: OutstandingAllocations) {
        ftcSize += kvp.second.Size;
    }
    
    size_t ftoSize = 0;
    for (auto kvp: OutstandingFrees) {
        ftoSize += kvp.second.Size;
    }
    
    // First check for memory leaks
    if (Configuration.LeakCheck && ftcSize > 0) {
        std::stringstream stringStream;
        stringStream << "Memory Leak! 'new' was called without a corresponding 'delete':\n";
        for (auto kvp : OutstandingAllocations) {
            stringStream << "  - " << kvp.second.Size << " Bytes from: " << kvp.second.File << ". Line: "
                << kvp.second.Line << "\n";
        }
        stringStream << ftcSize << " Bytes leaked total";
        FatalMessage = stringStream.str();
        return false;
    }
   
    // Min check
    if (Configuration.MinMemory != -1 && Configuration.MinMemory > closedSize) {
        std::stringstream stringStream;
        stringStream << "Not Enough Heap Memory Allocated!\n";
        stringStream << " - Minimum Required Heap Allocations: " << Configuration.MinMemory << " Bytes\n";
        stringStream << " - Total Heap Allocations: " << closedSize << " Bytes";
        FatalMessage = stringStream.str();
        return false;
    }
    
    // Max check
    if (Configuration.MaxMemory != -1 && Configuration.MaxMemory < closedSize) {
        std::stringstream stringStream;
        stringStream << "Too Much Heap Memory Allocated!\n";
        stringStream << " - Maximum Allowed Heap Allocations: " << Configuration.MaxMemory << " Bytes\n";
        stringStream << " - Total Heap Allocations: " << closedSize << " Bytes";
        FatalMessage = stringStream.str();
        return false;
    }
    
    return didPass;
}


void UTTest::RegisterAllocation (MemoryAllocation allocation) {
    OutstandingAllocations.insert( { allocation.Address, allocation } );
}

void UTTest::RegisterFree (void* ptr) {
    auto iter = OutstandingAllocations.find(ptr);
    if (iter != OutstandingAllocations.end()) {
        // Close correctly
        ClosedAllocations.push_back(iter->second);
    } else {
        // delete called without new being called
        OutstandingFrees.insert( { iter->first, iter->second } );
    }
    
    OutstandingAllocations.erase(iter);
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