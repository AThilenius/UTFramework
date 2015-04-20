//
//  main.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/16/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include <iostream>
#include "UTTestRunner.h"


SUITE (SuiteOne) {
    
    std::cout << "Suite Called" << std::endl;
    
    
    runner->GetConfig()->MaxMemory = 42;
    
    TEST ("Test One") {
        std::cout << "Test One Run!" << std::endl;
        runner->IsTrue(false, "Test Condition One", "Failure Message");
    }
    
    
    runner->GetConfig()->MinMemory = 16;
    
    TEST ("Test Two") {
        runner->IsTrue(true, "Test Condition One", "Should not be shown");
        throw std::exception();
    }
    
    TEST ("Test Three") {
        runner->IsTrue(true, "Test Condition One", "Should not be shown");
    }
    
    TEST ("Test Four") {
        runner->IsTrue(true, "Test Condition One", "Should not be shown");
        while (true) {}
    }
    
}

int main(int argc, const char * argv[]) {
    UTTestRunner runner;
    runner.RunSuite("Test Name", &SuiteOne);
}
