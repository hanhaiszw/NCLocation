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
	//���ڲ��������ڷ�������MySocket�д���
	//��MySocket��pMainDlg������
	CNCLocationDlg *pDlg;
	CClientItem() {
		m_ClientSocket = INVALID_SOCKET;
		m_hThread = NULL;
		m_pMainWnd = NULL;
		pDlg = NULL;
	}
};
