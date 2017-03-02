/////////////////////////////////////////////////
//
// Make LatenceProcessor object in here
//
/////////////////////////////////////////////////

#include "LatenceProcessor.h"
#include <winsock2.h>
#include <WS2tcpip.h>
#include <map>
#pragma comment (lib, "Ws2_32.lib")

typedef map<string, content*> __FIELDS;
typedef map<string, content*>::iterator __FIELDS_IT;
typedef map<string, string> __PARAMETERS;
typedef map<string, string>::iterator __PARAMETERS_IT;

class RequestImp : public Request
{
public:
	~RequestImp() override;
	const content* GetValueFromField(const string& r_Name)const override;
	const string& GetValueFromParameter(const string& r_Name)const override;
	RequestImp(SOCKET *s) :_socket(s) {};
protected:
	void PutFieldValue(string Name, content value);
	void PutParameterValue(string Name, const string value);

private:
	__FIELDS fields;
	__PARAMETERS parameters;
	SOCKET *_socket;
};

RequestImp::~RequestImp()
{
	fields.clear();
	parameters.clear();
}

const content* RequestImp::GetValueFromField(const string& r_Name) const
{
	try
	{
		return fields.at(r_Name);
	}
	catch (const std::out_of_range&)
	{
		return NULL;
	}
}

const string& RequestImp::GetValueFromParameter(const string& r_Name) const
{
	try
	{
		return parameters.at(r_Name);
	}
	catch (const std::out_of_range)
	{
		return NULL;

	}
}

void RequestImp::PutFieldValue(string Name, content Value)
{
	fields[Name] = &Value;
}

void RequestImp::PutParameterValue(string Name, string Value)
{
	parameters[Name] = Value;
}

class ResponseImp : public Response
{
public:
	ResponseImp(SOCKET *s) :_socket(s) {};
	~ResponseImp() override;
	bool OutText(const TextMedia& media, const string& message)const override;
	bool OutError(const int state, const string& message)const override;
private:
	SOCKET *_socket;
};

ResponseImp::~ResponseImp()
{

}

bool ResponseImp::OutText(const TextMedia& media, const string &message) const
{
	return TRUE;
}

bool ResponseImp::OutError(const int state, const string& message) const
{
	char *media = TextMediaIndex[0];
	return TRUE;
}

class LatenceProcessorImp :public LatenceProcessor
{
public:
	LatenceProcessorImp(Request *request, Response *response) :_pRequest(request), _pResponse(response) {}
	bool DoPost(Request &request, Response &response) override;
	~LatenceProcessorImp() override;
private:
	Request *_pRequest;
	Response *_pResponse;
};

bool LatenceProcessorImp::DoPost(Request &request, Response &response)
{
	return 0;
}

LatenceProcessorImp::~LatenceProcessorImp()
{

}

LatenceProcessor* MakeLatenceProcessor(SOCKET *socket)
{
	return new LatenceProcessorImp{new RequestImp(socket), new ResponseImp(socket)};
}
