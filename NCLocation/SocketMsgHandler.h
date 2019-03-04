#pragma once
#include "CHttpClient.h"
#include "CWebBrowser2.h"
//类的交叉引用
class CNCLocationDlg;
//const CString url = "http://api.map.baidu.com/geocoder/v2/?callback=renderReverse&extensions_road=true&output=xml&pois=0&ak=tHwiS3XNVwKNHo2kErtAHEb7aqXyswR8";
const CString url = "http://restapi.amap.com/v3/geocode/regeo?output=xml&key=657f444389b982da2a40416246f0fbc9";
class SocketMsgHandler
{
public:
	CHttpClient httpClient{ url };
	//用于操作主窗口方法
	CNCLocationDlg * pMainDlg;
	CWebBrowser2* m_web;
public:
	SocketMsgHandler(CNCLocationDlg * pMainDlg, CWebBrowser2* m_web);
	~SocketMsgHandler();
	string getAddress(CString longitude, CString latitude);
	void makePoint(CString longitude, CString latitude);
};

