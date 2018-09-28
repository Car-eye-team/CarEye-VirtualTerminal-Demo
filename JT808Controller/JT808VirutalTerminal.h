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
	 * @param SimCard : const char* ���������ն��ϵ�ID����
	 * @param datafile : const char*[] �������ն����󶨵��ļ���
	 * @param files : unsigned int ���ļ�����
	 */
	JT808VirtualTerminal(const char * SimCard, const char * datafile[], unsigned int files);
	~JT808VirtualTerminal();
	/**
	 * ���������ն�
	 * һ���������������������ӣ���������������
	 * @param IP : std::string ����̨��ַ
	 * @param Port : unsigned short ����̨�˿�
	 */
	void Start(const std::string IP, unsigned short Port);
};
