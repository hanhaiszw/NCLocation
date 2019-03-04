#pragma once
#include "ClientArrayList.h"

//��Ľ�������
class CNCLocationDlg;

//sokcet��صķ���
#define MAX_BUFF 4096   //socket���յĳ���
DWORD WINAPI ListenThreadFunc(LPVOID Lparam);
DWORD WINAPI ClientThreadProc(LPVOID Lparam);
BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);
class MyServerSocket
{
public:
	//���ڲ��������ڷ���
	CNCLocationDlg * pMainDlg;

	HANDLE m_hListenThread;
	SOCKET m_SockListen;
	CArray<CClientItem> m_ClientArray;
	//�˿�
	UINT m_ServicePort;
	BOOL m_isServerOpen;
public:
	MyServerSocket(CNCLocationDlg * pMainDlg);
	~MyServerSocket();
	
	//����SocketServer
	BOOL startServer(int port);
	//�ر�
	void stopServer();
	BOOL equal(const CClientItem * p1, const CClientItem * p2);
	void SendClientMsg(CString strMsg, CClientItem * pWhoseItem);
	void send2Client(SOCKET socket, char * msg, int msgLen);
	//��SocketServer�б��Ƴ�����Socket
	void RemoveClientFromArray(CClientItem in_item);
};

