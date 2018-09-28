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
 * 指令接口
 * 所有 808/1078 指令类都应该具有的成员和接口函数
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
 * 实时音视频传输请求
 */
class cmd0x9101 : public cmd
{
public:
	enum En_StreamType { ST_AudioVideo = 0, ST_Video = 1, ST_ImmeSpeak = 2, ST_Listen = 3, ST_Boardcast = 4, ST_Raw = 5 };
	enum En_CodecType { CT_MainCodec = 0, CT_SubCodec = 1 };
	std::string sIP;	// 服务器IP
	ContentType::WORD wTcpPort;		// 服务器 TCP 端口
	ContentType::WORD wUdpPort;		// 服务器 UDP 端口
	ContentType::BYTE byLogicChannel;// 逻辑通道号
	ContentType::BYTE byStreamType;		// 数据类型 En_StreamType 枚举之一
	ContentType::BYTE byCodecType;	// 码流类型 En_CodecType 枚举之一
	const unsigned short MessageID() const override { return 0x9101; }
	void Compile() override;
	bool Parse() override;
};

/**
 * 音视频实时传输控制
 */
class cmd0x9102 : public cmd
{
public:
	enum En_Command { CMD_Close = 0, CMD_Swith = 1, CMD_Pause = 2, CMD_Resume = 3, CMD_CloseImmeSpeak = 4 };
	enum En_AVCloseType { AVCT_All = 0, AVCT_Video = 1, AVCT_Audio = 2 };
	ContentType::BYTE byLogicChannel;// 逻辑通道号
	ContentType::BYTE byCommand;		// 控制指令 En_Command 枚举之一
	ContentType::BYTE byCloseType;	// 关闭音视频类型 En_AVCloseType 枚举之一
	ContentType::BYTE bySwitchType;	// 切换码流类型 En_CodecType 枚举之一
	const unsigned short MessageID() const override { return 0x9102; }
	void Compile() override;
	bool Parse() override;
};

class cmd0x0001 : public cmd
{
public:
	enum En_CommonResponseType { CRT_OK = 0, CRT_Fail = 1, CRT_Error = 2, CRT_Unsupport = 3 };
	ContentType::WORD wResponseFloatID;		// 应答流水号
	ContentType::WORD wResponseMessageID;	// 应答ID
	ContentType::BYTE byResult;				// 结果 En_CommonResponseType 枚举之一
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
	ContentType::WORD wProvince;		// 省份ID
	ContentType::WORD wCity;			// 市县ID
	std::string sProductor;		// 制造商ID
	std::string sModel;			// 终端型号
	std::string sTerminalID;	// 终端ID
	ContentType::BYTE byPlateColor;			// 车牌颜色
	std::string sPlateID;		// 车牌标识
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

