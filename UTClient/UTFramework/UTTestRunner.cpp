//
//  UTTestRunner.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/20/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "UTTestRunner.h"

#undef new
#undef delete

#include <thread>
#include <chrono>
#include <signal.h>

std::function<void()> g_testIterateFunction;
std::function<void()> g_testEndFunction;
std::function<void()> g_sigSegFailure;
std::function<void*(size_t)> g_defaultNewHandler = [](size_t size) -> void* { return malloc(size); };
std::function<void(void*)> g_defaultDeleteHandler = [](void* ptr) -> void { free(ptr); };
std::function<void*(size_t)> g_newHandler = g_defaultNewHandler;
std::function<void(void*)> g_deleteHandler = g_defaultDeleteHandler;

// Used to protect the test-end print
std::mutex g_printLock;

// Use by the new / delete overload macros
const char* __file__ = "unknown";
size_t __line__ = -1;
bool __handler_active__ = false;

void* operator new(size_t size) {
    if (__handler_active__) {
        return g_defaultNewHandler(size);
    } else {
        __handler_active__ = true;
        void* ptr = g_newHandler(size);
        __file__ = "unknown";
        __line__ = -1;
        __handler_active__ = false;
        return ptr;
    }
}

void operator delete(void* ptr) noexcept {
    if (__handler_active__) {
        g_defaultDeleteHandler(ptr);
    } else {
        __handler_active__ = true;
        g_deleteHandler(ptr);
        __file__ = "unknown";
        __line__ = -1;
        __handler_active__ = false;
    }
}

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
            SigSegCatcher(suiteFunction);
            
            m_currentTestIndex = 0;
        }
    };
    
    // Create the end function (can then be used by the POSIX SIGSEGV handler)
    g_testEndFunction = [this, suiteName]() {
        g_printLock.lock();
        g_testEndFunction = []() {};
        g_printLock.unlock();
        
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
        
        std::cout.flush();
    };
    
    // Used by the SIG-SEG handler to post an error message.
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
    // Register Custom handlers for new / delete
    g_newHandler = [this](size_t size) -> void* {
        void* ptr = malloc(size);
        
        if (__line__ != -1) {
            this->m_activeTest->RegisterAllocation(MemoryAllocation(ptr, size, __file__, __line__));
        }
        
        return ptr;
    };
    
    g_deleteHandler = [this](void* ptr) -> void {
        if (__line__ != -1) {
            this->m_activeTest->RegisterFree(ptr);
        }
        
        free(ptr);
    };
    
    StdExceptionCatcher(suiteFunction);
}

void UTTestRunner::ThreadedTimeout(std::function<void(UTTestRunner*)> suiteFunction) {
    std::cout.flush();
    bool threadFinished = false;
    std::thread workerThread ([this, &suiteFunction, &threadFinished](){
        // No need to syncronize this
        this->MemoryMonitor(suiteFunction);
        threadFinished = true;
    });
    // workerThread.detach();
    
    // Again, no need to syncronize this, there are no possible race conditions that I care about
    auto endTime = std::chrono::steady_clock::now() + std::chrono::seconds(4);
    while (threadFinished == false && std::chrono::steady_clock::now() < endTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Check if we timed out
    if (std::chrono::steady_clock::now() >= endTime) {
        workerThread.detach();
        m_activeTest->FatalMessage = "Timed out, likley an infite loop or waiting for user input!";
    } else {
        workerThread.join();
    }
    
    std::cout.flush();
}

void SigSegVHandler (int signum) {
    std::cout << "SIG-SEG Handler" << std::endl;
    
    // Re-Register the SIG-SEGV handler
    signal(SIGSEGV, SigSegVHandler);
    
    g_sigSegFailure();
    g_testIterateFunction();
    g_testEndFunction();
    std::cout.flush();
    signal(signum, SIG_DFL);
}

void UTTestRunner::SigSegCatcher(std::function<void(UTTestRunner*)> suiteFunction) {
    signal(SIGSEGV, SigSegVHandler);
    ThreadedTimeout(suiteFunction);
}