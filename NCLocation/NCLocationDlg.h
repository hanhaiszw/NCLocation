
// NCLocationDlg.h : header file
//

#pragma once
#include "CWebBrowser2.h"
#include "afxwin.h"
#include "MyServerSocket.h"
#include "WebPage.h"
#include "SocketMsgHandler.h"

// CNCLocationDlg dialog
class CNCLocationDlg : public CDialog
{
// Construction
public:
	CNCLocationDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CNCLocationDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NCLOCATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CWebBrowser2 m_web;   //浏览器插件

	// 显示后台执行信息控件
	CEdit m_EditMsgBox;

	//ServerSocket
	MyServerSocket* myServerSocket;
	//处理socket信息
	SocketMsgHandler* sktMsgHandler;
	void init();
	afx_msg void OnFileAbout();
	afx_msg void OnFileTest();
public:
	// 显示后台信息
	void setEditMsgBox(CString strMsg);

	void solveSocketMsg(CString socketMsg);

	void solveSocketData(char* pChar, int len, SOCKET socket);
	void makePoint(CString longitude, CString latitude);
	afx_msg void OnClear();
};
