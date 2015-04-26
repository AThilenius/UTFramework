//
//  TcpSocket.h
//  Anvil
//
//  Created by Alec Thilenius on 11/14/14.
//  Copyright (c) 2014 Alec Thilenius. All rights reserved.
//
#pragma once

#include <memory>
#include <string>


typedef uint8_t UInt8;
typedef int8_t Int8;
typedef int16_t Int16;
typedef uint16_t UInt16;
typedef int32_t Int32;
typedef uint32_t UInt32;
typedef int64_t Int64;
typedef uint64_t UInt64;


namespace Socket {
    
    
typedef std::shared_ptr<std::string> StringPtr;

    
class TcpSocket {
public:
    TcpSocket();
    ~TcpSocket();

    bool Connect(std::string ipAddress, UInt16 port);
    StringPtr Read();
    bool Write(StringPtr data);
    
private:
    bool ReadCompleate(int socket, char* buffer, UInt32 count);
    bool ReadToBlankline(int socket, StringPtr buffer);

private:
	int m_socketHandle;
};

    
} // namespace Socket