#include "stdafx.h"
#include "ToolUtils.h"
#include <fstream>

ToolUtils::ToolUtils()
{
}


ToolUtils::~ToolUtils()
{
}

CString ToolUtils::getTime() {
	CTime t;
	CString strTime;
	t = CTime::GetCurrentTime();
	strTime = t.Format(_T("%Y-%m-%d %H:%M:%S"));
	return strTime;
}

CString ToolUtils::getPreciseTime()
{
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
	strTime.Format(_T("%02d:%02d:%02d:%03d"), st.wHour, st.wMinute, st.wSecond,st.wMilliseconds);
	
	return strDate+" "+strTime;
}

/*
��ȡ��ǰ����·��  ��� \
*/
CString ToolUtils::getCurrentPath() {
	CString  strPathName;
	GetModuleFileName(NULL, strPathName.GetBuffer(256), 256);
	strPathName.ReleaseBuffer(256);
	//ȥ������.exe��ִ���ļ���
	int nPos = strPathName.ReverseFind('\\');
	strPathName = strPathName.Left(nPos + 1);
	
	return strPathName;
}

//����Ŀ¼
void ToolUtils::newFolder(const CString folderPath) {
	//���·���Ƿ���ڣ��������򴴽�
	if (!PathIsDirectory(folderPath))
	{
		::CreateDirectory(folderPath, 0);
	}
}

//��ȡ����IP
CString ToolUtils::getNetIp() {
	char buf[MAX_PATH] = { 0 };
	//char chTempIp[128];
	//char chIP[64];
	CString strPath;
	strPath = getCurrentPath() + _T("netip.ini");
	URLDownloadToFile(0, _T("http://2018.ip138.com/ic.asp"), strPath, 0, NULL);  //��ip138����ѯIP
	//USES_CONVERSION;//T2A

	CString ip("");
	FILE *fp;
	fopen_s(&fp, strPath,"r");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_SET);
		fread(buf, 1, 256, fp);
		fclose(fp);
		//����ɾ���ļ�����ʹ��buf
		//DeleteFile(strPath);
		//Ҫ��ȡ��ip��������֮��
		try
		{
			CString cstr(buf);
			int pos1 = cstr.Find('[');
			int pos2 = cstr.Find(']');
			ip = cstr.Mid(pos1 + 1, pos2 - pos1 - 1);
		}
		catch (const std::exception&)
		{

		}
		DeleteFile(strPath);
	}
	return ip;
}

void ToolUtils::write2File(CString filePath, CString str)
{
	std::ofstream OsWrite(filePath, std::ofstream::app);
	OsWrite << str;
	OsWrite << std::endl;
	OsWrite.close();
}

string ToolUtils::MUL_To_UTF8(string strData)
{
	//��GB2312����������ִ�ת��ΪUTF-8����������ִ�  
	int iLen = strData.length();
	CHAR* pMb = new CHAR[iLen + 1];
	int iMbLen = iLen + 1;
	ZeroMemory(pMb, iMbLen);
	//memset(pMb, 0, iMbLen);
	memcpy_s(pMb, iMbLen, strData.c_str(), strData.length());

	//�����ֽ��ַ�������ת���ɿ��ַ���  
	iLen = ::MultiByteToWideChar(CP_ACP, 0, pMb, iMbLen, NULL, 0);
	WCHAR* lpszW = NULL;
	lpszW = new WCHAR[iLen];
	::wmemset(lpszW, 0, iLen);

	int iRtn = ::MultiByteToWideChar(CP_ACP, 0, pMb, iMbLen, lpszW, iLen);
	if (iRtn != iLen)
	{
		delete[] pMb; pMb = NULL;
		delete[] lpszW; lpszW = NULL;
		return NULL;
	}
	//ת��һ�����ַ�����UTF8�ַ���  
	int iUTF8Len = ::WideCharToMultiByte(CP_UTF8, 0, lpszW, iLen, NULL, 0, NULL, NULL);
	if (0 == iUTF8Len)
	{
		delete[] pMb; pMb = NULL;
		delete[] lpszW; lpszW = NULL;

		return NULL;
	}

	char* pUTF8 = new char[iUTF8Len];
	::memset(pUTF8, 0, iUTF8Len);
	::WideCharToMultiByte(CP_UTF8, 0, lpszW, iLen, pUTF8, iUTF8Len, NULL, NULL);

	std::string strTemp(pUTF8);
	delete[] pMb; pMb = NULL;
	delete[] lpszW; lpszW = NULL;
	delete[] pUTF8; pUTF8 = NULL;

	return strTemp;
}

CString ToolUtils::UTF8_To_GBK(string utf8Str)
{
	const char* strUTF8 = utf8Str.c_str();
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	//USES_CONVERSION;
	CString result = strTemp.c_str();
	return result;
}

//û��ȡ�����ؿ�
//ֻ�鿴ָ����һ�֣���׺���ļ�
CString ToolUtils::selectFile(CString fileForm)
{
	CString strFile = _T("");
	CString strFileForm = _T("Describe Files(*.") + fileForm + _T(")|*.") + fileForm + _T("||)\"");
	//_T("Describe Files(*.txt)|*.txt||)")
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, strFileForm);
	//Ϊ�ļ�ѡ���ָ��һ����ʼλ��
	//�����÷������ȶ�  ����
	//dlgFile.m_ofn.lpstrInitialDir = getCurrentPath();
	//����
	dlgFile.m_ofn.lpstrTitle = _T("����");
	if (dlgFile.DoModal() == IDOK) {
		strFile = dlgFile.GetPathName();
	}
	return strFile;
}

//ʮ������ֵ��תASCII��
//���룺char ppch[] = { 0x03, 0x52};
//�����"0352"
void ToolUtils::HexToAscii(char * pHex, char * pAscii, int nLen)
{
	unsigned char Nibble[2];

	for (int i = 0; i < nLen; i++)
	{
		Nibble[0] = (pHex[i] & 0xF0) >> 4;
		Nibble[1] = pHex[i] & 0x0F;
		for (int j = 0; j < 2; j++)
		{
			if (Nibble[j] < 10)
				Nibble[j] += 0x30;
			else
			{
				if (Nibble[j] < 16)
					Nibble[j] = Nibble[j] - 10 + 'A';
			}
			*pAscii++ = Nibble[j];
		}   // for (int j = ...)
	}   // for (int i = ...)
}


//ʮ������ֵ��תʮ����ֵ
int ToolUtils::hex2dec(char* pHex, int nLen) {
	int length = nLen * 2 + 1;
	char* pChar = new char[length];
	HexToAscii(pHex, pChar, nLen);
	pChar[length - 1] = '\0';   //������   /0
							
	int nValue = 0;
	sscanf_s(pChar, "%x", &nValue);
	delete[] pChar;

	return nValue;
}

/*
 �ͷ���Դ�е��ļ�
 */
bool ToolUtils::realseFile(CString resourceType, CString fileName, const int resourceID)
{
	CString readmePath = ToolUtils::getCurrentPath() + fileName;
	//�ж����ļ��Ƿ��Ѿ�����
	//���ǲ����ڣ������Դ���ͷų���
	if (!ToolUtils::isFileExist(readmePath)) {
		//MessageBox("�ļ�������");
		//�ļ�������   ����Դ�ļ����ͷų���
		HRSRC hrFile = FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(resourceID), resourceType);

		if (hrFile == NULL)
		{
			AfxMessageBox("û���ҵ���Դ ");
			return false;
		}
		HGLOBAL hgFile = LoadResource(AfxGetInstanceHandle(), hrFile);
		if (hgFile == NULL)
		{
			AfxMessageBox("������Դʧ�� ");
			return false;
		}
		LPVOID pvFile = LockResource(hgFile);
		if (pvFile == NULL)
		{
			AfxMessageBox("���ܼ����ڴ� ");
			return false;
		}

		CString writePath = ToolUtils::getCurrentPath() + fileName;
		//AfxMessageBox(writePath);
		HANDLE hfFile = CreateFile(writePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		DWORD nFileLength = SizeofResource(AfxGetInstanceHandle(), hrFile);
		DWORD nWriteLength;
		WriteFile(hfFile, pvFile, nFileLength, &nWriteLength, NULL);
		CloseHandle(hfFile);
		FreeResource(hrFile);
	}
	//�ļ����ڣ���ֱ�ӷ���
	return true;
}

void ToolUtils::int2charsBig(int value, char result[])
{
	result[0] = (char)((value >> 24) & 0xFF);
	result[1] = (char)((value >> 16) & 0xFF);
	result[2] = (char)((value >> 8) & 0xFF);
	result[3] = (char)(value & 0xFF);
}

int ToolUtils::chars2intBig(char src[])
{
	int value;
	value = (int)(((src[0] & 0xFF) << 24)
		| ((src[1] & 0xFF) << 16)
		| ((src[2] & 0xFF) << 8)
		| (src[3] & 0xFF));
	return value;
}



