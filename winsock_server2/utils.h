#pragma once

#include <memory>
#include <assert.h>
#include <WinSock2.h>
#include "BaseErrorCode.h"

inline int TransByteOrder(void *inoutData, const int length)
{
	assert(sizeof(int) == sizeof(u_long));
	if (NULL == inoutData)
	{
		return EXE_FAIL;
	}
	if (length < 0)
	{
		return EXE_FAIL;
	}
	if (length % 4 != 0)
	{
		assert(0);
		return EXE_FAIL;
	}
	std::unique_ptr<char[]> buffer(new char[length]);
	memcpy(buffer.get(), inoutData, length);
	int *pData = (int*)buffer.get();
	int dataLen = length >> 2;
	int *pInOutData = (int*)inoutData;
	for (int i = 0; i < dataLen; ++i)
	{
		pInOutData[i] = ntohl(pData[i]);
	}
	//
	return EXE_SUCCESS;
}