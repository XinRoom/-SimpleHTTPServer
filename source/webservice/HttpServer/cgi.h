#pragma once

int _Cgi(const char * cmd, char *pRetMsg)
{
	char buffer[MAX_BUFFER] = { 0 };
	if (!cmd)
		return -1;

	FILE * fp = _popen(cmd, "r");
	if (!fp)
		return -2;

	while (!feof(fp))
	{
		if (fgets(buffer, MAX_BUFFER, fp) != NULL)
			strcat_s(pRetMsg, MAX_BUFFER, buffer);
	}
	_pclose(fp);
	//pRetMsg[strlen(pRetMsg)+3] = '\0';
	return 0;
}
