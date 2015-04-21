//
//  UTTestRunner.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/20/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "UTTestRunner.h"

#include <thread>
#include <future>
#include <chrono>
#include <signal.h>

// Needed by the POSIX SIG-SEGV handler. Don't have a good way around this ugly-ness yet.
std::function<void()> g_testIterateFunction;
std::function<void()> g_testEndFunction;
std::function<void()> g_sigSegFailure;

void UTTestRunner::RunSuite(std::string suiteName, std::function<void(UTTestRunner*)> suiteFunction) {
    // Set up the first UTTest now, to allow for the Config to be changed
    m_tests.push_back(UTTest());
    
    // Bind the scrap handler
    m_handler = std::bind(&UTTestRunner::ScrapeHandler, this, std::placeholders::_1);
    
    // Run the Scrape phase
    suiteFunction(this);
    
    // Bind the runner handler
    m_handler = std::bind(&UTTestRunner::RunHandler, this, std::placeholders::_1);
    m_activeTestIndex = -1;
    
    // Create the iterate function (can then be used by the POSIX SIGSEGV handler)
    g_testIterateFunction = [this, suiteFunction]() {
        m_currentTestIndex = 0;
        m_activeTestIndex++;
        for (m_activeTest = 0; m_activeTestIndex < m_tests.size() - 1; m_activeTestIndex++) {
            
            m_activeTest = &m_tests[m_activeTestIndex];
            
            // Need to load the new config here. Might want to use a RACK format for this...
            ThreadedTimeout(suiteFunction);
            
            m_currentTestIndex = 0;
        }
    };
    
    // Create the end function (can then be used by the POSIX SIGSEGV handler)
    g_testEndFunction = [this, suiteName]() {
        bool didPass = true;
        for (int i = 0; i < m_tests.size() - 1; i++) {
            UTTest* test = &m_tests[i];
            if (!test->DidPass())
                didPass = false;
        }
        
        std::cout << std::endl;
        std::cout << Blue << "Suite: " << suiteName << std::endl;
        std::cout << Blue << "|" << std::endl;
        for (int i = 0; i < m_tests.size() - 1; i++) {
            UTTest* test = &m_tests[i];
            std::cout << *test;
        }
        
        if (didPass) {
            std::cout << Green << "Passed!" << White << std::endl;
        } else {
            std::cout << Red << "Failed!" << White << std::endl;
        }
    };
    
    g_sigSegFailure = [this]() {
       m_activeTest->FatalMessage = "SEG-FAULT!\nYou are likley trying to dereference a null pointer.";
    };
    
    // Funally call both
    g_testIterateFunction();
    g_testEndFunction();
}

void UTTestRunner::IsTrue (bool expression, std::string name, std::string assertMessage) {
    // Add the condition
    m_activeTest->Conditions.push_back(UTCondition(expression, name, assertMessage));
}

UTTestConfiguration* UTTestRunner::GetConfig() {
    return &(m_tests[m_tests.size() - 1]).Configuration;
}

// Used by the testing framework itself
bool UTTestRunner::_TestPass (std::string testName) {
    return m_handler(testName);
}

// _TestPass handlers
bool UTTestRunner::ScrapeHandler (std::string testName) {
    // Update the last UTTest
    m_tests[m_tests.size() - 1].Name = testName;
    
    // Create a new config for the next test
    m_tests.push_back(UTTest());
    
    // Don't run the test
    return false;
}

bool UTTestRunner::RunHandler (std::string testName) {
    
    // Returns true for ONLY the active test
    if (m_currentTestIndex == m_activeTestIndex) {
        m_currentTestIndex++;
        return true;
    }
    
    m_currentTestIndex++;
    return false;
}

void UTTestRunner::StdExceptionCatcher(std::function<void(UTTestRunner*)> suiteFunction) {
    try {
        suiteFunction(this);
    } catch (...) {
       m_activeTest->FatalMessage = "A fatal exception was caught!";
    }
}

void UTTestRunner::MemoryMonitor(std::function<void(UTTestRunner*)> suiteFunction) {
    StdExceptionCatcher(suiteFunction);
}

void SigSegVHandler (int signum) {
    std::cout << "SIG-SEG Handler" << std::endl;
    
    // Re-Register the SIG-SEGV handler
    signal(SIGSEGV, SigSegVHandler);
    
    g_sigSegFailure();
    g_testIterateFunction();
    signal(signum, SIG_DFL);
    g_testEndFunction();
}

void UTTestRunner::SigSegCatcher(std::function<void(UTTestRunner*)> suiteFunction) {
 
    MemoryMonitor(suiteFunction);
    
//    signal(signum, SIG_DFL);
}

void UTTestRunner::ThreadedTimeout(std::function<void(UTTestRunner*)> suiteFunction) {
    signal(SIGSEGV, SigSegVHandler);
    SigSegCatcher(suiteFunction);
//    bool threadFinished = false;
//    std::thread workerThread ([this, &suiteFunction, &threadFinished](){
//        // No need to syncronize this
//        this->SigSegCatcher(suiteFunction);
//        threadFinished = true;
//    });
//    workerThread.detach();
//    
//    // Again, no need to syncronize this, there are no possible race conditions that I care about
//    auto endTime = std::chrono::steady_clock::now() + std::chrono::seconds(4);
//    while (threadFinished == false && std::chrono::steady_clock::now() < endTime) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//    }
//    
//    // Check if we timed out
//    if (std::chrono::steady_clock::now() >= endTime) {
//        m_activeTest->FatalMessage = "Timed out, likley an infite loop or waiting for user input!";
//    }
}