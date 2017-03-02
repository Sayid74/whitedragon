#pragma once
#include <vector>
class content
{
public:
	content(size_t size):_size(size), _data(new char[size]) {}        ;
	content(std::vector<char> v) :_size(v.size()), _data(v.data()) {}     ;
	~content() { delete _data; }                                      ;

	char * GetData() const { return _data; } ;
	size_t GetSize() const { return _size; } ;

	std::vector<char> ToVector() const
	{
		return { _data, _data + _size };
	};

private:
	size_t _size;
	char *_data;
};
