#include "httpserver.h"
#include "response_code.h"


//获取文件类型
int get_filetype(unsigned char *filename)
{
	//unsigned char filetype[50];
	int lSize = strlen(filename);
	if (strstr(filename, ".html") >= filename + lSize - 5)  //判断结尾是否符合
		strcpy(filename, "text/html;charset=utf-8\r\nCache-control: max-age=30");
	else if (strstr(filename, ".gif") >= filename + lSize - 4)
		strcpy(filename, "image/gif\r\nCache-control: max-age=3600");
	else if (strstr(filename, ".png") >= filename + lSize-4)
		strcpy(filename, "image/png\r\nCache-control: max-age=3600");
	else if (strstr(filename, ".jpg") >= filename + lSize-4)
		strcpy(filename, "image/jpeg\r\nCache-control: max-age=3600");
	else if (strstr(filename, ".js") >= filename + lSize-3)
		strcpy(filename, "application/x-javascript\r\nCache-control: max-age=360");
	else if (strstr(filename, ".css") >= filename+lSize-4)
		strcpy(filename, "text/css\r\nCache-control: max-age=360");
	else if (strstr(filename, ".ico") >= filename+lSize-4)
		strcpy(filename, "image/x-icon\r\nCache-control: max-age=3600"); 
	else if (strstr(filename, ".woff2") >= filename+lSize-6)
		strcpy(filename, "application/octet-stream\r\nCache-control: max-age=3600");
	else if (strstr(filename, ".map") >= filename+lSize-4)
		strcpy(filename, "text\r\nCache-control: max-age=3600");
	else
		strcpy(filename, "text/plain;charset=utf-8");
	return 0;
}

void
response_file(SOCKET sAccept, unsigned char * path) {
	FILE * txt = fopen(path, "rb");

	// 如果文件解析错误, 给它个404
	if (NULL == txt)
		response_404(sAccept);
	else {
		//发送给200的报文头过去, 并发送文件内容过去
		get_filetype(path);
		response_200(sAccept, path);

		long lBegin, lEnd;
		unsigned char *p;
		int  nSize;

		lBegin = ftell(txt);
		fseek(txt, 0L, SEEK_END);
		lEnd = ftell(txt);
		nSize = lEnd - lBegin;
		p = (unsigned char *)malloc(nSize * sizeof(unsigned char));
		fseek(txt, 0L, SEEK_SET);
		fread(p, sizeof(unsigned char), nSize, txt);
		fclose(txt);
		send(sAccept, p, nSize, 0);
	}
}

void
response_cgi(SOCKET sAccept, const unsigned char * path) {
	
	FILE * txt = fopen(path, "rb");
	if (NULL == txt) {
		response_404(sAccept);
		return;
	}
	fclose(txt);
	unsigned char buf[MAX_BUFFER << 1] = {0};
	_Cgi(path, buf);
	send(sAccept, buf, strlen(buf), 0);
}


DWORD WINAPI SimpleHTTPServer(LPVOID lparam)
{
	SOCKET sAccept = (SOCKET)(LPVOID)lparam;
	unsigned char recv_buf[BUF_LENGTH] = {0};
	unsigned char method[MIN_BUF];
	unsigned char wpath[MAX_PATH];
	unsigned char path[MAX_PATH];
	char *env_REMOTE_ADDR;
	char *env_URL_PATH;
	size_t requiredSize;
	int i=0, j=0;

	time_t t;
	t = time(NULL);
	char *src = ctime(&t);
	unsigned char dest[100];
	strncpy(dest, src, strlen(src) - 1);  //不拷贝换行
	dest[strlen(src) - 1] = '\0';   //加结束符'\0'

	//memset(recv_buf, 0, sizeof(recv_buf));
	if (recv(sAccept, recv_buf, sizeof(recv_buf), 0) == SOCKET_ERROR)   //接收错误
	{
		closesocket(sAccept);
		printf("recv() Failed:%d\n", WSAGetLastError());
		return USER_ERROR;
	}

	if (recv_buf[0] == '\0') {
		response_400(sAccept);
		closesocket(sAccept); return USER_ERROR;
	}
		

	// 取出METHOD
	while (!(' ' == recv_buf[j]) && (i < sizeof(method) - 1))
	{
		method[i] = recv_buf[j];
		i++; j++;
	}
	method[i] = '\0';
	if (stricmp(method, "GET") && stricmp(method, "POST"))
	{
		closesocket(sAccept); //释放连接套接字，结束与该客户的通信
		getenv_s(&requiredSize, NULL, 0, "REMOTE_ADDR");
		env_REMOTE_ADDR = (char*)malloc(requiredSize * sizeof(char));
		getenv_s(&requiredSize, env_REMOTE_ADDR, requiredSize, "REMOTE_ADDR");
		printf("%s - %s %s Deny Method!\r\n", dest, env_REMOTE_ADDR, method);
		return USER_ERROR;
	};

	if (paul(recv_buf) == -1) {
		response_400(sAccept);
		closesocket(sAccept);
		return 0;
	}

	i = 0, j = j + 2;
	//对url解析
	int path_do = 1;
	while ( ' ' != recv_buf[j] && '?' != recv_buf[j] && i < sizeof(recv_buf) - 1)
	{
		if (recv_buf[j] == '/') wpath[i] = '\\';
		else wpath[i] = recv_buf[j];

		i++; j++;
	}
	wpath[i] = '\0';
	//缓存环境变量
	getenv_s(&requiredSize, NULL, 0, "REMOTE_ADDR");
	env_REMOTE_ADDR = (char*)malloc(requiredSize * sizeof(char));
	getenv_s(&requiredSize, env_REMOTE_ADDR, requiredSize, "REMOTE_ADDR");

	getenv_s(&requiredSize, NULL, 0, "URL_PATH");
	env_URL_PATH = (char*)malloc(requiredSize * sizeof(char));
	getenv_s(&requiredSize, env_URL_PATH, requiredSize, "URL_PATH");

	printf("%s - %s %s %s\r\n", dest, env_REMOTE_ADDR, method, env_URL_PATH);

	strcpy(path, "www\\");
	strcat(path,wpath);

	if(strcmp(method, "GET") == 0){
		if (recv_buf[j-1] == '/') strcat(path, "index.html");
		if (path == strstr(path, "www\\cgi_bin")) response_cgi(sAccept, path);
		else response_file(sAccept, path);
	}
	else if (strcmp(method, "POST") == 0) {
		if (path == strstr(path, "www\\cgi_bin")) response_cgi(sAccept, path);
		else response_400(sAccept);
	}

	closesocket(sAccept);
	return 0;
}


int main()
{
	system("title Httpd - 0.1 [XinRoom]");
	system("chcp 65001");
	WSADATA wsaData;
	SOCKET sListen, sAccept;        //服务器监听套接字 和 连接套接字
	int serverport = DEFAULT_PORT;  //服务器端口号
	struct sockaddr_in ser, cli;    //服务器地址 和 客户端地址
	int iLen;

	//加载协议栈
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock.\n");
		return USER_ERROR;
	}

	//创建监听套接字，用于监听客户请求IPPROTO_TCP
	sListen = socket(AF_INET, SOCK_STREAM, 0);
	//sListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sListen == INVALID_SOCKET)
	{
		printf("socket() Failed:%d\n", WSAGetLastError());
		return USER_ERROR;
	}

	//创建服务器地址：IP+端口号
	ser.sin_family = AF_INET;
	ser.sin_port = htons(serverport);          //服务器端口号
	ser.sin_addr.s_addr = htonl(INADDR_ANY);   //服务器IP地址
	_putenv_s("SERVER_NAME", SERVER);

	//绑定监听套接字和服务器地址
	if (bind(sListen, (LPSOCKADDR)&ser, sizeof(ser)) == SOCKET_ERROR)
	{
		printf("blind() Failed:%d\n", WSAGetLastError());
		return USER_ERROR;
	}
	int iSockAttrOn = 1, keepalive = 1; // 开启keepalive属性
	setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, (void *)&iSockAttrOn, sizeof(iSockAttrOn));
	
	
	//通过监听套接字进行监听
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen() Failed:%d\n", WSAGetLastError());
		return USER_ERROR;
	}
	printf("httpd running on port %d\n", DEFAULT_PORT);

	DWORD ThreadID;
	while (1)  //循环等待客户的请求
	{
		//接受客户端的连接请求，返回与该客户建立的连接套接字
		iLen = sizeof(cli);
		sAccept = accept(sListen, (struct sockaddr*)&cli, &iLen);
		//setsockopt(sAccept, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive));
		_putenv_s("REMOTE_ADDR", inet_ntoa(cli.sin_addr));//客户端地址

		if (sAccept == INVALID_SOCKET)
		{
			printf("accept() Failed:%d\n", WSAGetLastError());
			break;
		}
		//创建线程接受浏览器请求
		CreateThread(NULL, 0, SimpleHTTPServer, (LPVOID)sAccept, 0, &ThreadID);
		//SimpleHTTPServer((LPVOID)sAccept);
	}
	closesocket(sListen);
	WSACleanup();
	return 0;
}
