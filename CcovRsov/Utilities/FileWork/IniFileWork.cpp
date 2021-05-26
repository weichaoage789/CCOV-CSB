#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IniFileWork.h"


namespace FileWork
{
	void Trim(char *src)
	{
		char *tmp = nullptr;
		int  len = 0;

		if (nullptr != src)
		{
			tmp = src;

			/*È¥³ý×ó²à¿Õ°××Ö·û*/
			while (isspace(*tmp))
			{
				tmp++;
			}

			/*È¥³ýÓÒ²à¿Õ°××Ö·û*/
			len = strlen(tmp);
			while ((len > 0) && (isspace(tmp[len - 1])))
			{
				len--;
			}
			tmp[len] = '\0';
		}

		return;
	}

	void IniReadValue(char* section, const char* key, char* val, const char* file)
	{
		FILE* fp;
		int i = 0;
		int lineContentLen = 0;
		int position = 0;
		char lineContent[LINE_CONTENT_MAX_LEN];

		fp = fopen(file, "r");
		if (fp == nullptr)
		{
			return;
		}
		while (feof(fp) == 0)
		{
			memset(lineContent, 0, LINE_CONTENT_MAX_LEN);
			fgets(lineContent, LINE_CONTENT_MAX_LEN, fp);
			if ((lineContent[0] == ';') || (lineContent[0] == '\0') || (lineContent[0] == '\r') || (lineContent[0] == '\n'))
			{
				continue;
			}

			//check section
			if (strncmp(lineContent, section, strlen(section)) == 0)
			{
				while (feof(fp) == 0)
				{
					memset(lineContent, 0, LINE_CONTENT_MAX_LEN);
					fgets(lineContent, LINE_CONTENT_MAX_LEN, fp);
					Trim(lineContent);
					//check key
					if (strncmp(lineContent, key, strlen(key)) == 0)
					{
						lineContentLen = strlen(lineContent);
						//find value
						for (i = strlen(key); i < lineContentLen; i++)
						{
							if (lineContent[i] == '=')
							{
								position = i + 1;
								break;
							}
						}
						if (i >= lineContentLen) break;
						strncpy(val, lineContent + position, strlen(lineContent + position));
						lineContentLen = strlen(val);
						for (i = 0; i < lineContentLen; i++)
						{
							if ((lineContent[i] == '\0') || (lineContent[i] == '\r') || (lineContent[i] == '\n'))
							{
								val[i] = '\0';
								break;
							}
						}
					}
					else if (lineContent[0] == '[')
					{
						break;
					}
				}
				break;
			}
		}

		fclose(fp);
		return;
	}

	bool ReadStringValue(const char* section, const char* key, char* val, const char* file)
	{
		char sect[SECTION_MAX_LEN];
		if (section == nullptr || key == nullptr || val == nullptr || file == nullptr)
		{
			return false;
		}

		memset(sect, 0, SECTION_MAX_LEN);
		sprintf(sect, "[%s]", section);
		IniReadValue(sect, key, val, file);

		return true;
	}

	int ReadIntValue(const char* section, const char* key, const char* file)
	{
		char strValue[STRVALUE_MAX_LEN];
		memset(strValue, '\0', STRVALUE_MAX_LEN);
		if (!ReadStringValue(section, key, strValue, file))
		{
			return 0;
		}
		return(atoi(strValue));
	}

	void IniWriteValue(const char* section, const char* key, const char* val, const char* file)
	{
		FILE* fp;
		int i = 0, n = 0, err = 0;
		int lineContentLen = 0;
		int position = 0;
		char lineContent[LINE_CONTENT_MAX_LEN];
		char strWrite[LINE_CONTENT_MAX_LEN];
		bool bFoundSection = false;
		bool bFoundKey = false;

		memset(lineContent, '\0', LINE_CONTENT_MAX_LEN);
		memset(strWrite, '\0', LINE_CONTENT_MAX_LEN);
		n = sprintf(strWrite, "%s=%s\n", key, val);
		fp = fopen(file, "r+");
		if (fp == nullptr)
		{
			printf("%s: Open file %s failed.\n", __FILE__, file);
			return;
		}
		while (feof(fp) == 0)
		{
			memset(lineContent, 0, LINE_CONTENT_MAX_LEN);
			fgets(lineContent, LINE_CONTENT_MAX_LEN, fp);
			if ((lineContent[0] == ';') || (lineContent[0] == '\0') || (lineContent[0] == '\r') || (lineContent[0] == '\n'))
			{
				continue;
			}
			//check section
			if (strncmp(lineContent, section, strlen(section)) == 0)
			{
				bFoundSection = true;
				while (feof(fp) == 0)
				{
					memset(lineContent, 0, LINE_CONTENT_MAX_LEN);
					fgets(lineContent, LINE_CONTENT_MAX_LEN, fp);
					//check key
					if (strncmp(lineContent, key, strlen(key)) == 0)
					{
						bFoundKey = true;
						printf("%s: %s=%s\n", __func__, key, val);
						fseek(fp, (0 - strlen(lineContent)), SEEK_CUR);
						err = fputs(strWrite, fp);
						if (err < 0) { printf("%s err.\n", __func__); }
						break;
					}
					else if (lineContent[0] == '[')
					{
						break;
					}
				}
				break;
			}
		}

		if (!bFoundSection) { printf("No section = %s\n", section); }
		else if (!bFoundKey) { printf("No key = %s\n", key); }
		fclose(fp);
	}

	bool writeStringVlaue(const char* section, const char* key, const char* val, const char* file)
	{
		char sect[SECTION_MAX_LEN];
		if (section == nullptr || key == nullptr || val == nullptr || file == nullptr)
		{
			printf("%s: input parameter(s) is NULL!\n", __func__);
			return false;
		}
		memset(sect, '\0', SECTION_MAX_LEN);
		sprintf(sect, "[%s]", section);
		IniWriteValue(sect, key, val, file);

		return true;
	}

	bool writeIntValue(const char* section, const char* key, const int val, const char* file)
	{
		char strValue[STRVALUE_MAX_LEN];
		memset(strValue, '\0', STRVALUE_MAX_LEN);
		sprintf(strValue, "%-4d", val);

		return writeStringVlaue(section, key, strValue, file);
	}
}