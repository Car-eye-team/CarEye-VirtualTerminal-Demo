#pragma once
#include <string>
#include <malloc.h>
#include <string.h>
#include "AutoIncressContent.h"

#ifndef _MSC_VER
#    define _alloca alloca
#endif

#ifndef min
#    define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define ByteSwap(v) do{decltype(v) _##v=v&0xFF;for(int i##v=1;i##v<sizeof(_##v);i##v++)_##v=(_##v<<8)|((v>>=8)&0xFF);v=_##v;}while(0)

class JTT808AutoIncressContent : public AutoIncressContent
{
private:
	unsigned int ReadPosition = 0;
public:
	AutoIncressContent & operator=(const AutoIncressContent & rv)
	{
		this->Reset();
		this->AutoIncressContent::Put(rv.Ptr(), rv.Size());
		return *this;
	}
	void Put(const ContentType::BYTE byData) override
	{
		this->AutoIncressContent::Put(byData);
	}
	void Put(const ContentType::WORD wData) override
	{
		ContentType::WORD _tmp = wData;
		ByteSwap(_tmp);
		this->AutoIncressContent::Put(_tmp);
	}
	void Put(const ContentType::DWORD dData) override
	{
		ContentType::DWORD _tmp = dData;
		ByteSwap(_tmp);
		this->AutoIncressContent::Put(_tmp);
	}
	void Put(const ContentType::QWORD qData) override
	{
		ContentType::QWORD _tmp = qData;
		ByteSwap(_tmp);
		this->AutoIncressContent::Put(_tmp);
	}
	void Put(const std::string & str)
	{
		this->AutoIncressContent::Put((const ContentType::BYTE *)str.c_str(), str.length());
	}
	void Put(const std::string & str, unsigned int uLength)
	{
		ContentType::BYTE * ptmp = (ContentType::BYTE*)_alloca(uLength);
		memset(ptmp, '\0', uLength);
		memcpy(ptmp, str.c_str(), min(str.length(), uLength));
		this->AutoIncressContent::Put(ptmp, uLength);
	}
	template<typename T> bool Get(T & v)
	{
		v = (T)0;
		if ((this->ReadPosition + sizeof(v)) > this->Size())
			return false;
		v = *(T*)(this->Ptr() + this->ReadPosition);
		T __tv = v;
		ByteSwap(__tv);
		v = __tv;
		this->ReadPosition += sizeof(T);
		return true;
	}
	bool Get(std::string & v, unsigned int uLength)
	{
		if (uLength == -1)
			uLength = this->Size() - this->ReadPosition;
		if ((this->ReadPosition + uLength) > this->Size())
			return false;
		v = std::string((const char*)this->Ptr() + this->ReadPosition, uLength);
		this->ReadPosition += uLength;
		return true;
	}
	void Reset()
	{
		this->ReadPosition = 0;
		this->AutoIncressContent::Reset();
	}
};

/**
 * ָ��ӿ�
 * ���� 808/1078 ָ���඼Ӧ�þ��еĳ�Ա�ͽӿں���
 */
class cmd
{
public:
	JTT808AutoIncressContent content;
	virtual const unsigned short MessageID() const = 0;
	virtual void Compile() = 0;
	virtual bool Parse() = 0;
};

/**
 * ʵʱ����Ƶ��������
 */
class cmd0x9101 : public cmd
{
public:
	enum En_StreamType { ST_AudioVideo = 0, ST_Video = 1, ST_ImmeSpeak = 2, ST_Listen = 3, ST_Boardcast = 4, ST_Raw = 5 };
	enum En_CodecType { CT_MainCodec = 0, CT_SubCodec = 1 };
	std::string sIP;	// ������IP
	ContentType::WORD wTcpPort;		// ������ TCP �˿�
	ContentType::WORD wUdpPort;		// ������ UDP �˿�
	ContentType::BYTE byLogicChannel;// �߼�ͨ����
	ContentType::BYTE byStreamType;		// �������� En_StreamType ö��֮һ
	ContentType::BYTE byCodecType;	// �������� En_CodecType ö��֮һ
	const unsigned short MessageID() const override { return 0x9101; }
	void Compile() override;
	bool Parse() override;
};

/**
 * ����Ƶʵʱ�������
 */
class cmd0x9102 : public cmd
{
public:
	enum En_Command { CMD_Close = 0, CMD_Swith = 1, CMD_Pause = 2, CMD_Resume = 3, CMD_CloseImmeSpeak = 4 };
	enum En_AVCloseType { AVCT_All = 0, AVCT_Video = 1, AVCT_Audio = 2 };
	ContentType::BYTE byLogicChannel;// �߼�ͨ����
	ContentType::BYTE byCommand;		// ����ָ�� En_Command ö��֮һ
	ContentType::BYTE byCloseType;	// �ر�����Ƶ���� En_AVCloseType ö��֮һ
	ContentType::BYTE bySwitchType;	// �л��������� En_CodecType ö��֮һ
	const unsigned short MessageID() const override { return 0x9102; }
	void Compile() override;
	bool Parse() override;
};

class cmd0x0001 : public cmd
{
public:
	enum En_CommonResponseType { CRT_OK = 0, CRT_Fail = 1, CRT_Error = 2, CRT_Unsupport = 3 };
	ContentType::WORD wResponseFloatID;		// Ӧ����ˮ��
	ContentType::WORD wResponseMessageID;	// Ӧ��ID
	ContentType::BYTE byResult;				// ��� En_CommonResponseType ö��֮һ
	const unsigned short MessageID() const override { return 0x0001; }
	void Compile() override;
	bool Parse() override;
};

class cmd0x0002 : public cmd
{
public:
	const unsigned short MessageID() const override { return 0x0002; }
	void Compile() override;
	bool Parse() override;
};

class cmd0x0100 : public cmd
{
public:
	ContentType::WORD wProvince;		// ʡ��ID
	ContentType::WORD wCity;			// ����ID
	std::string sProductor;		// ������ID
	std::string sModel;			// �ն��ͺ�
	std::string sTerminalID;	// �ն�ID
	ContentType::BYTE byPlateColor;			// ������ɫ
	std::string sPlateID;		// ���Ʊ�ʶ
	const unsigned short MessageID() const override { return 0x0100; }
	void Compile() override;
	bool Parse() override;
};

class cmd0x8100 : public cmd
{
public:
	ContentType::WORD wResponseID;
	ContentType::BYTE byResult;
	std::string sAuthCode;
	const unsigned short MessageID() const override { return 0x8100; }
	void Compile() override;
	bool Parse() override;
};

class cmd0x0102 : public cmd
{
public:
	std::string sAuthCode;
	const unsigned short MessageID() const override { return 0x0102; }
	void Compile() override;
	bool Parse() override;
};

class cmd0x8001 : public cmd
{
public:
	ContentType::WORD wResponseID;
	ContentType::WORD wMessageID;
	ContentType::BYTE byResult;
	const unsigned short MessageID() const override { return 0x8001; }
	void Compile() override;
	bool Parse() override;
};

class cmd0x0200 : public cmd
{
public:
	ContentType::DWORD dAlarm;
	ContentType::DWORD dStatus;
	ContentType::DWORD dLatitude;
	ContentType::DWORD dLongitude;
	ContentType::WORD wAltitude;
	ContentType::WORD wSpeed;
	ContentType::WORD wDirector;
	unsigned char sTime[6];
	const unsigned short MessageID() const override { return 0x0200; }
	void Compile() override;
	bool Parse() override;
};

class CommandClasses
{
public:
	CommandClasses();
	~CommandClasses();
};

