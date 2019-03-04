#include "stdafx.h"
#include "SocketMsgHandler.h"
#include <regex>
#include "NCLocationDlg.h"

SocketMsgHandler::SocketMsgHandler(CNCLocationDlg * pMainDlg,CWebBrowser2* m_web)
{
	this->pMainDlg = pMainDlg;
	this->m_web = m_web;
}


SocketMsgHandler::~SocketMsgHandler()
{
}





string SocketMsgHandler::getAddress(CString longitude, CString latitude)
{
	CString   value = "";
	RequestParam reqParm{ "location",longitude+","+latitude };
	//conn.addParam("location", );
	value = httpClient.doGet(1, reqParm);

	string result = value.GetBuffer();
	value.ReleaseBuffer();

	//正则匹配出地址数据
	string pattern = "<formatted_address>(.*?)</formatted_address>";
	regex re(pattern);
	smatch results;
	//string test_str = "<formatted_address>address</formatted_address>";
	string address("");
	if (regex_search(result, results, re)) {
		string str_match = results[1].str();
		//address = str_match.c_str();
		address = str_match;
		//AfxMessageBox(address);
		pMainDlg->setEditMsgBox(address.c_str());
	}
	else {
		//获取地址失败
	}
		
	//AfxMessageBox(value);
	return address;
}

void SocketMsgHandler::makePoint(CString longitude, CString latitude)
{
	CWebPage webPage;
	webPage.SetDocument(m_web->get_Document());
	const CString funcName(_T("makePoint"));
	CComVariant varResult;
	webPage.CallJScript(funcName, longitude, latitude, &varResult);
}
