#pragma once
//ʹ�õĹ��߷���
#include <string>
#include <time.h>

using namespace std;
class ToolUtils
{
public:
	ToolUtils();
	~ToolUtils();
	static CString getTime();

	static CString getPreciseTime(); //��ȷ��΢��
	/*
	��ȡ��ǰ����·��  ���  \
	*/
	static CString getCurrentPath();
	static void newFolder(const CString folder);
	static CString getNetIp();

	static void write2File(CString filePath, CString str);//д���ļ�
	//���ֽ�ת��Ϊutf8
	static string MUL_To_UTF8(string strData);
	//utf8ת��Ϊ���ֽ�
	static CString UTF8_To_GBK(string utf8Str);
	//ѡ���ļ�
	static CString selectFile(CString fileForm = _T("txt"));

	//�ж��ļ��Ƿ����
	static inline bool isFileExist(CString filePath) {
		return PathFileExists(filePath);
	};

	//ʮ������ֵ�� ת��Ϊʮ������
	//���룺char ppch[] = { 0x03, 0x52};
	//�����850     0x0352  �ȼ���  ʮ���� 850
	static int ToolUtils::hex2dec(char* pHex, int nLen);

	//����Դ�ļ����ͷų��ļ�����ǰĿ¼
	static bool realseFile(CString resourceType,CString fileName,const int resourceID);

	// result�����ǳ���Ϊ4��char����
	static void int2charsBig(int value, char result[]);
	// src�����ǳ���Ϊ4��char����
	static int chars2intBig(char src[]);
private:
	//ʮ������ֵ��תASCII��
	//���룺char ppch[] = { 0x03, 0x52};
	//�����"0352"
	static void ToolUtils::HexToAscii(char * pHex, char * pAscii, int nLen);
};

