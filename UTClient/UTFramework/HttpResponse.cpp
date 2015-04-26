//
//  HttpResponse.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/22/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "HttpResponse.h"

#include <iostream>
#include <sstream>
#include <vector>


HttpResponse::HttpResponse() {
    
}

HttpResponsePtr HttpResponse::FromHtml(std::string html) {
    // Just pull the header text out
    std::string header;
    std::string content;
    size_t index = html.find("\n\n");
    
    if (index == std::string::npos) {
        index = html.find("\r\n\r\n");
        if (index == std::string::npos) {
            std::cout << "Failed to parse HTML header from html: " << html << std::endl;
            return HttpResponsePtr(nullptr);
        } else {
            // Found \r\n\r\n
            header = html.substr(0, index);
            content = html.substr(index + 4, html.size() - 1);
        }
    } else {
        // Found \n\n
        header = html.substr(0, index);
        content = html.substr(index + 2, html.size() - 1);
    }
    
    HttpResponse* response = new HttpResponse();
    response->HeaderText = header;
    response->Content = content;
    
    return HttpResponsePtr(response);
}