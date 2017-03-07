#pragma once
#include "LatenceProcessor.h"

class DefaultLatenceProcessor : public LatenceProcessor
{
public:
	virtual BOOL DoPost(Request &request, Response &response);
	virtual ~LatenceProcessor() {};
};
