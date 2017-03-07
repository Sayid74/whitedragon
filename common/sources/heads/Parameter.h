#pragma once
#include "content.h"
#include "Exceptions.h"

#include <map>
#include <string>

#define PARAMETER_ERRCODE 90877;

using namespace std;

namespace SayidYoung{
	class ParameterOverflow : public DefaultException
	{
	private:
		const static ErrNum ExpID = START_ERR_NUM + 84680;
	public:
		ParameterOverflow(string message) : DefaultException(message, ExpID) {};
	};

	class ParameterAssignFailed : public DefaultException
	{
	private:
		const static ErrNum ExpID = START_ERR_NUM + 95817;
	public:
		ParameterAssignFailed(string message) :
			DefaultException(message, ExpID) {};
	};

	class Parameter
	{
	public:
		virtual ~Parameter() {};
		virtual const string* getName()const = 0;
		virtual const Content* getValue()const = 0;
	};

	class Parameters
	{
	public:
		typedef map<string, size_t> NameIndex;
		typedef NameIndex::iterator IndexIterator;

		class Cursor : Parameter
		{
		private:
			Cursor(const Parameters *container, int index);
			friend Parameters;
		public:
			~Cursor();
			Cursor(const Cursor& other);
			Cursor(Cursor &&other);

			const string* getName() const override;
			const Content* getValue() const override;

			Cursor& operator++(int);
			Cursor& operator--(int);
			Cursor& operator = (const Cursor &other);
			Cursor& operator = (Cursor &&other);
			bool operator == (Cursor &other);
			bool operator != (Cursor &other);

		private:
			volatile size_t _index;
			const Parameters *_container;
		};


		Parameters(size_t length, string *names, Content *values);

		Cursor& begin();
		Cursor& end();
		Cursor& getByName(string name);
		const string* getNames() const;
		bool haseName(string name);
		const size_t length;
	private:
		Parameters(const Parameters &parameters) :length(0) {};
		Parameters(Parameters &&parameters) :length(0) {};
		Parameters &operator = (const Parameters &parameters) {};
		Parameters &operator = (Parameters &&parameters) {};

		Content *_values;
		string *_names;
		NameIndex _nameIndex;
	};
}