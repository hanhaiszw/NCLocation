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
获取当前工作路径  后带 \
*/
CString ToolUtils::getCurrentPath() {
	CString  strPathName;
	GetModuleFileName(NULL, strPathName.GetBuffer(256), 256);
	strPathName.ReleaseBuffer(256);
	//去掉最后的.exe可执行文件名
	int nPos = strPathName.ReverseFind('\\');
	strPathName = strPathName.Left(nPos + 1);
	
	return strPathName;
}

//创建目录
void ToolUtils::newFolder(const CString folderPath) {
	//检查路径是否存在，不存在则创建
	if (!PathIsDirectory(folderPath))
	{
		::CreateDirectory(folderPath, 0);
	}
}

//获取外网IP
CString ToolUtils::getNetIp() {
	char buf[MAX_PATH] = { 0 };
	//char chTempIp[128];
	//char chIP[64];
	CString strPath;
	strPath = getCurrentPath() + _T("netip.ini");
	URLDownloadToFile(0, _T("http://2018.ip138.com/ic.asp"), strPath, 0, NULL);  //从ip138网查询IP
	//USES_CONVERSION;//T2A

	CString ip("");
	FILE *fp;
	fopen_s(&fp, strPath,"r");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_SET);
		fread(buf, 1, 256, fp);
		fclose(fp);
		//不可删除文件后再使用buf
		//DeleteFile(strPath);
		//要获取的ip在中括号之间
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
	//把GB2312编码的中文字串转换为UTF-8编码的中文字串  
	int iLen = strData.length();
	CHAR* pMb = new CHAR[iLen + 1];
	int iMbLen = iLen + 1;
	ZeroMemory(pMb, iMbLen);
	//memset(pMb, 0, iMbLen);
	memcpy_s(pMb, iMbLen, strData.c_str(), strData.length());

	//将多字节字符串编码转换成宽字符串  
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
	//转换一个宽字符串到UTF8字符串  
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

//没获取到返回空
//只查看指定（一种）后缀的文件
CString ToolUtils::selectFile(CString fileForm)
{
	CString strFile = _T("");
	CString strFileForm = _T("Describe Files(*.") + fileForm + _T(")|*.") + fileForm + _T("||)\"");
	//_T("Describe Files(*.txt)|*.txt||)")
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, strFileForm);
	//为文件选择框指定一个初始位置
	//此设置方法不稳定  不用
	//dlgFile.m_ofn.lpstrInitialDir = getCurrentPath();
	//标题
	dlgFile.m_ofn.lpstrTitle = _T("查找");
	if (dlgFile.DoModal() == IDOK) {
		strFile = dlgFile.GetPathName();
	}
	return strFile;
}

//十六进制值串转ASCII串
//输入：char ppch[] = { 0x03, 0x52};
//输出："0352"
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


//十六进制值串转十进制值
int ToolUtils::hex2dec(char* pHex, int nLen) {
	int length = nLen * 2 + 1;
	char* pChar = new char[length];
	HexToAscii(pHex, pChar, nLen);
	pChar[length - 1] = '\0';   //结束符   /0
							
	int nValue = 0;
	sscanf_s(pChar, "%x", &nValue);
	delete[] pChar;

	return nValue;
}

/*
 释放资源中的文件
 */
bool ToolUtils::realseFile(CString resourceType, CString fileName, const int resourceID)
{
	CString readmePath = ToolUtils::getCurrentPath() + fileName;
	//判断下文件是否已经存在
	//若是不存在，则从资源中释放出来
	if (!ToolUtils::isFileExist(readmePath)) {
		//MessageBox("文件不存在");
		//文件不存在   从资源文件中释放出来
		HRSRC hrFile = FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(resourceID), resourceType);

		if (hrFile == NULL)
		{
			AfxMessageBox("没有找到资源 ");
			return false;
		}
		HGLOBAL hgFile = LoadResource(AfxGetInstanceHandle(), hrFile);
		if (hgFile == NULL)
		{
			AfxMessageBox("加载资源失败 ");
			return false;
		}
		LPVOID pvFile = LockResource(hgFile);
		if (pvFile == NULL)
		{
			AfxMessageBox("不能加载内存 ");
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
	//文件存在，则直接返回
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



