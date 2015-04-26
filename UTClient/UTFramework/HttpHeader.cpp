//
//  HttpHeader.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/22/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "HttpHeader.h"

#include <sstream>


HttpHeader::HttpHeader (HttpMethod method, std::string route) :
    Method(method),
    Route(route) {
    // Set default headers
    Headers["User-Agent"] = "Mozilla/5.0";
    Headers["Accept"] = "application/json";
    Headers["Accept-Charset"] = "utf-8";
    //Headers["Keep-Alive"] = "300";
    Headers["Connection"] = "close";
    Headers["Content-Type"] = "application/json";
}

std::string HttpHeader::ToHtml(std::string content) {
    std::stringstream stringStream;
    
    // Set Content length
    Headers["Content-Length"] = std::to_string(content.length());
    
    stringStream << (Method == HttpMethod::GET ? "GET " : "POST ") << Route << " HTTP/1.1" << "\r\n";
    
    // Now loop through the Header Map and add in each
    for (auto kvp : Headers) {
        stringStream << kvp.first << ": " << kvp.second << "\r\n";
    }
    
    // Add a new return line to denote data region and add content
    stringStream << "\r\n";
    stringStream << content;
    
    return stringStream.str();
}