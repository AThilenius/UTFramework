//
//  HttpRequest.h
//  UTFramework
//
//  Created by Alec Thilenius on 4/21/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#pragma once

#include <string>

#include "HttpResponse.h"


class HttpRequest {
public:
    static HttpResponsePtr Post (std::string ipAddress, std::string route, std::string content);
    
private:
    HttpRequest();
};