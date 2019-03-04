#pragma once
#include "stdafx.h"
class CNCLocationDlg;
class MyServerSocket;

class CClientItem
{
public:
	CString m_strIp;
	SOCKET m_ClientSocket;
	HANDLE m_hThread;
	MyServerSocket *m_pMainWnd;
	//用于操作主窗口方法，由MySocket中传入
	//对MySocket中pMainDlg的引用
	CNCLocationDlg *pDlg;
	CClientItem() {
		m_ClientSocket = INVALID_SOCKET;
		m_hThread = NULL;
		m_pMainWnd = NULL;
		pDlg = NULL;
	}
};
