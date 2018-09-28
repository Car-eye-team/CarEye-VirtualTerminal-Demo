#pragma once
#include <tuple>
#include <thread>
#include <map>
#include <vector>
#include <string>
#include <atomic>
#include "CommandClasses.h"
#include "HPSocket/HPSocket.h"
#include "BlockingRingBuffer.h"
#include "JTT808Packer.h"
#include "RTPPusher.h"

/**
 * �����ն�
 * һ��ʵ������һ���ն�
 * һ���ն���������ж���豸ͨ��
 */
class Terminal : ITcpClientListener
{
private:
	CTcpClientPtr client;
	WORD AllocMessageSeq = 0;
	BYTE abyPhoneNumberBCD[6];
	std::atomic_bool login;
	std::vector<std::string> * DataFileList;
	std::map<unsigned int, RTPPusher*> PusherSet;	// RTP ��������
	std::tuple<std::string, unsigned short> ConsoleAddress = { "", 0 };
	std::thread * pthConnectThread = nullptr;	// �����߳�ָ��
	std::thread * pthHeartThread = nullptr;
	std::string AuthenticationCode = "";		// ��Ȩ��
	AutoIncressContent buff;// ���ý��ջ���
	JTT808Packer packer;	// ���ô����
	/**
	 * ��������ָ�����ֱ̨���ɹ�
	 * ������ʧ����ȴ� 3 ���������
	 */
	void ConnectConsole();
	/**
	 * ÿ�� 60 �������
	 */
	void HeartBeats();
	/**
	 * ����ָ�����ݰ���Ϣ��־λ
	 * �ҵ�����ָ������ݵ�ָ�룬���򷵻� nullptr
	 * @param pData : const BYTE * ������������
	 * @param uStart : unsigned int ��ʼλ��
	 * @param uLength : unsigned int ���ݳ���
	 */
	BYTE * SearchPackageFlag(const BYTE * pData, unsigned int uStart, unsigned int uLength);
	/**
	 * ָ��ִ��
	 * @param head : JTT808Head �Ѿ���������ͷ�ṹ
	 * @param content : AutoIncressContent δ��������ָ�����
	 */
	bool CommandSwitch(JTT808Head & head, AutoIncressContent & content);
	/**
	 * ����ָ��
	 */
	bool SendCommand(cmd * command);
	EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;
	EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override;
	EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override;
	EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override;
	EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
	EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
	EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) override;
public:
	/**
	 * @param SimCard : const char* ���������ն��ϵ�ID����
	 * @param datafile : const char*[] �������ն����󶨵��ļ���
	 * @param files : unsigned int ���ļ�����
	 */
	Terminal(const char * SimCard, const char * datafile[], unsigned int files);
	~Terminal();
	/**
	 * ���������ն�
	 * һ���������������������ӣ���������������
	 * @param IP : std::string ����̨��ַ
	 * @param Port : unsigned short ����̨�˿�
	 */
	void Start(const std::string IP, unsigned short Port);

};
