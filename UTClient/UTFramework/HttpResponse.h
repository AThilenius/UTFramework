//
//  HttpResponse.h
//  UTFramework
//
//  Created by Alec Thilenius on 4/22/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#pragma once

#include <string>


class HttpResponse {
public:
    HttpResponse();
    bool FromHtml(std::string html);
    
public:
    std::string Content;
};