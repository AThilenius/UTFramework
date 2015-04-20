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
    
private:
    std::vector<UTTest> m_tests;
    std::function<bool(std::string)> m_handler;
    UTTest* m_activeTest;
    int m_activeTestIndex;
    int m_currentTestIndex;
};