#include <WinSock2.h>
#include <list>
#include <map>
#include <string>
#include <iostream>

#include "LatenceProcessor.h"
#include "PostServer.h"

using namespace std;

class RoutineList
{
public:
	RoutineList() {};
	~RoutineList() {};
	PostRoutine* add(string name, PostRoutine *routine)
	{
		if (routine == NULL) return NULL;

		if (_NameIndex.find(name) == _NameIndex.end())
		{
			_Routines.push_back(routine);
			_NameIndex[name] = (_Routines.end()--);
			return routine;
		}
		else
		{
			return NULL;
		}
	}
	void remote(string name)
	{
		_ROUTINENAMES::iterator name_it = _NameIndex.find(name);
		if (name_it != _NameIndex.end())
		{
			_ROUTINE_IT routine_it = _NameIndex.find(name)->second;
			_Routines.erase(routine_it);
		}
	}

	void DoRoutine(SOCKET *s) {
		if (_Routines.empty()) return;
		for (_ROUTINE_IT it = _Routines.begin(); it != _Routines.end(); it++)
		{
			PostRoutine *routine = (*it);
			routine->DoProcessing(s);
		}
	}
private:
	typedef list<PostRoutine*> _ROUTINES;
	typedef _ROUTINES::iterator _ROUTINE_IT;
	typedef map<string, _ROUTINE_IT> _ROUTINENAMES;

	_ROUTINENAMES _NameIndex;
	_ROUTINES _Routines;
};

class RoutineImp: public PostRoutine 
{
public:
	RoutineImp() {};
	~RoutineImp() {};
	virtual void DoProcessing(SOCKET *s) {
		LatenceProcessor *p = MakeLatenceProcessor(s);
		p->DoPost();
	};
private:
};

class PostServerImp: public PostServer
{
public:
	PostServerImp();
	~PostServerImp();

	virtual int start() {};
	virtual int stop() {};
	virtual void AddRoutine(std::string name, PostRoutine &routine) {};
	virtual void DelRoutine(std::string name) {};
private:

};

PostServerImp::PostServerImp()
{
}

PostServerImp::~PostServerImp()
{
}

int main(int argc, char** argv)
{
	class _PostRoutine_1:public PostRoutine
	{
	public:
		_PostRoutine_1() :PostRoutine() {};
		~_PostRoutine_1() {};

		
		void DoProcessing(SOCKET *s) override
		{
			cout << "1	hello world!" << endl;
		}
	};

	class _PostRoutine_2:public PostRoutine
	{
	public:
		_PostRoutine_2() :PostRoutine() {};
		~_PostRoutine_2() {};

		
		void DoProcessing(SOCKET *s) override
		{
			cout << "2	hello world!" << endl;
		}
	};

	RoutineList routines;
	_PostRoutine_1 routine_1;
	_PostRoutine_2 routine_2;
	routines.add("routine1", &routine_1);
	routines.add("routine2", &routine_2);
	routines.DoRoutine(NULL);
}

