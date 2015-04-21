//
//  UTTestRunner.h
//  UTFramework
//
//  Created by Alec Thilenius on 4/20/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#pragma once

#include <functional>

#include "ConsoleColors.h"
#include "UTTest.h"


#define SUITE(suite_name) void suite_name (UTTestRunner* runner)
#define TEST(test_name) if (runner->_TestPass(test_name))


extern const char* __file__;
extern size_t __line__;

void* operator new(size_t size);
void operator delete(void* ptr) noexcept;

#define new (__file__=__FILE__,__line__=__LINE__) && 0 ? NULL : new
#define delete (__file__=__FILE__,__line__=__LINE__), delete


class UTTestRunner {
    
public:
    void RunSuite (std::string suiteName, std::function<void(UTTestRunner*)> suiteFunction);
    void IsTrue (bool expression, std::string name, std::string assertMessage);
    UTTestConfiguration* GetConfig();
    
    // Used by the testing framework itself
    bool _TestPass (std::string testName);
    
private:
    // TestPass handlers
    bool ScrapeHandler (std::string testName);
    bool RunHandler (std::string testName);
    
    // MiddleWare
    void StdExceptionCatcher(std::function<void(UTTestRunner*)> suiteFunction);
    void MemoryMonitor(std::function<void(UTTestRunner*)> suiteFunction);
    void SigSegCatcher(std::function<void(UTTestRunner*)> suiteFunction);
    void ThreadedTimeout(std::function<void(UTTestRunner*)> suiteFunction);
    
    friend void SigSegVHandler (int signum);
    
private:
    std::vector<UTTest> m_tests;
    std::function<bool(std::string)> m_handler;
    UTTest* m_activeTest;
    int m_activeTestIndex;
    int m_currentTestIndex;
};