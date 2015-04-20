//
//  ConsoleColors.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/20/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "ConsoleColors.h"

#define DISABLE_COLORS

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    #include <Windows.h>
#else

#endif

std::ostream& operator << (std::ostream& stream, ConsoleColor color) {
    
#if !defined(DISABLE_COLORS)
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    //Windows
    
    switch ( color ) {
        case White:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
            break;
            
        case Red:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_INTENSITY);
            break;
            
        case Yellow:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|FOREGROUND_RED
                                    |FOREGROUND_INTENSITY);
            break;
            
        case Green:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|FOREGROUND_INTENSITY);
            break;
            
        case Blue:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE|FOREGROUND_GREEN
                                    |FOREGROUND_INTENSITY);
            break;
    }
    
#else
    // Unix Based (OSX / Linux)
    
    switch ( color ) {
        case White:
            std::cout << "\033[0m";
            break;
            
        case Red:
            std::cout << "\033[31m";
            break;
            
        case Yellow:
            std::cout << "\033[33m";
            break;
            
        case Green:
            std::cout << "\033[32m";
            break;
            
        case Blue:
            std::cout << "\033[36m";
            break;
    }
#endif
#endif
    return stream;
}