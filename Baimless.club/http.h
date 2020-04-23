#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <winsock.h>
#include "Singleton.h"
using namespace std;

class http_request : public Singleton<http_request>
{
public:
    HINSTANCE hInst;
    WSADATA wsaData;
    void mParseUrl(char* mUrl, string& serverName, string& filepath, string& filename);
    SOCKET connectToServer(char* szServerName, WORD portNum);
    int getHeaderLength(char* content);
    char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut);
    void api(char* user,char* pass);
    bool isonline();
};
