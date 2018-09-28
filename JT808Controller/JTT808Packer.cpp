#include "stdafx.h"
#include <malloc.h>
#include "JTT808Packer.h"
#include <cassert>

using namespace ContentType;

JTT808Packer::JTT808Packer()
{
}


JTT808Packer::~JTT808Packer()
{
}

void JTT808Packer::PutByte(BYTE byData)
{
	if (byData == 0x7D)
	{
		this->result.Put((BYTE)0x7D);
		this->result.Put((BYTE)0x01);
	}
	else if (byData == 0x7E)
	{
		this->result.Put((BYTE)0x7D);
		this->result.Put((BYTE)0x02);
	}
	else
	{
		this->result.Put(byData);
	}
}

void JTT808Packer::Encoder(JTT808Head & head, const BYTE* pData, unsigned uLength)
{
	this->result.Reset();
	this->result.Put((BYTE)0x7E);
	BYTE byCheckCode = 0x00;
	head.MessageLength = uLength;
	head.MakeHead();
	BYTE *hptr = head.RawHeadData.Ptr();
	for (unsigned int i = 0; i < head.RawHeadData.Size(); i++)
	{
		byCheckCode ^= hptr[i];
		this->PutByte(hptr[i]);
	}
	for (unsigned int i = 0; i < uLength; i++)
	{
		byCheckCode ^= pData[i];
		this->PutByte(pData[i]);
	}
	this->PutByte(byCheckCode);
	this->result.Put((BYTE)0x7E);
}

bool JTT808Packer::Decoder(JTT808Head & head, const BYTE* pData, unsigned uLength)
{
	this->result.Reset();
	head.RawHeadData.Reset();
	if (pData[0] != 0x7E || pData[uLength - 1] != 0x7E)
	{
		assert(false);
		return false;
	}
	BYTE * temp = (BYTE*)_alloca(uLength);
	unsigned int PayloadLength = 0;
	BYTE checkCode = 0x00;
	// ½âÂë
	for (unsigned int i = 1; i < (uLength - 1); i++, PayloadLength++)
	{
		if (pData[i] == 0x7D && pData[i + 1] == 0x01)
		{
			temp[PayloadLength] = 0x7D;
			i++;
		}
		else if (pData[i] == 0x7D && pData[i + 1] == 0x02)
		{
			temp[PayloadLength] = 0x7E;
			i++;
		}
		else
		{
			temp[PayloadLength] = pData[i];
		}
		checkCode ^= temp[PayloadLength];
	}
	// Ð£Ñé
	if (checkCode != 0)
	{
		assert(false);
		return false;
	}
	head.RawHeadData.Put(temp, 16);
	if (!head.ParseHead())
	{
		assert(false);
		return false;
	}
	unsigned int HeadLength = (head.SubpackFlag) ? 16 : 12;
	PayloadLength -= HeadLength;
	this->result.Put(temp + HeadLength, PayloadLength - 1);
	return true;
}
