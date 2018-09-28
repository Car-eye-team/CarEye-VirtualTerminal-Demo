#include "stdafx.h"
#include <cstdlib>
#include <cstring>
#include "AutoIncressContent.h"


AutoIncressContent::AutoIncressContent()
{
	this->uContentLength = this->uBlockSize;
	this->lpbyContent = this->lpbyContentPtr = (ContentType::BYTE*)malloc(this->uBlockSize);
}

AutoIncressContent::AutoIncressContent(AutoIncressContent& rv)
{
	this->Copy(rv);
}

AutoIncressContent::~AutoIncressContent()
{
	free(this->lpbyContent);
}

void AutoIncressContent::Copy(const AutoIncressContent& rv)
{
	this->Reset();
	this->IncressCheck(rv.Size());
	std::memcpy(this->lpbyContent, rv.Ptr(), rv.Size());
	this->lpbyContentPtr = this->lpbyContent + rv.Size();
}

AutoIncressContent& AutoIncressContent::operator=(const AutoIncressContent& rv)
{
	this->Copy(rv);
	return *this;
}

void AutoIncressContent::IncressCheck(ContentType::size_t uIncressSize)
{
	ContentType::QWORD uChacheUsed = this->lpbyContentPtr - this->lpbyContent;
	ContentType::QWORD uRequestLength = uChacheUsed + uIncressSize - this->uContentLength;
	if (uRequestLength > 0)
	{
		unsigned long long uTargetLength = uChacheUsed + uIncressSize;
		uTargetLength = (1 + uTargetLength / this->uBlockSize) * this->uBlockSize;
		this->lpbyContent = (ContentType::BYTE*)realloc(this->lpbyContent, uTargetLength);
		this->lpbyContentPtr = this->lpbyContent + uChacheUsed;
		this->uContentLength = uTargetLength;
	}
}

void AutoIncressContent::Reset()
{
	this->lpbyContentPtr = this->lpbyContent;
}

void AutoIncressContent::Put(const ContentType::BYTE * pData, ContentType::size_t uLength)
{
	this->IncressCheck(uLength);
	memcpy(this->lpbyContentPtr, pData, uLength);
	this->lpbyContentPtr += uLength;
}

void AutoIncressContent::Put(const ContentType::BYTE byData)
{
	this->IncressCheck(1);
	*this->lpbyContentPtr++ = byData;
}

void AutoIncressContent::Put(const ContentType::WORD wData)
{
	this->PutMeta(wData);
}

void AutoIncressContent::Put(const ContentType::DWORD dData)
{
	this->PutMeta(dData);
}

void AutoIncressContent::Put(const ContentType::QWORD qData)
{
	this->PutMeta(qData);
}

ContentType::BYTE * AutoIncressContent::Dump()
{
	ContentType::QWORD uLength = this->lpbyContentPtr - this->lpbyContent;
	ContentType::BYTE * lpbyTemp = (ContentType::BYTE*)malloc(uLength);
	memcpy(lpbyTemp, this->lpbyContent, uLength);
	return lpbyTemp;
}

ContentType::BYTE * AutoIncressContent::Ptr() const
{
	return this->lpbyContent;
}

unsigned int AutoIncressContent::Size() const
{
	return (int)(this->lpbyContentPtr - this->lpbyContent);
}