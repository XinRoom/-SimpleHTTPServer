#pragma once

//Parse Header
int paul(const char *buf)
{
	if (buf == NULL) return -1;
	char head[1024], method[10], query[200], from[200], path[200];
	int i = 0, j = 0, k = 0, m = 0, n=0;
	int do_method = 1, do_path = 0, do_query = 0, do_http = 0, do_head = 0, do_from = 0;

	while (buf[i] != '\0')
	{
		//判断
		if (do_method && buf[i] == ' ') {
			do_method = 0; method[i] = '\0'; do_path = 1;
		}
		else if (do_path && (buf[i] == '?' || buf[i] == ' ')) {
			do_path = 0; path[j] = '\0';
			if (buf[i] == '?') do_query = 1; 
			if (buf[i] == ' ') { do_http = 1; query[k] = '\0'; }
		}
		else if (do_query && buf[i] == ' ') {
			do_query = 0; query[k] = '\0'; do_http = 1;
		}
		else if (do_http && buf[i] == '\r') {
			do_http=0; do_head = 1;
		}
		else if (do_head && buf[i] == '\r') {
			if (buf[i + 2] == '\r') { 
				do_head = 0; head[m] = '\0';
				do_from = 1;
				if (buf[i + 4] == '\0') {
					do_from = 0; from[n] = '\0';
				}
			}
			else { head[m] = '\n'; m++; } //转换分隔符
		}
		else if (do_from ) {
			if (buf[i + 1] == '\0') from[n + 1] = '\0';
		}

		//过掉不用存的信息
		if (buf[i] == ' ' || buf[i] == '\r' || buf[i] == '\n') {
			i++; continue;
		}
		//存储
		if (do_method) { if (i>9) return -1; method[i] = buf[i]; }
		else if (do_path) { if (j>199) return -1; path[j] = buf[i]; j++; }
		else if (do_query) { if (k>199) return -1; query[k] = buf[i]; k++; }
		else if (do_head && buf[i] != '?') { if (m>1020) return -1; head[m] = buf[i]; m++; }
		else if (do_from) { if (n>198) return -1; from[n] = buf[i]; n++; }
		
		i++;
	};
	_putenv_s("REQUEST_METHOD", method);
	_putenv_s("QUERY_STRING", query);
	_putenv_s("POST_DATA", from);
	_putenv_s("URL_PATH", path);
	//printf("%s\n%s\n%s\n%s\n%s\n",head, method, query,  from, path);
	return 0;
}
