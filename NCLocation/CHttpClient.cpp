#include "stdafx.h"
#include "CHttpClient.h"


CHttpClient::CHttpClient(CString url)
{
	this->url = url;
}


CHttpClient::~CHttpClient()
{
}

/*************************类实现********************************/
// 添加字段  
//void   CHttpClient::addParam(CString   name, CString   value)
//{
//	names.AddTail((LPCTSTR)name);
//	values.AddTail((LPCTSTR)value);
//	CString   eq = "=";
//	CString   an = "&";
//	CONTENT = CONTENT + name + eq + value + an;
//	CL = CONTENT.GetLength();
//}
// 以http Get方式请求URL  
CString   CHttpClient::doGet(int count, RequestParam...)
{
	CString param="";
	va_list ap = nullptr;
	va_start(ap, count);
	CString   eq = "=";
	CString   an = "&";
	for (int i = 0; i < count; ++i) {
		RequestParam arg = va_arg(ap, RequestParam);
		param += (an + arg.name + eq + arg.value);
	}
	va_end(ap);


	CString   httpsource = "";
	CInternetSession   session1(nullptr, 0);
	CHttpFile*   pHTTPFile = nullptr;
	try {
		pHTTPFile = (CHttpFile*)session1.OpenURL(url+param);
		//session1.    
	}
	catch (CInternetException& e)
	{
		pHTTPFile = nullptr;
		
	}
	if (pHTTPFile)
	{
		CString   text;
		for (int i = 0; pHTTPFile->ReadString(text); i++)
		{
			httpsource = httpsource + text + "\r\n";
		}
		pHTTPFile->Close();
		delete   pHTTPFile;
	}
	else
	{

	}
	//utf8转gbk
	string str = httpsource.GetBuffer();
	httpsource.ReleaseBuffer();
	return   ToolUtils::UTF8_To_GBK(str);
}


// 以Http Post方式请求URL  
CString   CHttpClient::doPost(int count, RequestParam...)
{
	CString param = "";
	va_list ap = nullptr;
	va_start(ap, count);
	CString   eq = "=";
	CString   an = "&";
	for (int i = 0; i < count; ++i) {
		RequestParam arg = va_arg(ap, RequestParam);
		param += (an + arg.name + eq + arg.value);
	}
	va_end(ap);


	CString   httpsource = "";
	CInternetSession   session1;
	CHttpConnection*   conn1 = nullptr;
	CHttpFile*   pFile = nullptr;
	CString   strServerName;
	CString   strObject;
	INTERNET_PORT   nPort;
	DWORD   dwServiceType;
	AfxParseURL((LPCTSTR)(url + param), dwServiceType, strServerName, strObject, nPort);
	DWORD   retcode;

	CString strContent;
	char*   outBuff = strContent.GetBuffer(1000);
	try
	{
		conn1 = session1.GetHttpConnection(strServerName, nPort);
		pFile = conn1->OpenRequest(0, strObject, nullptr, 1, nullptr, "HTTP/1.1", INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT);
		pFile->AddRequestHeaders("Content-Type:   application/x-www-form-urlencoded");
		pFile->AddRequestHeaders("Accept:   */*");
		pFile->SendRequest(nullptr, 0, outBuff, strlen(outBuff) + 1);
		pFile->QueryInfoStatusCode(retcode);
	}
	catch (CInternetException& e) {};
	if (pFile)
	{
		CString   text;
		for (int i = 0; pFile->ReadString(text); i++)
		{
			httpsource = httpsource + text + "\r\n";
		}
		pFile->Close();
	}
	else
	{
		// do anything.....  
	}
	strContent.ReleaseBuffer();

	string str = httpsource.GetBuffer();
	httpsource.ReleaseBuffer();
	return   ToolUtils::UTF8_To_GBK(str);
	delete   pFile;
	delete   conn1;
	session1.Close();
}
