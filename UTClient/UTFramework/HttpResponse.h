//
//  HttpResponse.h
//  UTFramework
//
//  Created by Alec Thilenius on 4/22/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#pragma once

#include <string>
#include <memory>


class HttpResponse;
typedef std::shared_ptr<HttpResponse> HttpResponsePtr;


class HttpResponse {
public:
    static HttpResponsePtr FromHtml(std::string html);
    
private:
    HttpResponse();
    
public:
    std::string HeaderText;
    std::string Content;
};