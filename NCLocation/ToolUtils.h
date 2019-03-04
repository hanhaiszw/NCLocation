#pragma once
//使用的工具方法
#include <string>
#include <time.h>

using namespace std;
class ToolUtils
{
public:
	ToolUtils();
	~ToolUtils();
	static CString getTime();

	static CString getPreciseTime(); //精确到微秒
	/*
	获取当前工作路径  后带  \
	*/
	static CString getCurrentPath();
	static void newFolder(const CString folder);
	static CString getNetIp();

	static void write2File(CString filePath, CString str);//写入文件
	//多字节转化为utf8
	static string MUL_To_UTF8(string strData);
	//utf8转化为多字节
	static CString UTF8_To_GBK(string utf8Str);
	//选择文件
	static CString selectFile(CString fileForm = _T("txt"));

	//判断文件是否存在
	static inline bool isFileExist(CString filePath) {
		return PathFileExists(filePath);
	};

	//十六进制值串 转化为十进制数
	//输入：char ppch[] = { 0x03, 0x52};
	//输出：850     0x0352  等价于  十进制 850
	static int ToolUtils::hex2dec(char* pHex, int nLen);

	//从资源文件中释放出文件到当前目录
	static bool realseFile(CString resourceType,CString fileName,const int resourceID);

	// result必须是长度为4的char数组
	static void int2charsBig(int value, char result[]);
	// src必须是长度为4的char数组
	static int chars2intBig(char src[]);
private:
	//十六进制值串转ASCII串
	//输入：char ppch[] = { 0x03, 0x52};
	//输出："0352"
	static void ToolUtils::HexToAscii(char * pHex, char * pAscii, int nLen);
};

