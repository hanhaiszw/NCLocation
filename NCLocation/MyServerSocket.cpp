#include "stdafx.h"
#include "MyServerSocket.h"
#include "NCLocationDlg.h"
#include "ToolUtils.h"

MyServerSocket::MyServerSocket(CNCLocationDlg * pMainDlg)
{
	this->pMainDlg = pMainDlg;
	m_isServerOpen = false;
}


MyServerSocket::~MyServerSocket()
{
	//����������ڿ��ţ���ر�
	if (m_isServerOpen) {
		stopServer();
	}
}

//����SocketServer����
BOOL MyServerSocket::startServer(int port) {
	//pMainDlg->setEditMsgBox(_T("this is a test"));
	//�����Ѿ��򿪣���ֱ�ӷ���
	if (m_isServerOpen) {
		return TRUE;
	}
	m_ServicePort = port;

	m_hListenThread = CreateThread(NULL, 0, ListenThreadFunc, this, 0, 0);

	//�˴��޷��ж��Ƿ�����˿ڳɹ�   �жϵ��������߳��Ƿ�ɹ�
	if (m_hListenThread == NULL)
	{
		//AfxMessageBox(_T("�����˿ڼ���ʧ��"));
		return FALSE;
	}
	else {
		//AfxMessageBox(_T("����socket�ɹ�"));
		return TRUE;
	}
}
//�رշ���
void MyServerSocket::stopServer() {
	int AllClient = m_ClientArray.GetCount();
	for (int idx = 0; idx < AllClient; idx++)
	{
		closesocket(m_ClientArray.GetAt(idx).m_ClientSocket);
	}
	m_ClientArray.RemoveAll();
	closesocket(m_SockListen);
	m_isServerOpen = FALSE;

	//TerminateThread(m_hListenThread, EXIT_FAILURE);
	//CloseHandle(m_hListenThread);
}

//�ȴ��ͻ������߳�
DWORD WINAPI ListenThreadFunc(LPVOID Lparam) {
	MyServerSocket * pServer = (MyServerSocket*)Lparam;
	if (INVALID_SOCKET == (pServer->m_SockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		AfxMessageBox(_T("����socketʧ��"));
		return 0;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(pServer->m_ServicePort);
	if (0 != ::bind(pServer->m_SockListen, (sockaddr *)&service, sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("�󶨶˿�ʧ��"));
		return 0;
	}
	if (0 != listen(pServer->m_SockListen, 5))
	{
		AfxMessageBox(_T("�����˿�ʧ��"));
		return 0;
	}

	CString strPort;
	strPort.Format("%d", pServer->m_ServicePort);
	pServer->pMainDlg->setEditMsgBox(_T("���������ɹ�����ʼ�����˿�") + strPort);
	pServer->m_isServerOpen = TRUE;

	//����ѭ���������˿�
	while (TRUE)
	{
		if (socket_Select(pServer->m_SockListen, 100, TRUE))
		{
			sockaddr_in clientAddr;
			int iLen = sizeof(sockaddr_in);
			SOCKET accSock = accept(pServer->m_SockListen, (struct sockaddr *)&clientAddr, &iLen);
			if (accSock == INVALID_SOCKET)
			{
				continue;
			}
			//���ڵ��������
			CClientItem tItem;
			tItem.m_ClientSocket = accSock;
			//tItem.m_strIp = inet_ntoa(clientAddr.sin_addr); //IP��ַ
			char sendBuf[20] = { '\0' };
			inet_ntop(AF_INET, (void*)&clientAddr.sin_addr.s_addr, sendBuf, 16);
			tItem.m_strIp = sendBuf;          //ip��ַ

			tItem.m_pMainWnd = pServer;
			//��������ָ�봫��CClientItem����������
			tItem.pDlg = pServer->pMainDlg;
			int idx = pServer->m_ClientArray.Add(tItem); //idx�ǵ�x�����ӵĿͻ���
			tItem.m_hThread = CreateThread(NULL, 0, ClientThreadProc, //����һ���̲߳�����:CREATE_SUSPENDED
				&(pServer->m_ClientArray.GetAt(idx)), CREATE_SUSPENDED, NULL);
			pServer->m_ClientArray.GetAt(idx).m_hThread = tItem.m_hThread;
			//�Ȱ�hThread�����˽ڵ㣬�ſ�ʼִ���̣߳�����
			ResumeThread(tItem.m_hThread);
			pServer->pMainDlg->setEditMsgBox(tItem.m_strIp + _T("����"));
			Sleep(100);
		}
	}
}

//����ÿ���ͻ��̶߳Ի�
//��������
//int totalRoad = 5000;
//int xPos = 0;
DWORD WINAPI ClientThreadProc(LPVOID Lparam) {
	USES_CONVERSION;
	//CString strMsg;
	CClientItem ClientItem = *(CClientItem *)Lparam;
	// �洢���յ�������
	char szRev[MAX_BUFF] = { 0 };  //
	char chLen[4] = { 0 };  // ��ȡ����
	while (TRUE)
	{
		if (socket_Select(ClientItem.m_ClientSocket, 100, TRUE))
		{
			// �Ƚ������ݳ����ٶ�ȡ����
			int iRet0 = recv(ClientItem.m_ClientSocket, chLen, 4, MSG_WAITALL);
			int len = ToolUtils::chars2intBig(chLen);

			// iRet�� ��ȡ�ĳ���
			//int iRet = recv(ClientItem.m_ClientSocket, szRev, sizeof(szRev), MSG_WAITALL);
			int iRet = recv(ClientItem.m_ClientSocket, szRev, len, MSG_WAITALL);
			if (iRet == len)  // iRet > 0
			{
				//strMsg = A2T(szRev); //���ĳ������룬Ӣ������
									 //strMsg.Format(_T("%s"),szRev); //��ôд��Ӣ�Ķ�������
				char* pChar = new char[iRet+1];
				memcpy(pChar, szRev, iRet * sizeof(char));
				pChar[iRet] = 0;
				// ���������洦��
				ClientItem.pDlg->solveSocketData(pChar, iRet+1, ClientItem.m_ClientSocket);
			}
			else {
				CString Msg = ClientItem.m_strIp + _T(" ���뿪");
				ClientItem.m_pMainWnd->RemoveClientFromArray(ClientItem);
				//��ʾĳ���ڵ��Ѿ��뿪
				ClientItem.pDlg->setEditMsgBox(Msg);
				//AfxMessageBox(strMsg);
				break;
			}
		}
	}
	return 0;
}

//�жϵ�ǰsocket״̬
BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead) {
	FD_SET fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket, &fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;
	int iRet = 0;
	if (bRead)
	{
		iRet = select(0, &fdset, NULL, NULL, &tv);
	}
	else
	{
		iRet = select(0, NULL, &fdset, NULL, &tv);
	}
	if (iRet <= 0)
	{
		return FALSE;
	}
	else if (FD_ISSET(hSocket, &fdset))
	{
		return TRUE;
	}
	return FALSE;
}



BOOL MyServerSocket::equal(const CClientItem * p1, const CClientItem * p2)
{
	if (p1->m_ClientSocket == p2->m_ClientSocket && p1->m_hThread == p2->m_hThread && p1->m_strIp == p2->m_strIp)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//��ͻ��˷�����Ϣ
void MyServerSocket::SendClientMsg(CString strMsg, CClientItem * pWhoseItem)
{
	if (strMsg == "") {
		return;
	}
	CString Msg;
	//USES_CONVERSION;//T2A
	char szBuf[256] = { 0 };
	strcpy_s(szBuf, 256,strMsg);
	for (int i = 0; i < m_ClientArray.GetCount(); i++)
	{
		if (!pWhoseItem || equal(pWhoseItem, &(m_ClientArray.GetAt(i))))
		{
			send(m_ClientArray.GetAt(i).m_ClientSocket, szBuf, 256, 0);
			Msg = _T("������>>") + strMsg + _T(">> ") + m_ClientArray.GetAt(i).m_strIp;
			pMainDlg->setEditMsgBox(Msg);
		}
	}

}

// ��Ϣ����
const int FATHER_MSG = 1;
void MyServerSocket::send2Client(SOCKET socket, char * msg, int msgLen)
{
	//������socket������Ҳ��������쳣
	// �ȷ�����������
	char chLen1[4] = { 0 };
	ToolUtils::int2charsBig(FATHER_MSG, chLen1);

	send(socket, chLen1, 4, 0);

	// ���ͳ���
	char chLen2[4] = { 0 };
	ToolUtils::int2charsBig(msgLen, chLen2);

	// ��������
	send(socket, chLen2, 4, 0);
	int size = send(socket, msg, msgLen, 0);
	/*if (size < 0) {
	AfxMessageBox("�����쳣");
	}*/
}


//�ͻ������ߣ��������Ƴ��ýڵ�
void MyServerSocket::RemoveClientFromArray(CClientItem in_item) {
	for (int idx = 0; idx < m_ClientArray.GetCount(); idx++)
	{
		if (in_item.m_ClientSocket == m_ClientArray[idx].m_ClientSocket &&
			in_item.m_hThread == m_ClientArray[idx].m_hThread &&
			in_item.m_strIp == m_ClientArray[idx].m_strIp &&
			in_item.m_pMainWnd == m_ClientArray[idx].m_pMainWnd)
		{
			m_ClientArray.RemoveAt(idx);
		}
	}
	return;
}


