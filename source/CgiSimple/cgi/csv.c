#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>


int hex2dec(char c)
{
	if ('0' <= c && c <= '9')
	{
		return c - '0';
	}
	else if ('a' <= c && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if ('A' <= c && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else
	{
		return -1;
	}
}

char dec2hex(short int c)
{
	if (0 <= c && c <= 9)
	{
		return c + '0';
	}
	else if (10 <= c && c <= 15)
	{
		return c + 'A' - 10;
	}
	else
	{
		return -1;
	}
}

// 解码url
void urldecode(char url[])
{
	int i = 0;
	int len = strlen(url);
	int res_len = 0;
	char res[200];
	for (i = 0; i < len; ++i)
	{
		char c = url[i];
		if (c != '%')
		{
			res[res_len++] = c;
		}
		else
		{
			char c1 = url[++i];
			char c0 = url[++i];
			int num = 0;
			num = hex2dec(c1) * 16 + hex2dec(c0);
			res[res_len++] = num;
		}
	}
	res[res_len] = '\0';
	strcpy(url, res);
}


void main() {

	//putenv("REQUEST_METHOD=GET");
	//putenv("POST_DATA=name=%E9%A9%AC&age=1&sex=2&tell=3");
	char *pszMethod;
	pszMethod = getenv("REQUEST_METHOD");
	//system("chcp 65001");
	if (strcmp(pszMethod, "GET") == 0)  // get  
	{
		printf(
			"HTTP/1.1 200 OK\r\n"
			"Server: XinRoom_http_0.1\r\n"
			"Content-Type: application/json; charset=utf-8\r\n"
			"Connection: keep-alive\r\n"
			"\r\n"
		);

		// 读取环境变量来获取数据  
		FILE *fp = NULL;
		char *line, *record;
		char buffer[1024];
		if ((fp = fopen(".\\data\\data.txt", "at+")) != NULL)
		{
			//fseek(fp, 170L, SEEK_SET);  //定位到第二行，每个英文字符大小为1  
			char delims[] = ",";
			char *result = NULL, *p;
			int j = 0;
			fgets(buffer, sizeof(buffer), fp);
			printf("[");
			if ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)//当没有读取到文件末尾时循环继续  
			{
				if ((p = strchr(line, '\n')) != NULL)
					*p = '\0';//手动将\n位置处的值变为0  
				record = strtok(line, ",");
				printf("[");
				if (record != NULL)//读取每一行的数据  
				{
					printf("\"%s\"", record);
					record = strtok(NULL, ",");
					j++;
				}
				while (record != NULL)//读取每一行的数据  
				{
					printf(",\"%s\"", record);;
					record = strtok(NULL, ",");
					j++;
				}
				printf("]");
				j = 0;

			}

			while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)//当没有读取到文件末尾时循环继续  
			{
				if ((p = strchr(line, '\n')) != NULL)
					*p = '\0';//手动将\n位置处的值变为0  
				record = strtok(line, ",");
				printf(",[");
				if (record != NULL)//读取每一行的数据  
				{
					printf("\"%s\"", record);
					record = strtok(NULL, ",");
					j++;
				}
				while (record != NULL)//读取每一行的数据  
				{
					printf(",\"%s\"", record);//将读取到的每一个数据打印出来  
											  //if (j == 4)  //只需读取前9列  
											  //	break;
					record = strtok(NULL, ",");
					j++;
				}
				printf("]");
				//printf("\n");
				j = 0;

			}
			printf("]");
			fclose(fp);
			fp = NULL;
		}
	}
	else  // post  
	{
		printf(
			"HTTP/1.1 302 Temporarily Moved\r\n"
			"Server: XinRoom_http_0.1\r\n"
			"Location: /info.html\r\n"
			"Connection: keep-alive\r\n"
			"\r\n"
		);
		// 读取环境变量来获取数据  
		FILE *fp = NULL;
		char *data = getenv("POST_DATA") , *record1,*record2;
		char *buffer[128],pull[128];
		char *name, *age, *sex, *tell;
		int i = 0;
		while (data[i] != '\0') {
			if (data[i] == '+') data[i] = ' ';
			i++;
		}
		urldecode(data);
		if ((fp = fopen(".\\data\\data.txt", "at+")) != NULL)
		{
			record1 = strtok(data, "&");
			int i = 0;
			while (record1 != NULL)//读取每一行的数据  
			{
				buffer[i] = record1;
				i++;
				record1 = strtok(NULL, "&");
			}

			for (int j = 0; j < i;j++) {
				record2 = strtok(buffer[j], "=");
				if (strcmp(record2, "name") == 0) {
					record2 = strtok(NULL, "=");
					name = record2;
				}
				else if (strcmp(record2, "age") == 0) {
					record2 = strtok(NULL, "=");
					age = record2;
				}
				else if (strcmp(record2, "sex") == 0) {
					record2 = strtok(NULL, "=");
					sex = record2;
				}
				else if (strcmp(record2, "tell") == 0) {
					record2 = strtok(NULL, "=");
					tell = record2;
				}
			}
			sprintf(pull,"%s,%s,%s,%s\n",name,age,sex,tell);
			//printf(pull);
			fputs(pull, fp);
			//fseek(fp, 170L, SEEK_SET);  //定位到第二行，每个英文字符大小为1  

			fclose(fp);
			fp = NULL;
		}
	}

	//getchar();
}
