#pragma once
#include "AutoIncressContent.h"

/**
 * 符合 JT/T 808 标准的包头数据结构及打包拆包程序
 */
class JTT808Head
{
public:
	/**
	 * 加密方式枚举
	 */
	enum En_EncryptType { ET_NONE = 0b000, ET_RSA = 0b001 };
	/**
	 * 消息ID
	 */
	ContentType::WORD MessageID = 0;
	/**
	 * 分包标记
	 */
	bool SubpackFlag = false;
	/**
	 * 加密方式
	 */
	En_EncryptType EncryptType = ET_NONE;
	/**
	 * 电话号码
	 */
	ContentType::BYTE PhoneNumber[16] = { 0 };
	/**
	 * 消息序号
	 */
	ContentType::WORD MessageSeq = 0;
	/**
	 * 消息内容长度
	 */
	ContentType::DWORD MessageLength = 0;
	/**
	 * 分包总数
	 */
	ContentType::WORD SubpackCount = 0;
	/**
	 * 分包序号
	 */
	ContentType::WORD SubpackSequence = 0;
	/**
	 * 原始数据
	 */
	AutoIncressContent RawHeadData;
	JTT808Head();
	~JTT808Head();
	/**
	 * 通过各成员变量组合成原始数据，封包流水号会被自动设置
	 * 原有数据将会被清空
	 */
	void MakeHead();
	/**
	 * 通过解析原始数据得到各成员变量
	 */
	bool ParseHead();
};

