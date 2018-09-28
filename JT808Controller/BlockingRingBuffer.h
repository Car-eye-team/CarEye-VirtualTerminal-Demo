#pragma once

#include "AutoIncressContent.h"
#include "Semaphore.h"

typedef unsigned char BYTE;
enum BlockingRingBufferFetchResult { RBFR_OK = 0, RBFR_STOP = 1, RBFR_TOOLONG = 2 };
typedef enum BlockingRingBufferFetchResult En_BlockingRingBuffer_FetchResult;

class BlockingRingBuffer : private Semaphore
{
private:
	// AutoIncressContent aic;
	// unsigned long checked = 0;
	unsigned long ReadableRemain();
	unsigned long WritableRemain();
	bool Working = true;
public:
	class Block
	{
	public:
		const static unsigned long BlockSize = 4 * 1024;
		Block *nextBlock = nullptr;
		BYTE *content = nullptr;
		Block();
		~Block();
	};
	Block *pReadBlock, *pWriteBlock;
	BYTE *pReadPtr, *pWritePtr;
	BlockingRingBuffer();
	~BlockingRingBuffer();
	void Cat(const BYTE *pData, unsigned long uLength);
	En_BlockingRingBuffer_FetchResult Fetch(BYTE *pData, unsigned long uLength);
	void Stop();
};
