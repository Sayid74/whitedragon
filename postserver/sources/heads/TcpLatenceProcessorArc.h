#pragma once
#include <WinSock2.h>
#include <map>

#include "LatenceProcessor.h"
#include "string"

using namespace std;

class Parameters
{
public:
	class Cursor
	{
	public:
		~Cursor();
		string getKey() = 0;
		string getValue() = 0;
		Cursor& operator++() = 0;
		Cursor& operator--() = 0;
	};


	Parameters();
	BOOL putParameter(string name, string value);
	Cursor* current();
	Cursor* begin();
	Cursor* getByName(string name);
	BOOL isEmpty();
private:
	typedef map<string, string> NameIndex;
	typedef NameIndex::iterator 

	NameIndex _names;
};

class TcpRequest :public Request
{
public:
	TcpRequest(SOCKET s) : Request(), _socket(s) {};
private:
	SOCKET _socket;
};

class TcpRespons : public Response
{
public:
	TcpRespons(SOCKET s) : Response(), _socket(s) {};
private:
	SOCKET _socket;
};

class TcpLatenceProcessorArc : public LatenceProcessor
{
public:
	virtual BOOL DoPost(Request *request, Response *response) = 0;
};

class TcpLatenceProcessorCallerArc : public LatenceProcessorCaller
{
public:
	TcpLatenceProcessorCallerArc(SOCKET *s);
private:
	SOCKET   *_socket;
	Request  *_request;
	Response *_response;
};
