#pragma once

#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <ostream>
#include <istream>

class str { 
	size_t sz = 0;
	char* ptr = nullptr; 

	static const size_t short_max = 15;

	union 
	{
		size_t space; // additional space
		char ch[short_max + 1];
	};

	inline void check_range(size_t pos) const
	{
		if(pos >= sz)
		{
			throw std::out_of_range("str::at() index out of bounds");
		}
	}
	
	char* expand(const char* ptr, size_t n)
	{
		char* p = new char[n];
		strcpy(p, ptr);
		return p;
	}

	void copy_from(const str& other)
	{
		if(other.sz <= short_max)
		{
			memcpy(this, &other, sizeof(other));
			ptr = ch; // after copy ptr must not have address of other.ch, but address of this->ch
		}
		else
		{
			ptr = expand(other.ptr, other.sz + 1);
			sz = other.sz;
			space = 0; // to copy we only expand to other.sz+1 so there is no additional space right now
		}
	}
	
	void move_from(str& other) noexcept
	{
		if(other.sz <= short_max)
		{
			memcpy(this, &other, sizeof(other));
			ptr = ch;
		}
		else 
		{
			ptr = other.ptr;
			sz = other.sz;
			space = other.space;

			other.ptr = other.ch; // now other is short string
			other.sz = 0;
			other.ch[0] = 0;
		}
	}


public:

	str() noexcept 
		: sz{0}
		, ptr{ch} 
	{
		ch[0] = 0;
	}

	str(const char* cstr) 
		: sz{ strlen(cstr) }
		, ptr{ (sz <= short_max) ? ch : new char[sz + 1] }
		, space{ 0 }
	{
		strcpy(ptr, cstr);
	}

	str(const str& other)
	{
		copy_from(other);
	}

	str(str&& other) noexcept
	{
		move_from(other);
	}

	str& operator=(const str& other) 
	{
		if(this == &other)
		{
			return *this;
		}
		char* p = (short_max < sz) ? ptr : nullptr;
		copy_from(other);
		delete[] p;
		return *this;
	}

	str& operator=(str&& other) noexcept
	{
		if(this == &other)
		{
			return *this;
		}
		if(short_max < sz)
		{
			delete[] ptr;
		}
		move_from(other);
		return *this;
	}

	const char& operator[](size_t pos) const noexcept
	{
		return ptr[pos];
	}

	char& operator[](size_t pos) noexcept
	{
		return ptr[pos];
	}

	const char& at(size_t pos) const
	{
		check_range(pos);
		return ptr[pos];
	}

	char& at(size_t pos)
	{
		check_range(pos);
		return ptr[pos];
	}

	str& operator+=(char c)
	{
		if(sz == short_max)
		{
			int n = sz + sz + 2; 
			ptr = expand(ptr, n);
			space = n - sz - 2; // 15 chars, 1 new char, 1 null
		}
		else if(short_max < sz)
		{
			if(space == 0) // no additional space available
			{
				int n = sz + sz + 2;
				char* p = expand(ptr, n);
				delete[] ptr;
				ptr = p;
				space = n - sz - 2;
			}
			else
			{
				--space;
			}
		}
		ptr[sz] = c;
		ptr[++sz] = 0;

		return *this;
	}

	const char* c_str() const noexcept
	{
		return ptr;
	}

	/*
	// i dont understand why it's needed as const member functions can be called by non-const objects
	const char* c_str() 
	{
		return ptr;
	}
	*/

	size_t size() const noexcept
	{
		return sz;
	}

	size_t capacity() const noexcept
	{
		return (sz <= short_max) ? short_max : sz + space;
	}

	bool operator==(const str& other) const noexcept
	{
		if(sz != other.sz)
		{ 
			return false;
		}

		for(int i = 0; i < sz; ++i)
		{
			if(ptr[i] != other.ptr[i])
			{
				return false;
			}
		}

		return true;
	}

	bool operator!=(const str& other) const noexcept
	{
		return !(*this == other);
	}

	~str()
	{
		if(short_max < sz) 
		{
			delete[] ptr;
		}
	}
};


char* begin(str& s) noexcept
{
	return const_cast<char*> (s.c_str());
}

char* end(str& s) noexcept
{
	return const_cast<char*> (s.c_str()) + s.size();
}

const char* begin(const str& s) noexcept
{
	return s.c_str();
}

const char* end(const str& s) noexcept
{
	return s.c_str() + s.size();
}

std::ostream& operator<<(std::ostream& out, const str& str)
{
	out << str.c_str();
	return out;
}

std::istream& operator>>(std::istream& in, str& str)
{
	str = "";
	in >> std::ws;
	char ch;
	while(in.get(ch) && !isspace(ch))
	{
		str += ch;
	}

	return in;
}

str& operator+=(str& a, const str& b)
{
	for(auto ch : b)
	{
		a += ch;
	}
	return a;
}

str operator+(const str& a, const str& b)
{
	str res{a};
	res += b;
	return res;
}

str operator""_s (const char* ptr, size_t)
{
	return str{ptr};
}


