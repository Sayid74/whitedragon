#include <Parameter.h>
#include <exception>
#include <string>


using namespace SayidYoung;

Parameters::Cursor::Cursor(const Parameters *container, int index) :_container(container), _index(index)
{

}

Parameters::Cursor::Cursor(const Parameters::Cursor &other)
{
	this->_container = other._container;
	this->_index = other._index;
}


Parameters::Cursor::Cursor(Parameters::Cursor &&other)
{
	this->_container = other._container;
	this->_index = other._index;
}

Parameters::Cursor::~Cursor()
{
}


Parameters::Cursor& Parameters::Cursor::operator++(int)
{
	size_t len = _container->length;
	_index < len ? _index++ : len;
	return *this;
}


Parameters::Cursor& Parameters::Cursor::operator--(int)
{
	_index == 0 ? 0 : _index--;
	return *this;
}

Parameters::Cursor& Parameters::Cursor::operator=(const Parameters::Cursor &other)
{

	if (&_container != &(other._container))
		throw ParameterAssignFailed("Difference container!");

	_index = other._index;
	return *this;
}

Parameters::Cursor& Parameters::Cursor::operator=(Parameters::Cursor &&other)
{
	if (&_container != &(other._container))
		throw ParameterAssignFailed("Difference container!");
	
	_index = other._index;
	return *this;
}

bool Parameters::Cursor::operator==(Parameters::Cursor &other)
{
	return _index == other._index;
}

bool Parameters::Cursor::operator!=(Parameters::Cursor &other)
{
	return _index != other._index;
}

const Content* Parameters::Cursor::getValue() const
{
	size_t len = _container->length;
	return _index < len ? &(_container->_values[_index]) : NULL;
}

const string* Parameters::Cursor::getName() const
{
	size_t len = _container->length;
	return _index < len ? &(_container->_names[_index]) : NULL ;
}


Parameters::Parameters(size_t length, string *names, Content *values) :length(length)
{
	_values = new Content[length]{};
	_names = new string[length];
	for (size_t i = 0; i < length; i++)
	{
		_names[i] = names[i];
		_values[i] = values[i];
		_nameIndex[*(names + i)] = i;
	}
	memcpy(_names, names, length * sizeof(string));
	memcpy(_values, values, length * sizeof(Content));
}

Parameters::Cursor& Parameters::begin()
{
	return *(new Cursor(this, 0));
}

Parameters::Cursor& Parameters::end()
{
	return *(new Cursor(this, length));
}

Parameters::Cursor& Parameters::getByName(string name)
{
	if (_nameIndex.find(name) == _nameIndex.end())
		return end();
	return *(new Cursor(this, _nameIndex[name]));
}

const string* Parameters::getNames() const
{
	return _names;
};

bool Parameters::haseName(string name)
{
	return _nameIndex.find(name) != _nameIndex.end();
}

void main(int argc, char** argv)
{
	Content *contents = new Content[5]
	{ {4, "abc\0"}
	, {4, "bcd\0"}
	, {4, "efg\0"}
	, {4, "hij\0"}
	, {4, "klm\0"}
	};

	string *names = new string[5]{
		"one", "two", "three", "four", "five"
	};
	Parameters *ps = new Parameters(5, names, contents);

	for (Parameters::Cursor c = ps->begin(); c != ps->end(); c++)
	{
		printf("name:%s, value:%s \n", c.getName()->c_str(), c.getValue()->getData());
	}

	delete ps;
}