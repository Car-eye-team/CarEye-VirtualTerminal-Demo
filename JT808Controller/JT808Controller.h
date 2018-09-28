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
 * 虚拟终端
 * 一个实例代表一个终端
 * 一个终端下面可以有多个设备通道
 */
class Terminal : ITcpClientListener
{
private:
	CTcpClientPtr client;
	WORD AllocMessageSeq = 0;
	BYTE abyPhoneNumberBCD[6];
	std::atomic_bool login;
	std::vector<std::string> * DataFileList;
	std::map<unsigned int, RTPPusher*> PusherSet;	// RTP 推流器池
	std::tuple<std::string, unsigned short> ConsoleAddress = { "", 0 };
	std::thread * pthConnectThread = nullptr;	// 连接线程指针
	std::thread * pthHeartThread = nullptr;
	std::string AuthenticationCode = "";		// 鉴权码
	AutoIncressContent buff;// 内置接收缓存
	JTT808Packer packer;	// 内置打包器
	/**
	 * 持续连接指令控制台直到成功
	 * 若连接失败则等待 3 秒继续连接
	 */
	void ConnectConsole();
	/**
	 * 每隔 60 秒的心跳
	 */
	void HeartBeats();
	/**
	 * 查找指令数据包消息标志位
	 * 找到返回指向该数据的指针，否则返回 nullptr
	 * @param pData : const BYTE * 被搜索的数据
	 * @param uStart : unsigned int 起始位置
	 * @param uLength : unsigned int 数据长度
	 */
	BYTE * SearchPackageFlag(const BYTE * pData, unsigned int uStart, unsigned int uLength);
	/**
	 * 指令执行
	 * @param head : JTT808Head 已经解析过的头结构
	 * @param content : AutoIncressContent 未被解析的指令参数
	 */
	bool CommandSwitch(JTT808Head & head, AutoIncressContent & content);
	/**
	 * 发送指令
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
	 * @param SimCard : const char* 绑定在虚拟终端上的ID卡号
	 * @param datafile : const char*[] 该虚拟终端所绑定的文件组
	 * @param files : unsigned int 绑定文件个数
	 */
	Terminal(const char * SimCard, const char * datafile[], unsigned int files);
	~Terminal();
	/**
	 * 启动虚拟终端
	 * 一旦被启动则会持续尝试连接，函数会立即返回
	 * @param IP : std::string 控制台地址
	 * @param Port : unsigned short 控制台端口
	 */
	void Start(const std::string IP, unsigned short Port);

};
