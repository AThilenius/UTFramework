//
//  TcpSocket.cpp
//  Anvil
//
//  Created by Alec Thilenius on 11/14/14.
//  Copyright (c) 2014 Alec Thilenius. All rights reserved.
//
#include "TcpSocket.h"

#include <string>
#include <stdlib.h>
#include <sstream>
#include <iostream>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <stdio.h>
	#include <stdlib.h>
	#pragma comment(lib,"Ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
#endif


namespace Socket {
    
    
    Int64 ReadSocket (int socket, char* buffer, UInt64 count) {
#ifdef _WIN32
        return recv(socket, buffer, count, 0);
#else
        return read(socket, buffer, count);
#endif
    }
    
    Int64 WriteSocket (int socket, const char* buffer, UInt64 count) {
#ifdef _WIN32
		return send(socket, buffer, count, 0);
#else
        return write(socket, buffer, count);
#endif
    }
    
    
TcpSocket::TcpSocket() :
    m_socketHandle(-1) {
    
}

TcpSocket::~TcpSocket() {
    
}

bool TcpSocket::Connect(std::string ipAddressStr, UInt16 port) {
    
#ifdef _WIN32
	// Fucking Windows...
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
        std::cout << "Winsock isn't supported on your system." std::endl;
		return false;
	}
#endif

    // Setup address
    sockaddr_in serverAddress;
    inet_pton(AF_INET, ipAddressStr.c_str(), &(serverAddress.sin_addr));
    serverAddress.sin_port = htons(port);
    serverAddress.sin_family = AF_INET;
    
    m_socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socketHandle < 0) {
        return false;
    }
    
    if (connect(m_socketHandle, (sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        return false;
    }
    
    return true;
}

std::string TcpSocket::Read() {
    std::string response;
    
    // Read until the socket closes
    char buffer [1024];
    while (ReadSocket(m_socketHandle, buffer, 1024) > 0) {
        response += buffer;
    }
    
    return response;
}
    
bool TcpSocket::Write(std::string data) {
        // Write Complete
        UInt32 writeCount = 0;
        while (writeCount < data.size()) {
            Int64 n = WriteSocket(m_socketHandle, &(data[writeCount]), data.size() - writeCount);
            writeCount += n;
            if (n < 0) {
                // Socket was forcibly closed
                return false;
            }
        }
        return true;
    }



} // namespace Socket