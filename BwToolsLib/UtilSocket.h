//================================================================================
// BOOST SOFTWARE LICENSE
//
// Copyright 2020 BitWise Laboratories Inc.
// Author.......Jim Waschura
// Contact......info@bitwiselabs.com
//
//Permission is hereby granted, free of charge, to any person or organization
//obtaining a copy of the software and accompanying documentation covered by
//this license (the "Software") to use, reproduce, display, distribute,
//execute, and transmit the Software, and to prepare derivative works of the
//Software, and to permit third-parties to whom the Software is furnished to
//do so, all subject to the following:
//
//The copyright notices in the Software and this entire statement, including
//the above license grant, this restriction and the following disclaimer,
//must be included in all copies of the Software, in whole or in part, and
//all derivative works of the Software, unless such copies or derivative
//works are solely in the form of machine-executable object code generated by
//a source language processor.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.
//================================================================================

#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,  "ws2_32.lib") 

class UtilSocket
{

public:

    UtilSocket(int domain);
    UtilSocket(SOCKET socknum, sockaddr_in addrinet);
    virtual ~UtilSocket();

    bool getDestroying() { return m_Destroying; }
    bool getIsBlocking() { return m_IsBlocking; }

    void BindToPort(int port);
    void Listen(int queueLen);
    UtilSocket* Accept();
    void Connect(const char* address, int port);
    void SetSocketOption(int optname, const void* optval, int optlen);
    void GetSocketOption(int optname, void* optval, int* optlen);
    bool SetBlocking(bool newValue); /* returns if was blocking */
    int Receive(char* buffer, int bufferSize);
    void Send(char* buff, int len);

    void Send(const char* buff, int len) { Send((char*)buff, len); }
    void Send(const char* message) { Send((char*)message); }
    void Send(char* message) { Send(message, (int)strlen(message)); }
    void Send(char thechar) { Send(&thechar, 1); }

protected:
    SOCKET getSock() { return m_Sock; }

private:

    SOCKET m_Sock;
    bool m_Destroying;
    int m_Domain;
    sockaddr_in m_SockAddrInet;
    bool m_IsBlocking;

};

/* EOF */