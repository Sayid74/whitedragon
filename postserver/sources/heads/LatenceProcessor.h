#pragma once

#include <memory>
#include <string>
#include <WinSock2.h>
#include "content.h"

using namespace std;


enum class TextMedia: int
{
	PLAIN = 0, JSON = 1, HTML = 2
};

char *TextMediaIndex[] = {
	"text/plain", "text/json", "text/html"
};


class Request {
public:
	virtual ~Request() {};
	virtual const content* GetValueFromField(const string& r_Name)const = 0 ;
	virtual const string& GetValueFromParameter(const string& r_Name)const = 0 ;
};

class Response
{
public:
	virtual ~Response() {};
	virtual bool OutText(const TextMedia& media, const string& message)const = 0;
	virtual bool OutError(const int state, const string& message)const = 0;
};

class LatenceProcessor
{
public:
	virtual bool DoPost(Request &request, Response &response)= 0;
	virtual ~LatenceProcessor() {};
};

LatenceProcessor* MakeLatenceProcessor(SOCKET *socket);