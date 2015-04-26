//
//  HttpRequest.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/21/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "HttpRequest.h"

#include <iostream>


HttpRequest::HttpRequest(HttpMethod method, std::string route) :
m_header(method, route) {
    
}


HttpResponse HttpRequest::Post (std::string ipAddress, std::string route) {
    
}

HttpRequestPtr HttpRequest::FromIp(std::string ipAddress, HttpMethod method, std::string route) {
    HttpRequest* req = new HttpRequest(method, route);
    req->m_ipAddress = ipAddress;
    return HttpRequestPtr(req);
}

bool HttpRequest::GetResponse (HttpResponse* httpResponse) {
    m_socket.Connect(m_ipAddress, 3000);
    std::string html = m_header.ToHtml(Content);
    
    std::cout << "Sending: " << std::endl << html << std::endl;
    
    if (!m_socket.Write(html) {
        std::cout << "Failed to connect to server." << std::endl;
        return false;
    } else {
        // Wait for response
        Socket::TcpMessagePtr response = m_socket.Read();
        
        // Null terminate
        char* cStr = new char [response->Count + 1];
        memcpy(cStr, response->Data, response->Count);
        cStr[response->Count] = 0;
        std::string responseHtml (cStr);
        delete cStr;
        
        return httpResponse->FromHtml(responseHtml);
    }
}