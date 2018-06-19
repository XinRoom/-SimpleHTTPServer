#pragma once
#pragma warning(disable:4996)
#include <Winsock2.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#pragma comment(lib,"Ws2_32.lib")
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "env.h"
#include "cgi.h"
#include "header.h"

DWORD WINAPI SimpleHTTPServer(LPVOID lparam);