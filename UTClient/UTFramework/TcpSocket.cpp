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

StringPtr TcpSocket::Read() {
    // TODO: This is a horrifically inefficient way of doing this. Doesn't matter for this use case but still.
    // Right now it's pulling 1 byte at a time. The better way to do this is to read into a PERSISTENT buffer and
    // scan that. But remaining data (past the end) must be preserved to parse the next message
    
    // First read to blank line
    StringPtr buffer = StringPtr(new std::string());
    if (!ReadToBlankline(m_socketHandle, buffer)) {
        std::cout << "Failed to read header from TCP socket" << std::endl;
        return StringPtr(nullptr);
    }
    
    
    
    return buffer;
}

bool TcpSocket::Write(StringPtr data) {
    // Write Complete
    UInt32 writeCount = 0;
    while (writeCount < data->size()) {
        Int64 n = WriteSocket(m_socketHandle, &((*data)[writeCount]), data->size() - writeCount);
        writeCount += n;
        if (n < 0) {
            // Socket was forcibly closed
            return false;
        }
    }
    return true;
}
    
bool ReadCompleate(int socket, char* buffer, UInt32 count) {
    UInt32 readCount = 0;
    while (readCount < count) {
        Int64 n = ReadSocket(socket, &buffer[readCount], count - readCount);
        readCount += n;
        if (n < 0) {
            // Socket was forcibly closed
            return false;
        }
    }
    return true;
 
}

bool ReadToBlankline(int socket, StringPtr buffer) {
    char input = 0;
    
    while (true) {
        Int64 n = ReadSocket(socket, &input, 1);
        if (n != 1) {
            // Socket was forcibly closed
            return false;
        }
        
        (*buffer) += input;
    
        // Check for blank line
        if ( ((*buffer)[buffer->length() - 1] == '\n' && (*buffer)[buffer->length() - 2] == '\n') ||
             ((*buffer)[buffer->length() - 1] == '\n' && (*buffer)[buffer->length() - 2] == '\r' &&
              (*buffer)[buffer->length() - 3] == '\n' && (*buffer)[buffer->length() - 4] == '\r') ) {
            // Finished, let's make like a tree and get the fu... fuuu.. fuck out of here
            return true;
        }
        
    }
}


} // namespace Socket