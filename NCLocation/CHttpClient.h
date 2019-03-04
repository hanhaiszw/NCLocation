#pragma once
#include   "wininet.h"    
#include   "afxinet.h"  
struct RequestParam
{
	CString name;
	CString value;
};
/*********************¿‡∂®“Â***********************************/
class CHttpClient
{
public:
	//void   addParam(CString   name, CString   value);
	CString   doPost(int count, RequestParam...);
	CString   doGet(int count, RequestParam...);
	CHttpClient(CString url);
	virtual   ~CHttpClient();
private:
	CString url;
	
//	CString   CONTENT;
//	int   CL;
//	CStringList   values;
//	CStringList   names;
};

