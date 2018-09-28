#pragma once
#include "JT808Controller.h"

#if defined _WIN32 || defined __CYGWIN__
#	ifdef JT808CONTROLLER_EXPORTS
#		ifdef __GNUC__
#			define DLL_PUBLIC __attribute__ ((dllexport))
#		else
			// Note: actually gcc seems to also supports this syntax.
#			define DLL_PUBLIC __declspec(dllexport)
#		endif
#	else
#		ifdef __GNUC__
#			define DLL_PUBLIC __attribute__ ((dllimport))
#		else
			// Note: actually gcc seems to also supports this syntax.
#			define DLL_PUBLIC __declspec(dllimport)
#		endif
#	endif
#	define DLL_LOCAL
#else
#	if __GNUC__ >= 4
#		define DLL_PUBLIC __attribute__ ((visibility ("default")))
#		define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#	else
#		define DLL_PUBLIC
#		define DLL_LOCAL
#	endif
#endif

class DLL_PUBLIC JT808VirtualTerminal
{
private:
	Terminal * VirtualTerminalPtr = nullptr;
public:
	/**
	 * @param SimCard : const char* 绑定在虚拟终端上的ID卡号
	 * @param datafile : const char*[] 该虚拟终端所绑定的文件组
	 * @param files : unsigned int 绑定文件个数
	 */
	JT808VirtualTerminal(const char * SimCard, const char * datafile[], unsigned int files);
	~JT808VirtualTerminal();
	/**
	 * 启动虚拟终端
	 * 一旦被启动则会持续尝试连接，函数会立即返回
	 * @param IP : std::string 控制台地址
	 * @param Port : unsigned short 控制台端口
	 */
	void Start(const std::string IP, unsigned short Port);
};
