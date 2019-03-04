#pragma once
#include "ClientArrayList.h"

//类的交叉引用
class CNCLocationDlg;

//sokcet相关的方法
#define MAX_BUFF 4096   //socket接收的长度
DWORD WINAPI ListenThreadFunc(LPVOID Lparam);
DWORD WINAPI ClientThreadProc(LPVOID Lparam);
BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);
class MyServerSocket
{
public:
	//用于操作主窗口方法
	CNCLocationDlg * pMainDlg;

	HANDLE m_hListenThread;
	SOCKET m_SockListen;
	CArray<CClientItem> m_ClientArray;
	//端口
	UINT m_ServicePort;
	BOOL m_isServerOpen;
public:
	MyServerSocket(CNCLocationDlg * pMainDlg);
	~MyServerSocket();
	
	//开启SocketServer
	BOOL startServer(int port);
	//关闭
	void stopServer();
	BOOL equal(const CClientItem * p1, const CClientItem * p2);
	void SendClientMsg(CString strMsg, CClientItem * pWhoseItem);
	void send2Client(SOCKET socket, char * msg, int msgLen);
	//从SocketServer列表移除无用Socket
	void RemoveClientFromArray(CClientItem in_item);
};

