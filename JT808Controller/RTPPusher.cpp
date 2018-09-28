#include "stdafx.h"
#include "utils.h"
#include "RTPPusher.h"
#include <string.h>

RTPPusher::RTPPusher(const unsigned char sPhoneNumber[], const unsigned int uLogicChannel, const char * datafile) : client(this), LogicChannel(uLogicChannel), fn(datafile)
{
	String2BCD((const char*)sPhoneNumber, this->PhoneNumber);
}


RTPPusher::~RTPPusher()
{
	this->Close();
}

void RTPPusher::UpdateTarget(const std::string& IP, unsigned short Port, unsigned int uStreamType, unsigned int uCodecType)
{
	this->IP = IP;
	this->Port = Port;
	this->uStreamType = uStreamType;
	this->uCodecType = uCodecType;
	if (this->client->IsConnected())
	{
		this->client->Stop();
		this->ptPusher->join();
	}
	if (this->client->Start((LPCTSTR)IP.c_str(), Port))
	{
		this->AudioEnable = this->VideoEnable = true;
		this->ptPusher = new std::thread(&RTPPusher::PushProc, this);
	}
}

void RTPPusher::ChangeCodec(unsigned uCodecType)
{
	this->uCodecType = uCodecType;
}

void RTPPusher::PushProc()
{
	// FILE *dump;
	// fopen_s(&dump, "dump.bin", "wb");
	FILE * fp = nullptr;
#ifdef _MSC_VER
	fopen_s(&fp, this->fn, "rb");
#else
	fp = fopen(this->fn, "rb");
#endif
	std::unique_lock<std::mutex> lock(this->PauseMutex);
	if (fp)
	{
		BYTE pack[30 + 950];
		while (this->client->IsConnected() && !feof(fp))
		{
			this->PauseCV.wait(lock, [this] { return !this->Paused; });
			BYTE * head = pack, *payload = nullptr;
			if (fread(head, 16, 1, fp) == 0) break;
			int dt = (head[15] >> 4) & 0b1111;
			int head_size[] = { 30, 30, 30, 26, 18 };
			fread(head + 16, head_size[dt] - 16, 1, fp);
			payload = pack + head_size[dt];
			WORD payloadLength = *(payload - 2) << 8 | *(payload - 1);
			fread(payload, payloadLength, 1, fp);
			unsigned long plen = (unsigned long)(payload - pack + payloadLength);
			memcpy(pack + 8, this->PhoneNumber, 6);	// Ìæ»» SIM ¿¨ºÅ
			pack[14] = (BYTE)this->LogicChannel;	// Ìæ»»Âß¼­Í¨µÀºÅ
			if ((this->VideoEnable && head_size[dt] == 30) || (this->AudioEnable && head_size[dt] == 26) || (head_size[dt] == 18))
			{
				this->client->Send(pack, plen);
				printf("Send %d bytes\n", plen);
			}
			// fseek(dump, 0, SEEK_END);
			// fwrite(pack, plen, 1, dump);
		}
		fclose(fp);
		// fclose(dump);
	}
	this->client->Stop();
}

void RTPPusher::Close()
{
	this->client->Stop();
	if (this->ptPusher)
	{
		if (this->ptPusher->joinable())
		{
			this->ptPusher->join();
		}
		delete this->ptPusher;
		this->ptPusher = nullptr;
	}
}

void RTPPusher::CloseAudio()
{
	this->AudioEnable = false;
}

void RTPPusher::CloseVideo()
{
	this->VideoEnable = false;
}

void RTPPusher::Pause()
{
	this->Paused = true;
}

void RTPPusher::Resume()
{
	this->Paused = false;
	this->PauseCV.notify_all();
}

EnHandleResult RTPPusher::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) { return EnHandleResult::HR_OK; }
EnHandleResult RTPPusher::OnConnect(ITcpClient* pSender, CONNID dwConnID) { return EnHandleResult::HR_OK; }
EnHandleResult RTPPusher::OnHandShake(ITcpClient* pSender, CONNID dwConnID) { return EnHandleResult::HR_OK; }
EnHandleResult RTPPusher::OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) { return EnHandleResult::HR_OK; }
EnHandleResult RTPPusher::OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return EnHandleResult::HR_OK; }
EnHandleResult RTPPusher::OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) { return EnHandleResult::HR_OK; }
EnHandleResult RTPPusher::OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return EnHandleResult::HR_OK; }
