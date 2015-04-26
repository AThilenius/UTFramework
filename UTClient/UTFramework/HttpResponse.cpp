//
//  HttpResponse.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/22/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "HttpResponse.h"


HttpResponse::HttpResponse() {
    
}

bool HttpResponse::FromHtml(std::string html) {
    // TODO: Parse out header
    Content = html;
    return true;
}