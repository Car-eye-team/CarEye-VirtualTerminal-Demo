#include "stdafx.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

void BCD2String(const unsigned char* const BCDCode, const unsigned BCDLength, char* str)
{
	for (unsigned int i = 0; i < BCDLength; i++)
	{
		str[i * 2] = ((BCDCode[i] >> 4) & 0xF) + '0';
		str[i * 2 + 1] = (BCDCode[i] & 0xF) + '0';
	}
	str[BCDLength * 2] = '\0';
}

void String2BCD(const char* const str, unsigned char* BCDCode)
{
	unsigned int pos = 0;
	if (strlen(str) % 2 == 1)
	{
		BCDCode[pos] = str[pos] - '0';
		pos++;
	}
	while (pos < (strlen(str) / 2))
	{
		BCDCode[pos] = ((str[pos * 2] - '0') << 4) | (str[pos * 2 + 1] - '0');
		pos++;
	}
}

void PrintHex(const char * ptr, unsigned long length)
{
	for (unsigned long i = 0; i < length; i++)
	{
		printf("%02hhX ", (unsigned char)ptr[i]);
	}
}
