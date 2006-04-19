#ifndef CLOADINI_H
#define CLOADINI_H

#include <string>
#include <map>

class CLoadIni
{
	bool m_bLoaded;
	std::map<std::string,std::string> m_Fields;

	enum phases
	{
		phWFField,	//WF = Wait for
		phField,
		phWFOperator,
		phOperator,
		phWFData,
		phData,
		phWFNewline,
		phWFCatLineField,
		phCatLineField,
		phWFCatLineData,
		phCatLineData,

		phCount
	};
public:
	CLoadIni()
	{
		m_bLoaded = false;
	}

	~CLoadIni()
	{
		m_bLoaded = false;
	}
	static void GetIntArray (const std::string& str, int* &pArray, int* pX = NULL, int* pY = NULL);
	static void GetDblArray (const std::string& str, double* &pArray, int* pX = NULL, int* pY = NULL);
	static void rtrimstr(std::string& str)
	{
		char c;
		while (str.length() > 0)
		{
			c = *(str.rbegin());
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c=='\f')
				str.erase(str.length()-1);
			else
				break;
		}
	}
	int ReadIni(const char* pIniFile);
	int AddField(std::string& name, std::string& val)
	{
		//Right trim name and value
		rtrimstr(name);
		rtrimstr(val);
		m_Fields[name] = val;
		name.clear();
		val.clear();
		return 0;
	}
	int GetField(const std::string& name, std::string& val)
	{
		if (!m_bLoaded)
			return -1;
		std::map<std::string,std::string>::iterator a = m_Fields.find(name);
		if (a == m_Fields.end())
			return -2;
		val = a->second;
		return 0;
	}
	void Clear()
	{
		m_Fields.empty();
		m_bLoaded = false;
	}
};

#endif //CLOADINI_H