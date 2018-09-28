#include "stdafx.h"
#include "JTT808Head.h"
#include "utils.h"
#include <string>

using namespace ContentType;

JTT808Head::JTT808Head()
{
}


JTT808Head::~JTT808Head()
{
}

void JTT808Head::MakeHead()
{
	this->RawHeadData.Reset();
	this->RawHeadData.Put(((BYTE*)&this->MessageID)[1]);
	this->RawHeadData.Put(((BYTE*)&this->MessageID)[0]);
	
	WORD HeadAttribute = 0;
	if (this->SubpackFlag) HeadAttribute |= 0b1;

	HeadAttribute <<= 3;
	HeadAttribute |= this->EncryptType & 0b111;

	HeadAttribute <<= 10;
	HeadAttribute |= this->MessageLength & 0b11111111111;

	this->RawHeadData.Put(((BYTE*)&HeadAttribute)[1]);
	this->RawHeadData.Put(((BYTE*)&HeadAttribute)[0]);

	BYTE phonebcd[6] = { 0 };
	String2BCD((const char * const)this->PhoneNumber, phonebcd);
	this->RawHeadData.Put(phonebcd, 6);

	this->RawHeadData.Put(((BYTE*)&this->MessageSeq)[1]);
	this->RawHeadData.Put(((BYTE*)&this->MessageSeq)[0]);

	if (this->SubpackFlag)
	{
		this->RawHeadData.Put(((BYTE*)&this->SubpackCount)[1]);
		this->RawHeadData.Put(((BYTE*)&this->SubpackCount)[0]);
		this->RawHeadData.Put(((BYTE*)&this->SubpackSequence)[1]);
		this->RawHeadData.Put(((BYTE*)&this->SubpackSequence)[0]);
	}
}

bool JTT808Head::ParseHead()
{
	if (this->RawHeadData.Size() < 12)
		return false;
	BYTE * ptr = this->RawHeadData.Ptr();
	this->MessageID = (ptr[0] << 8) | ptr[1];
	WORD HeadAttribute = (ptr[2] << 8) | ptr[3];
	this->MessageLength = HeadAttribute & 0b11111111111;
	HeadAttribute >>= 10;
	this->EncryptType = (JTT808Head::En_EncryptType)(HeadAttribute & 0b111);
	HeadAttribute >>= 3;
	this->SubpackFlag = HeadAttribute & 0b1;
	BCD2String(ptr + 4, 6, (char*)this->PhoneNumber);
	this->MessageSeq = (ptr[10] << 8) | ptr[11];
	if (this->SubpackFlag)
	{
		if (this->RawHeadData.Size() < 16)
			return false;
		this->SubpackCount = (ptr[12] << 8) | ptr[13];
		this->SubpackSequence = (ptr[14] << 8) | ptr[15];
	}
	return true;
}
