#pragma once
#include "AutoIncressContent.h"

/**
 * ���� JT/T 808 ��׼�İ�ͷ���ݽṹ������������
 */
class JTT808Head
{
public:
	/**
	 * ���ܷ�ʽö��
	 */
	enum En_EncryptType { ET_NONE = 0b000, ET_RSA = 0b001 };
	/**
	 * ��ϢID
	 */
	ContentType::WORD MessageID = 0;
	/**
	 * �ְ����
	 */
	bool SubpackFlag = false;
	/**
	 * ���ܷ�ʽ
	 */
	En_EncryptType EncryptType = ET_NONE;
	/**
	 * �绰����
	 */
	ContentType::BYTE PhoneNumber[16] = { 0 };
	/**
	 * ��Ϣ���
	 */
	ContentType::WORD MessageSeq = 0;
	/**
	 * ��Ϣ���ݳ���
	 */
	ContentType::DWORD MessageLength = 0;
	/**
	 * �ְ�����
	 */
	ContentType::WORD SubpackCount = 0;
	/**
	 * �ְ����
	 */
	ContentType::WORD SubpackSequence = 0;
	/**
	 * ԭʼ����
	 */
	AutoIncressContent RawHeadData;
	JTT808Head();
	~JTT808Head();
	/**
	 * ͨ������Ա������ϳ�ԭʼ���ݣ������ˮ�Żᱻ�Զ�����
	 * ԭ�����ݽ��ᱻ���
	 */
	void MakeHead();
	/**
	 * ͨ������ԭʼ���ݵõ�����Ա����
	 */
	bool ParseHead();
};

