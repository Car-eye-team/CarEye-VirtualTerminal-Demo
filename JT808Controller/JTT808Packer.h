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
	 * ����� �ú���һ����ָ��ͺ�������
	 * ����ͷ�������尴�� 808 Э�������Ĺ��̴��Ϊԭʼ����
	 * ����Ľ�����ݴ洢�� result ��
	 * @param head : JTT808Head ��ͷ����
	 * @param pData : const BYTE * ָ�����
	 * @param uLength : unsigned int ��������
	 */
	void Encoder(JTT808Head & head, const ContentType::BYTE * pData, unsigned int uLength);
	/**
	 * ����� �ú���һ�������ݽ���������
	 * ��ԭʼ���ݰ��� 808 Э�������Ĺ��̽��Ϊ��ͷ��������
	 * ���У��ʧ�ܻ�������ԭ���µĽ���ʧ���򷵻� false�����򷵻� true
	 * �����Ĳ������ݴ洢�� result ��
	 * @param head : JTT808Head ��ͷ����
	 * @param pData : const BYTE * ����ָ��
	 * @param uLength : unsigned int ���ݳ���
	 */
	bool Decoder(JTT808Head & head, const ContentType::BYTE * pData, unsigned int uLength);
};

