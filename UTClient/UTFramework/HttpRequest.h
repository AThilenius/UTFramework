//
//  HttpRequest.h
//  UTFramework
//
//  Created by Alec Thilenius on 4/21/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#pragma once

#include <string>
#include <memory>

#include "HttpHeader.h"
#include "HttpResponse.h"
#include "TcpSocket.h"


class HttpRequest;
typedef std::shared_ptr<HttpRequest> HttpRequestPtr;


class HttpRequest {
public:
    static HttpResponse Post (std::string ipAddress, std::string route);
    static HttpRequestPtr FromIp (std::string ipAddress, HttpMethod method, std::string route);
    bool GetResponse (HttpResponse* response);
    
private:
    HttpRequest(HttpMethod method, std::string route);
    
public:
    std::string Content;
    
private:
    std::string m_ipAddress;
    HttpHeader m_header;
    Socket::TcpSocket m_socket;
};