//
//  main.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/16/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include <iostream>

#include "UTTestRunner.h"
#include "HttpRequest.h"
#include "HttpResponse.h"


//SUITE (SuiteOne) {
//    
//    TEST ("True Check") {
//        runner->IsTrue(true, "Should be True", "Should NOT show up!");
//        runner->IsTrue(true, "Should also be True", "Should NOT show up!");
//    }
//    
//    TEST ("False Check") {
//        runner->IsTrue(false, "Should be False", "Should Be Shown");
//        runner->IsTrue(false, "Should also be False", "Should Also Be Shown");
//    }
//
//    TEST ("Seg Fault Check") {
//        runner->IsTrue(true, "Should be True", "Should NOT show up!");
//        runner->IsTrue(false, "Should be False", "Should Be Shown");
//        runner->IsTrue(true, "Should Seg-Fault After This", "Should NOT show up!");
//        //int foo = *((int*)0);
//        //std::cout << foo;
//    }
//    
//    TEST ("Infinite Loops Check") {
//        runner->IsTrue(true, "Should be True", "Should NOT show up!");
//        runner->IsTrue(false, "Should be False", "Should Be Shown");
//        runner->IsTrue(true, "Should Timout After This", "Should NOT show up!");
//        while (true) { }
//    }
//    
//    TEST ("Memory Leak Check Pass") {
//        runner->IsTrue(true, "Should be True", "Should NOT show up!");
//        runner->IsTrue(false, "Should be False", "Should Be Shown");
//        int * foo = new int[1];
//        delete foo;
//    }
//    
//    TEST ("Memory Leak Check Fail") {
//        runner->IsTrue(true, "Should be True", "Should NOT show up!");
//        runner->IsTrue(false, "Should be False", "Should Be Shown");
//        int * foo = new int[1];
//    }
//    
//    runner->GetConfig()->MinMemory = 8;
//    TEST ("Min Memory Check") {
//        runner->IsTrue(true, "Should be True", "Should NOT show up!");
//        runner->IsTrue(false, "Should be False", "Should Be Shown");
//        int * foo = new int[1];
//        delete foo;
//    }
//    
//    runner->GetConfig()->MaxMemory = 8;
//    TEST ("Max Memory Check") {
//        runner->IsTrue(true, "Should be True", "Should NOT show up!");
//        runner->IsTrue(false, "Should be False", "Should Be Shown");
//        int * foo = new int[64];
//        delete foo;
//    }
//}

int main(int argc, const char * argv[]) {
//    UTTestRunner runner;
//    runner.RunSuite("Test Name", &SuiteOne);
    
    HttpResponsePtr response = HttpRequest::Post("127.0.0.1", "/", "{\"entity_name\" : \"TestName\"}");
    std::cout << "Got back:\n" << response->Content << std::endl;
}
