#ifndef __STREAM_H__
#define __STREAM_H__

#include <vector>
#include <cstring>
#include <string>
#include "types.h"

using namespace type;
class Stream
{
public:
	Stream();
	Stream( byte_t* data, ushort_t size);

	template<class T>
	void Buffer(const uint_t spos, const T& data)
	{
		std::memcpy(buffer.data() + spos, &data, sizeof(data));
	}

	void    Buffer(byte_t* data, ushort_t size);
	byte_t*   Buffer();
	ushort_t  Size() const;

	Stream& operator << ( const uint_t data );
	Stream& operator << ( const unsigned long data );
	Stream& operator >> ( unsigned long& data );
	Stream& operator << ( const int_t data);
	Stream& operator << ( const byte_t data);
	Stream& operator << ( const ushort_t data);
	Stream& operator << ( const short data);
	Stream& operator >> ( short& data);
	Stream& operator << ( const std::string data);
	Stream& operator << ( Stream& s);
	Stream& operator << ( type::ustring& data);
	Stream& operator >> ( uint_t& data );
	Stream& operator >> ( int_t& data );
	Stream& operator >> ( byte_t& data );
	Stream& operator >> ( ushort_t& data);
	Stream& operator >> ( std::string& data );
	Stream& operator >> ( type::dnstring& data);
	Stream& operator >> ( type::ustring& data);

	Stream& operator=(const Stream& s);
	Stream& operator=(const Stream&& s);

private:
	template<class T>
	Stream& write(const T& data)
	{
		std::memcpy(buffer.data()+writeOffset, &data, sizeof(data));
		writeOffset += sizeof(data);
		return *this;
	}
	template<class T>
	Stream& read(T& data)
	{
		std::memcpy(&data, buffer.data()+readOffset, sizeof(data));
		readOffset += sizeof(data);
		writeOffset -= sizeof(data);
		return *this;
	}

	ushort_t writeOffset;
	ushort_t readOffset;
	const ushort_t size;
	std::vector<byte_t> buffer;
};

#endif
