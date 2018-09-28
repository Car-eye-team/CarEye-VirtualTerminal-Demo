#pragma once

namespace ContentType
{
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned long DWORD;
	typedef unsigned long long QWORD;
	typedef unsigned long long size_t;
};

class AutoIncressContent
{
protected:
	const unsigned int uBlockSize = 1024 * 4;
	ContentType::QWORD uContentLength;
	ContentType::BYTE *lpbyContent, *lpbyContentPtr;
	void IncressCheck(ContentType::size_t uIncressSize);
	template<typename T> void PutMeta(const T & Data)
	{
		unsigned int l = sizeof(T);
		this->IncressCheck(l);
		this->Put((const ContentType::BYTE *)&Data, l);
	}
	void Copy(const AutoIncressContent & rv);
public:
	AutoIncressContent();
	AutoIncressContent(AutoIncressContent & rv);
	~AutoIncressContent();
	AutoIncressContent & operator=(const AutoIncressContent & rv);
	virtual void Put(const ContentType::BYTE * pData, ContentType::size_t uLength);
	virtual void Put(const ContentType::BYTE byData);
	virtual void Put(const ContentType::WORD wData);
	virtual void Put(const ContentType::DWORD dData);
	virtual void Put(const ContentType::QWORD qData);
	void Reset();
	ContentType::BYTE * Dump();
	ContentType::BYTE * Ptr() const;
	unsigned int Size() const;
};

