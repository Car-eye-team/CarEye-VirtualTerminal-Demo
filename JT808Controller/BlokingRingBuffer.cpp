#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <algorithm>
#include "BlockingRingBuffer.h"

unsigned int ci = 0;

BlockingRingBuffer::Block::Block()
{
	this->content = new BYTE[this->BlockSize];
}

BlockingRingBuffer::Block::~Block()
{
	delete this->content;
}


BlockingRingBuffer::BlockingRingBuffer()
{
	this->pReadBlock = this->pWriteBlock = new Block();
	this->pReadPtr = this->pReadBlock->content;
	this->pWritePtr = this->pWriteBlock->content;
	this->pWriteBlock->nextBlock = this->pReadBlock;
}

BlockingRingBuffer::~BlockingRingBuffer()
{
	BlockingRingBuffer::Block *ptr = this->pReadBlock;
	do
	{
		BlockingRingBuffer::Block *del = ptr;
		ptr = ptr->nextBlock;
		delete del;
	} while (ptr != this->pReadBlock);
}

unsigned long BlockingRingBuffer::ReadableRemain()
{
	if (this->pReadBlock == this->pWriteBlock)
	{
		return (unsigned long)(this->pWritePtr - this->pReadPtr);
	}
	else
	{
		return (unsigned long)(this->pReadBlock->content + BlockingRingBuffer::Block::BlockSize - this->pReadPtr);
	}
}

unsigned long BlockingRingBuffer::WritableRemain()
{
	return (unsigned long)(this->pWriteBlock->content + BlockingRingBuffer::Block::BlockSize - this->pWritePtr);
}

void BlockingRingBuffer::Cat(const BYTE* pData, unsigned long uLength)
{
	unsigned long __uLength = uLength;
	// this->aic.Put(pData, uLength);
	while (uLength > 0)
	{
		const unsigned long uWriteLen = std::min(this->WritableRemain(), uLength);
		assert(uWriteLen != 0);
		memcpy(this->pWritePtr, pData, uWriteLen);
		uLength -= uWriteLen;
		pData += uWriteLen;
		this->pWritePtr += uWriteLen;
		if (this->pWritePtr == (this->pWriteBlock->content + BlockingRingBuffer::Block::BlockSize))
		{
			// 如果尾块下一个是头块则插入新块否则直接使用下一个块
			if (this->pWriteBlock->nextBlock == this->pReadBlock)
			{
				Block *insertBlock = new Block();
				insertBlock->nextBlock = this->pWriteBlock->nextBlock;
				this->pWriteBlock->nextBlock = insertBlock;
			}
			this->pWriteBlock = this->pWriteBlock->nextBlock;
			this->pWritePtr = this->pWriteBlock->content;
		}
	}
	assert(uLength == 0);
	this->IncreaseLevel(__uLength);
}

En_BlockingRingBuffer_FetchResult BlockingRingBuffer::Fetch(BYTE* pData, unsigned long uLength)
{
	if (this->Working)
	{
		this->Request(uLength);
	}
	if (!this->Working) return En_BlockingRingBuffer_FetchResult::RBFR_STOP;
	BYTE *_pData = pData;
	unsigned long _uLength = uLength;
	while (uLength > 0)
	{
		if (this->pReadPtr == this->pWritePtr)
		{
			assert(false);
			return En_BlockingRingBuffer_FetchResult::RBFR_TOOLONG;
		}
		const unsigned long uReadLen = std::min(this->ReadableRemain(), uLength);
		memcpy(pData, this->pReadPtr, uReadLen);
		uLength -= uReadLen;
		pData += uReadLen;
		this->pReadPtr += uReadLen;
		if (this->pReadPtr == (this->pReadBlock->content + BlockingRingBuffer::Block::BlockSize))
		{
			this->pReadBlock = this->pReadBlock->nextBlock;
			this->pReadPtr = this->pReadBlock->content;
		}
	}
	assert(uLength == 0);
	// assert(memcmp(pData, this->aic.Ptr() + this->checked, uLength) == 0);
	// this->checked += uLength;
	// FILE *fp = fopen("recv.dat", "ab+");
	// fseek(fp, 0, SEEK_END);
	// fwrite(_pData, _uLength, 1, fp);
	// fclose(fp);
	return En_BlockingRingBuffer_FetchResult::RBFR_OK;
}

void BlockingRingBuffer::Stop()
{
	this->Working = false;
	this->Semaphore::Stop();
}
