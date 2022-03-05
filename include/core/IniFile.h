#pragma once
#include <string>

class IniFile
{
public:
	IniFile();
	IniFile(std::string FileName);
	~IniFile();

	int  OpenFile(const char *pszFilename);
	std::string ReadString(const char *pszSectionName,
		const char *pszKeyName,
		const char *pszDefaultValue);

	int ReadInt(const char *pszSectionName,
		const char *pszKeyName,
		const int pszDefaultValue);
private:
	unsigned int GetItemValue(const char *pszSectionName,
		const char *pszKeyName,
		char *pszReturnedString,
		unsigned int nSize);

	unsigned int GetItemValue(const char *pszSectionName,
		const char *pszKeyName,
		int &lReturnedValue,
		int lDefaultValue);

	unsigned int GetItemValue(const char *pszSectionName,
		const char *pszKeyName,
		int &lReturnedValue);

	unsigned int LocateSection(const char *pszSectionName,
		char * &pszSectionBegin,
		char * &pszSectionEnd);

	unsigned int LocateKeyRange(const char *pszKeyName,
		const char *pszSectionBegin,
		const char *pszSectionEnd,
		char * &pszKeyBegin,
		char * &pszKeyEnd);

	unsigned int LocateKeyValue(const char *pszKeyName,
		const char *pszSectionBegin,
		const char *pszSectionEnd,
		char * &pszValueBegin,
		char * &pszValueEnd);

	char *LocateStr(const char *pszCharSet,
		const char *pszBegin,
		const char *pszEnd);

	char *SearchMarchStr(const char *pszBegin, const char *pszCharSet);
	char *MapToContent(const char *p);
	char *MapToShadow(const char *p);

	void ToLower(char * pszSrc, size_t len);
	char* strdup(const char* szStr);
	void CloseFile();
private:
	char    *m_pszContent;      /* 配置文件的原始内容 */
	char    *m_pszShadow;       /* 配置文件的内容全部转换成小写 */
	size_t  m_nSize;            /* 配置文件内容的长度，不包括最后的NULL */
	short   m_bIsOpen;          /* 配置文件是否打开成功的标志 */
	char    *m_pszFilename;     /* 存放需要读取的配置文件名 */
};

