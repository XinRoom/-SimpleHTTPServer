#pragma once

inline void
response_400(SOCKET sAccept) {
	const unsigned char * estr =
		"HTTP/1.1 400 BAD REQUEST\r\n"
		"Server: XinRoom_http_0.1\r\n"
		"Content-Type: text/html;charset=utf-8\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<p>你的请求有问题,请检查语法!</p>";

	send(sAccept, estr, strlen(estr), 0);
}

inline void
response_404(SOCKET sAccept) {
	unsigned char estr[BUFSIZ];
	sprintf(estr,
		"HTTP/1.1 404 NOT FOUND\r\n"
		"Server: XinRoom_http_0.1\r\n"
		"Content-Type: text/html;charset=utf-8\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html>"
		"<head><title>404 Not Found</title></head>\r\n"
		"<body style = 'text-align: center;'><h1>404 Not Found</h1><hr><p>GET %s</p></body>"
		"</html>"
		, getenv("URL_PATH"));

	send(sAccept, estr, strlen(estr), 0);
}

inline void
response_501(SOCKET sAccept) {
	const unsigned char * estr =
		"HTTP/1.1 501 Method Not Implemented\r\n"
		"Server: XinRoom_http_0.1\r\n"
		"Content-Type: text/html;charset=utf-8\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html>"
		"<head><title>小伙子不要乱请求</title></head>\r\n"
		"<body><p>too young too simple, 年轻人别总想弄出个大新闻.</p></body>"
		"</html>";

	send(sAccept, estr, strlen(estr), 0);
}

inline void
response_500(SOCKET sAccept) {
	const unsigned char * estr =
		"HTTP/1.1 500 Internal Server Error\r\n"
		"Server: XinRoom_http_0.1\r\n"
		"Content-Type: text/html;charset=utf-8\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html>"
		"<head><title>Sorry </title></head>\r\n"
		"<body><p>最近有点方了!</p></body>"
		"</html>";

	send(sAccept, estr, strlen(estr), 0);
}

inline void
response_200(SOCKET sAccept, const char *type) {
	unsigned char estr[BUFSIZ];
	sprintf(estr, 
		"HTTP/1.1 200 OK\r\n"
		"Server: XinRoom_http_0.1\r\n"
		"Content-Type: %s\r\n"
		"Connection: keep-alive\r\n"
		"Keep-Alive: timeout=20\r\n"
		"\r\n"
		, type);

	send(sAccept, estr, strlen(estr), 0);
}