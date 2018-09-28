#pragma once
#include "AutoIncressContent.h"
#include "CommandClasses.h"
#include "JTT808Head.h"

class JTT808Packer
{
private:
	void PutByte(ContentType::BYTE byData);
public:
	JTT808Packer();
	~JTT808Packer();

	AutoIncressContent result;
	/**
	 * 打包器 该函数一般由指令发送函数调用
	 * 将包头、数据体按照 808 协议描述的过程打包为原始数据
	 * 打包的结果数据存储在 result 中
	 * @param head : JTT808Head 包头设置
	 * @param pData : const BYTE * 指令参数
	 * @param uLength : unsigned int 参数长度
	 */
	void Encoder(JTT808Head & head, const ContentType::BYTE * pData, unsigned int uLength);
	/**
	 * 解包器 该函数一般由数据接收器调用
	 * 将原始数据按照 808 协议描述的过程解包为包头、数据体
	 * 如果校验失败或者其它原因导致的解析失败则返回 false，否则返回 true
	 * 解析的参数数据存储在 result 中
	 * @param head : JTT808Head 包头设置
	 * @param pData : const BYTE * 数据指针
	 * @param uLength : unsigned int 数据长度
	 */
	bool Decoder(JTT808Head & head, const ContentType::BYTE * pData, unsigned int uLength);
};

