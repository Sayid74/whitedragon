#pragma once

#include <WinSock2.h>
#include <string>

class PostRoutine;

class PostServer
{
public:
	PostServer() {};
	virtual ~PostServer() {};
	virtual int start() = 0;
	virtual int stop() = 0;
	virtual void AddRoutine(std::string name, PostRoutine &routine) = 0;
	virtual void DelRoutine(std::string name) = 0;
protected:
};

class PostRoutine
{
public:
	virtual void DoProcessing(SOCKET *s) = 0;
};
