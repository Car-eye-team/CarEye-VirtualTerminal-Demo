// JT808Controller.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "JT808Controller.h"
#include "JT808VirutalTerminal.h"
#include "JTT808Packer.h"
#include "utils.h"
#include <cassert>
#ifndef _MSC_VER
#	include <unistd.h>
#	define Sleep(ms) usleep(ms * 1000)
#endif

Terminal::Terminal(const char * SimCard, const char * datafile[], unsigned int files) : client(this)
{
	String2BCD(SimCard, this->abyPhoneNumberBCD);
	this->DataFileList = new std::vector<std::string>();
	for (unsigned int i = 0; i < files; i++)
	{
		std::string sfn(datafile[i], strlen(datafile[i]));
		this->DataFileList->push_back(sfn);
	}
}

Terminal::~Terminal()
{
	delete this->DataFileList;
	for (auto push : this->PusherSet)
	{
		delete push.second;
	}
	if (this->pthHeartThread)
	{
		if (this->pthHeartThread->joinable())
			this->pthHeartThread->join();
		delete this->pthHeartThread;
	}
}

void Terminal::Start(const std::string IP, unsigned short Port)
{
	this->ConsoleAddress = { IP, Port };
	this->pthConnectThread = new std::thread(&Terminal::ConnectConsole, this);
}

void Terminal::ConnectConsole()
{
	std::string IP;
	unsigned short Port;
	std::tie(IP, Port) = this->ConsoleAddress;
	while (this->client->Start((LPCTSTR)IP.c_str(), Port, 0) == false)
	{
		Sleep(3 * 1000);
	}
}

void Terminal::HeartBeats()
{
	cmd0x0002 heart;
	while (this->client->IsConnected())
	{
		this->SendCommand(&heart);
		Sleep(60 * 1000);
	}
}

BYTE* Terminal::SearchPackageFlag(const BYTE* pData, unsigned uStart, unsigned uLength)
{
	for (unsigned int uPosition = uStart; uPosition < uLength; uPosition++)
	{
		if (pData[uPosition] == 0x7E)
			return (BYTE*)pData + uPosition;
	}
	return nullptr;
}

bool Terminal::CommandSwitch(JTT808Head& head, AutoIncressContent & content)
{
	BYTE PhoneBCD[6] = { 0 };
	String2BCD((const char*)head.PhoneNumber, PhoneBCD);
	if (memcmp(PhoneBCD, this->abyPhoneNumberBCD, 6) != 0)
		return false;
	switch (head.MessageID)
	{
	case 0x9101:
	{
		cmd0x9101 request;
		request.content = content;
		if (request.Parse())
		{
			if (request.byLogicChannel > this->DataFileList->size())
				break;
			if (this->PusherSet.find(request.byLogicChannel) == this->PusherSet.end())
			{
				RTPPusher * p = new RTPPusher(head.PhoneNumber, request.byLogicChannel, this->DataFileList->operator[](request.byLogicChannel - 1).c_str());
				this->PusherSet.insert(std::make_pair(request.byLogicChannel, p));
			}
			RTPPusher * pusher = this->PusherSet[request.byLogicChannel];
			pusher->UpdateTarget(request.sIP, request.wTcpPort, request.byStreamType, request.byCodecType);
			cmd0x0001 response;
			response.wResponseMessageID = head.MessageID;
			response.wResponseFloatID = head.MessageSeq;
			response.byResult = 0;
			this->SendCommand(&response);
		}
		break;
	}
	case 0x9102:
	{
		cmd0x9102 request;
		request.content = content;
		if (request.Parse())
		{
			if (this->PusherSet.find(request.byLogicChannel) == this->PusherSet.end())
				break;
			RTPPusher * pusher = this->PusherSet[request.byLogicChannel];
			switch (request.byCommand)
			{
			case cmd0x9102::En_Command::CMD_Close:
			{
				switch (request.byCloseType)
				{
				case cmd0x9102::En_AVCloseType::AVCT_All: pusher->Close(); break;
				case cmd0x9102::En_AVCloseType::AVCT_Audio: pusher->CloseAudio(); break;
				case cmd0x9102::En_AVCloseType::AVCT_Video: pusher->CloseVideo(); break;
				default: assert(false);
				}
				break;
			}
			case cmd0x9102::En_Command::CMD_Swith: pusher->ChangeCodec(request.bySwitchType); break;
			case cmd0x9102::En_Command::CMD_Pause: pusher->Pause(); break;
			case cmd0x9102::En_Command::CMD_Resume: pusher->Resume(); break;
			case cmd0x9102::En_Command::CMD_CloseImmeSpeak: break;
			}
			cmd0x0001 response;
			response.wResponseMessageID = head.MessageID;
			response.wResponseFloatID = head.MessageSeq;
			response.byResult = 0;
			this->SendCommand(&response);
		}
		break;
	}
	case 0x8100:
	{
		cmd0x8100 request;
		request.content = content;
		if (request.Parse())
		{
			if (request.byResult == 0 && this->AuthenticationCode.length() == 0)
			{
				this->AuthenticationCode = request.sAuthCode;
				cmd0x0102 authreq;
				authreq.sAuthCode = this->AuthenticationCode;
				printf("收到鉴权码：");
				PrintHex(this->AuthenticationCode.c_str(), this->AuthenticationCode.length());
				putc('\n', stdout);
				this->SendCommand(&authreq);
			}
		}
		break;
	}
	case 0x8001:
	{
		cmd0x8001 commresp;
		commresp.content = content;
		if (commresp.Parse())
		{
			printf("通用应答：流水号 %d   消息ID 0x%04X   结果 %d\n", commresp.wResponseID, commresp.wMessageID, commresp.byResult);
			if (commresp.wMessageID == 0x0102 && !this->login)
			{
				this->login = true;
				printf("鉴权成功！\n");
				this->pthHeartThread = new std::thread(&Terminal::HeartBeats, this);
				cmd0x0200 position;
				position.dLatitude = (unsigned long)(28.193611 * pow(10, 6));
				position.dLongitude = (unsigned long)(113.000338 * pow(10, 6));
				this->SendCommand(&position);
			}
		}
	}
	}
	return true;
}

bool Terminal::SendCommand(cmd * command)
{
	JTT808Head head;
	printf("发送 0x%04X 请求指令\n", command->MessageID());
	command->Compile();
	BCD2String(this->abyPhoneNumberBCD, 6, (char*)head.PhoneNumber);
	head.EncryptType = JTT808Head::ET_NONE;
	head.MessageID = command->MessageID();
	head.SubpackFlag = false;
	head.MessageSeq = this->AllocMessageSeq++;
	this->packer.Encoder(head, command->content.Ptr(), command->content.Size());
	this->client->Send(this->packer.result.Ptr(), this->packer.result.Size());
	return true;
}

enum EnHandleResult Terminal::OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) { return EnHandleResult::HR_OK; }
EnHandleResult Terminal::OnHandShake(ITcpClient* pSender, CONNID dwConnID) { return EnHandleResult::HR_OK; }
EnHandleResult Terminal::OnConnect(ITcpClient* pSender, CONNID dwConnID)
{
	printf("已连接到控制台\n");
	// 开始进行终端注册
	cmd0x0100 regreq;
	regreq.wProvince = 43;
	regreq.wCity = 103;
	regreq.sProductor = "CR-EY";
	regreq.sModel = "CAR-EYE-DEMO-VM-0";
	regreq.sTerminalID = "A102110";
	regreq.byPlateColor = 1;
	regreq.sPlateID = "A000001";
	this->SendCommand(&regreq);
	return EnHandleResult::HR_OK;
}
EnHandleResult Terminal::OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	puts("send -> ");
	PrintHex((const char*)pData, iLength);
	puts("\n");
	return EnHandleResult::HR_OK;
}
EnHandleResult Terminal::OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	puts("recv <- ");
	PrintHex((const char*)pData, iLength);
	puts("\n");
	for (int i = 0; i < iLength; i++)
	{
		BYTE ch = pData[i];
		if (this->buff.Size() > 0)	// 只有当缓存中有数据的时候才会接收
		{
			this->buff.Put(ch);
			if (ch == 0x7E)
			{
				this->packer.result = this->buff;
				JTT808Head head;
				if (this->packer.Decoder(head, this->buff.Ptr(), this->buff.Size()))
				{
					this->CommandSwitch(head, this->packer.result);
				}
				this->buff.Reset();
			}
		}
		else if (ch == 0x7E)	// 由第一个 0x7E 开启接收
		{
			this->buff.Reset();
			this->buff.Put(ch);
		}
	}
	return EnHandleResult::HR_OK;
}
EnHandleResult Terminal::OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) { return EnHandleResult::HR_OK; }
EnHandleResult Terminal::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	printf("断线重连……\n");
	if (this->pthConnectThread)
	{
		this->pthConnectThread->join();
		delete this->pthConnectThread;
	}
	this->pthConnectThread = new std::thread(&Terminal::ConnectConsole, this);
	return EnHandleResult::HR_OK;
}

JT808VirtualTerminal::JT808VirtualTerminal(const char * SimCard, const char * datafile[], unsigned int files)
{
	this->VirtualTerminalPtr = new Terminal(SimCard, datafile, files);
}

JT808VirtualTerminal::~JT808VirtualTerminal()
{
	if (this->VirtualTerminalPtr)
		delete this->VirtualTerminalPtr;

}

void JT808VirtualTerminal::Start(const std::string IP, unsigned short Port)
{
	if (this->VirtualTerminalPtr)
		this->VirtualTerminalPtr->Start(IP, Port);
}