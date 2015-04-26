//
//  HttpRequest.cpp
//  UTFramework
//
//  Created by Alec Thilenius on 4/21/15.
//  Copyright (c) 2015 Thilenius. All rights reserved.
//
#include "HttpRequest.h"

#include <iostream>
#include <map>
#include <sstream>

#include "TcpSocket.h"


HttpRequest::HttpRequest() {
}


HttpResponsePtr HttpRequest::Post (std::string ipAddress, std::string route, std::string content) {
    // Try to connect the socket
    Socket::TcpSocket socket;
    if (!socket.Connect(ipAddress, 3000)) {
        std::cout << "Failed to establish a connection with the UTServer." << std::endl;
        return HttpResponsePtr(nullptr);
    }
    
    // Build out the HTML request
    std::map<std::string, std::string> headers;
    std::stringstream htmlStream;
    
    headers["Accept"] = "application/json";
    headers["Accept-Charset"] = "utf-8";
    headers["Connection"] = "close";
    headers["Content-Length"] = std::to_string(content.length());
    headers["Content-Type"] = "application/json";
    
    htmlStream << "POST " << route << " HTTP/1.1" << "\r\n";
    htmlStream << "Host: " << ipAddress << "\r\n";
    
    for (auto kvp : headers) {
        htmlStream << kvp.first << ": " << kvp.second << "\r\n";
    }
    
    htmlStream << "\r\n";
    htmlStream << content;
    
    // Send the request
    if (!socket.Write(htmlStream.str())) {
        std::cout << "Failed to send request to UTServer." << std::endl;
        return HttpResponsePtr(nullptr);
    }
    
    // Get Response
    std::string response = socket.Read();
    
    return HttpResponse::FromHtml(response);
}
