#pragma once
#include <string>

using namespace std;
typedef unsigned int ErrNum;

const static ErrNum START_ERR_NUM = 271878;

class CommonException
{
public:
	virtual string getMessage()const = 0;
	virtual ErrNum getExpNo()const = 0;
};

class DefaultException : public CommonException
{
public:
	DefaultException(string message = "NULL", ErrNum expno = START_ERR_NUM):_message(message), _expNo(expno) {};
	string getMessage()const override
	{
		return _message;
	};
	ErrNum getExpNo()const override
	{
		return _expNo;
	};
private:
	string _message;
	ErrNum _expNo;
};