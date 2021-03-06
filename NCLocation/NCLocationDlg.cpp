
// NCLocationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCLocation.h"
#include "NCLocationDlg.h"
#include "afxdialogex.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNCLocationDlg dialog



CNCLocationDlg::CNCLocationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NCLOCATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CNCLocationDlg::~CNCLocationDlg()
{
	if (myServerSocket != nullptr) {
		delete myServerSocket;
	}
	if (sktMsgHandler != nullptr) {
		delete sktMsgHandler;
	}
}

void CNCLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_web);
	DDX_Control(pDX, IDC_EDIT1, m_EditMsgBox);
}

BEGIN_MESSAGE_MAP(CNCLocationDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_ABOUT, &CNCLocationDlg::OnFileAbout)
	ON_COMMAND(ID_FILE_TEST, &CNCLocationDlg::OnFileTest)
	ON_COMMAND(ID_FILE_CLEAR, &CNCLocationDlg::OnClear)
END_MESSAGE_MAP()


// CNCLocationDlg message handlers

BOOL CNCLocationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// 全屏显示
	this->ShowWindow(SW_SHOWMAXIMIZED);
	init();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNCLocationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNCLocationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNCLocationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//初始化变量
void CNCLocationDlg::init()
{
	//加载网页
	/*COleVariant varEmpty;
	CString str("www.baidu.com");
	COleVariant varUrl(str);
	m_web.Navigate2(varUrl, varEmpty, varEmpty, varEmpty, varEmpty);*/
	//CString strPath = ToolUtils::ResourceToURL(_T("en_Location.html"));
	//m_web.Navigate(_T("file:////E:/VSWorkSpace/NCLocation/en_Location.html"), nullptr, nullptr, nullptr, nullptr);

	//获取资源文件中的html文件
	char filepath[255];
	wsprintf(filepath, "res://%s.exe/%d", ::AfxGetAppName(), IDR_HTML_EN_LOCATION);
	m_web.Navigate(filepath, nullptr, nullptr, nullptr, nullptr);


	//初始化socket
	//非常重要
	//把窗口类对象传给socket类，用于数据回传和控制
	myServerSocket = new MyServerSocket(const_cast<CNCLocationDlg*>(this));

	myServerSocket->startServer(8260);
	//socket信息处理类
	sktMsgHandler = new SocketMsgHandler(const_cast<CNCLocationDlg*>(this), &m_web);
}

// 显示后台信息
void CNCLocationDlg::setEditMsgBox(CString strMsg)
{
	static int editMaxLine = 200;  //控制最大显示行数
	int count = m_EditMsgBox.GetLineCount();
	if (count > 200) {
		m_EditMsgBox.SetWindowText(_T(""));
	}

	int length = m_EditMsgBox.GetWindowTextLengthA();
	m_EditMsgBox.SetSel(length, length, true);
	m_EditMsgBox.SetFocus();

	//SendDlgItemMessage(IDC_EDITMSGBOX, WM_VSCROLL, SB_BOTTOM, 0);
	m_EditMsgBox.ReplaceSel(ToolUtils::getTime() + _T("\r\n") + strMsg + _T("\r\n"));
}

void CNCLocationDlg::solveSocketMsg(CString socketMsg)
{
	static int count = 0;
	setEditMsgBox(std::to_string(++count).c_str());
	CString strTmp;
	int i = 0;
	CString arr[2];
	// 字符串切分  注意 不支持多个空格分割
	while (AfxExtractSubString(strTmp, socketMsg, i, _T(' ')))
		arr[i++] = strTmp;

	//MessageBox(arr[0] + "   " + arr[1]);
	sktMsgHandler->getAddress(arr[0], arr[1]);
	//makePoint(arr[0], arr[1]);
}

void CNCLocationDlg::solveSocketData(char * pChar, int len,SOCKET socket)
{
	//MessageBox(ToolUtils::UTF8_To_GBK(pChar));
	CString strTmp;
	int i = 0;
	CString arr[2];
	// 字符串切分  注意 不支持多个空格分割
	while (AfxExtractSubString(strTmp, pChar, i, _T(','))) {
		arr[i++] = strTmp;
		if (i == 2) {
			break;
		}
	}
		
	string address = sktMsgHandler->getAddress(arr[0], arr[1]);

	address = ToolUtils::MUL_To_UTF8(address);
	myServerSocket->send2Client(socket, (char*)address.c_str(), address.size());

	//makePoint(arr[0], arr[1]);
	//MessageBox(address);
}



void CNCLocationDlg::makePoint(CString longitude, CString latitude)
{
	CWebPage webPage;
	webPage.SetDocument(m_web.get_Document());
	const CString funcName(_T("makePoint"));
	CComVariant varResult;
	webPage.CallJScript(funcName, longitude, latitude, &varResult);
}



void CNCLocationDlg::OnFileAbout()
{
	// TODO: Add your command handler code here
	CString fileName = "readme.txt";
	CString readmePath = ToolUtils::getCurrentPath() + fileName;
	if (ToolUtils::realseFile("TXT", fileName, IDR_TXT_README)) {
		CString strCMD = "notepad " + readmePath;
		WinExec(strCMD, SW_SHOW);
	}
}


void CNCLocationDlg::OnFileTest()
{

	CString lon;
	lon.Format(_T("%lf"), 118.801741);
	CString lat;
	lat.Format(_T("%lf"), 32.064245);


	string msg = sktMsgHandler->getAddress(lon,lat);
	MessageBox(msg.c_str());
	makePoint(lon, lat);
	//sktMsgHandler->makePoint(lon, lat);
}





void CNCLocationDlg::OnClear()
{
	// TODO: Add your command handler code here
	// 清空消息显示框
	m_EditMsgBox.SetWindowText(_T(""));
}
