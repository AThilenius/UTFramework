//
//  HttpHeader.h
//  UTFramework
//
//  Created by Alec Thilenius on 4/22/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#pragma once

#include <string>
#include <map>


enum class HttpMethod {
    POST,
    GET
};


class HttpHeader {
public:
    HttpHeader (HttpMethod method, std::string route);
    std::string ToHtml(std::string content);
    
public:
    HttpMethod Method;
    std::string Route;
    std::map<std::string, std::string> Headers;
};