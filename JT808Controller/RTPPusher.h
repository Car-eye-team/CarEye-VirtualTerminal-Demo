#pragma once
#include <string>
#include <thread>
#include <condition_variable>
#include "HPSocket/HPSocket.h"

/**
 * RTP ������ ÿ�� SIM+ͨ�� ���ǵ�����ʵ��
 */
class RTPPusher : ITcpClientListener
{
private:
	CTcpClientPtr client;
	std::thread * ptPusher = nullptr;
	unsigned int uStreamType = 0;
	unsigned int uCodecType = 0;
	BYTE PhoneNumber[6] = { 0 };
	unsigned int LogicChannel = 0;
	bool VideoEnable = true, AudioEnable = true;
	const char * fn;
	// ----------- ���ƹ����ı��� --------
	bool Paused = false;
	std::mutex PauseMutex;
	std::condition_variable PauseCV;
	// -----------------------------------
	void PushProc();
	EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override;
	EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override;
	EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override;
	EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
	EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
	EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) override;
	EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;
public:
	std::string IP;
	unsigned short Port;
	RTPPusher(const unsigned char sPhoneNumber[], const unsigned int uLogicChannel, const char * datafile);
	~RTPPusher();
	/**
	 * ��������Ŀ�겢ֹͣԭ���Ĺ���
	 */
	void UpdateTarget(const std::string & IP, unsigned short Port, unsigned int uStreamType, unsigned int uCodecType);
	void CloseAudio();
	void CloseVideo();
	void Close();
	void ChangeCodec(unsigned int uCodecType);
	void Pause();
	void Resume();
};

