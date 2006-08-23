#include "stdafx.h"
#include "CLoadIni.h"
#include "stdio.h"
#include <fstream>

using namespace std;

//Loose parsing of ini file:
//fieldName=fieldvalue
//	or:
//fieldName=firstHalfOfFieldValue /	optionalComment
//secondHalfOfFieldValue /
//thirdHalfOfFieldValue
//	or:
//#comment
//	or:
//fieldName=fieldValue #comment
int CLoadIni::ReadIni(const char* pIniFile)
{
	Clear();
	std::string sField,sData;
	ifstream file;
	file.open(pIniFile, ios_base::in|ios_base::_Nocreate);
	if (!file.is_open())
	{
		return -1;
	}
	char c;
//	char* buff[512];
	int nPhase = phWFField;
	bool bEOF = false;
	while (1)
	{
		if (bEOF)
			break;
		file.read(&c, 1);
		if (file.eof())
		{	//Insert artificial newline at eof
			bEOF = true;
			c = '\n';
		}

		switch (nPhase)
		{
		case phCatLineField:
			if (c == '\r' || c == '\n')
			{
				break;
			}
		case phWFField:	//No field name yet. Presumably, beginning of the input line
			if (c == '/')	//Treat as a comment
			{
				rtrimstr(sField);
				nPhase = phWFNewline;
				break;
			}
			else if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c=='\f') //Trim whitespaces
			{
				break;
			}
			else if (c == '#' || c == '=')	//Comment or an assignment to unnamed fields are ignored
			{
				nPhase = phWFNewline;
				break;
			}
			else //Any other character is considered a valid field name character (ignore this -->)	//// if (c == '-' || c == '_' || (c>='a' && c<= 'z') || (c>='A' && c<= 'Z'))	//Allowable field name characters
			{
				nPhase = phField;
			}
		case phField:	//Setting field name
			if (c == '/')	//Treat as a comment
			{
				rtrimstr(sField);
				nPhase = phWFCatLineField;
				break;
			}
			else if (c == '\r' || c == '\n')	//Field entered with no value assignment
			{
				nPhase = phWFField;
				break;
			}
			else if (c == '=')	//An assignment
			{
				nPhase = phOperator;
			}
			else if (c == '#')	//Comment - Field entered with no value assignment
			{
				AddField(sField, sData);
				nPhase = phWFNewline;
				break;
			}
			else //Any other character is considered a valid field name character
			{
				sField += c;
				break;
			}
		case phOperator:	//setting operator type (only assignment supported at the moment)
			if (c == '=')	//Assignment operator
				nPhase = phWFData;
			else
				nPhase = phWFNewline;
			break;
		case phCatLineData:
			if (c == '\r' || c == '\n')
			{
				break;
			}
		case phWFData:		//Waiting to start getting data
			if (c == '/')	//Treat as a comment
			{
				rtrimstr(sData);
				nPhase = phWFCatLineData;
				break;
			}
			else if (c == '\r' || c == '\n')	//Empty data - Field entered with no data (even though there was an operator)
			{
				AddField(sField, sData);
				nPhase = phWFField;
				break;
			}
			else if (c == ' ' || c == '\t' || c=='\f') //Trim whitespaces
			{
				break;
			}
			else if (c == '#')	//Comment - Field entered with no data (even though there was an operator)
			{
				AddField(sField, sData);
				nPhase = phWFNewline;
				break;
			}
			else //Any other character is considered a valid data character
			{
				nPhase = phData;
			}
		case phData:		//Set field data
			if (c == '/')	//Treat as a comment
			{
				rtrimstr(sData);
				nPhase = phWFCatLineData;
				break;
			}
			else if (c == '\r' || c == '\n')	//data ended
			{
				AddField(sField, sData);
				nPhase = phWFField;
			}
			else if (c == '#')	//Comment
			{
				AddField(sField, sData);
				nPhase = phWFNewline;
			}
			else //Any other character is considered a valid data character
			{
				sData += c;
			}
			break;
		case phWFNewline:	//In comment/data ended - wait for a new line
			if (c == '\r' || c == '\n')	//NewLine
			{
				nPhase = phWFField;
			}
			break;
		case phWFCatLineField:	//Cat line done - rest of line is considered comment
			if (c == '\r' || c == '\n')	//NewLine
			{
				nPhase = phCatLineField;
			}
			break;
		case phWFCatLineData:	//Cat line done - rest of line is considered comment
			if (c == '\r' || c == '\n')	//NewLine
			{
				nPhase = phCatLineData;
			}
			break;
		default:
			nPhase = phWFNewline;
		}
	}
	m_bLoaded = true;
	file.close();
	return 0;
}


void CLoadIni::GetIntArray (const std::string& str, int* &pArray, int* pX, int* pY)
{
	std::string buff;

	int nMaxX = 0;
	int x = 0;
	int y = 0;
	pArray = NULL;

	//Check array dimensions
	std::string::const_iterator a = str.begin();
	while (a != str.end())
	{
		switch (*a)
		{
		case ',':
			x++;
			break;
		case ';':
			if (x > nMaxX)
				nMaxX = x;
			x = 0;
			y++;
			break;
		default:
			break;
		}
		a++;
	}

	//If no data is given, Minimal array is 1X1 with 0 as value
	nMaxX++;
	int nMaxY = y+1;
	if (pX != NULL)
		*pX = nMaxX;
	if (pY != NULL)
		*pY = nMaxY;

	//Create Array
	int nCount = nMaxX*nMaxY;
	pArray = new int [nCount];
	memset (pArray, 0, sizeof(int)*nCount);

	//Fill Array with values
	x = y = 0;
	a = str.begin();
	while (a != str.end())
	{
		switch (*a)
		{
		case ',':
			pArray[y*nMaxY+x] = atoi(buff.c_str());
			buff.clear();
			x++;
			break;
		case ';':
		case '\0':
			pArray[y*nMaxY+x] = atoi(buff.c_str());
			buff.clear();
			x = 0;
			y++;
			break;
		default:
			buff += *a;
			break;
		}
		a++;
	}
	pArray[y*nMaxY+x] = atoi(buff.c_str());
}


void CLoadIni::GetDblArray (const std::string& str, double* &pArray, int* pX, int* pY)
{
	std::string buff;

	int nMaxX = 0;
	int x = 0;
	int y = 0;
	pArray = NULL;

	//Check array dimensions
	std::string::const_iterator a = str.begin();
	while (a != str.end())
	{
		switch (*a)
		{
		case ',':
			x++;
			break;
		case ';':
			if (x > nMaxX)
				nMaxX = x;
			x = 0;
			y++;
			break;
		default:
			break;
		}
		a++;
	}

    if( x > nMaxX )
        nMaxX = x;

	//If no data is given, Minimal array is 1X1 with 0 as value
	nMaxX++;
	int nMaxY = y+1;
	if (pX != NULL)
		*pX = nMaxX;
	if (pY != NULL)
		*pY = nMaxY;

	//Create Array
	int nCount = nMaxX*nMaxY;
	pArray = new double [nCount];
	for (int i=0; i<nCount; i++)
		pArray[i] = 0;

	//Fill Array with values
	x = y = 0;
	a = str.begin();
	while (a != str.end())
	{
		switch (*a)
		{
		case ',':
			pArray[y*nMaxY+x] = atof(buff.c_str());
			buff.clear();
			x++;
			break;
		case ';':
		case '\0':
			pArray[y*nMaxY+x] = atof(buff.c_str());
			buff.clear();
			x = 0;
			y++;
			break;
		default:
			buff += *a;
			break;
		}
		a++;
	}
	pArray[y*nMaxY+x] = atof(buff.c_str());
}

