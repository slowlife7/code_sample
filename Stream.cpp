#ifdef WIN32
#include <winsock2.h>
#else
#include <iostream>
#include <arpa/inet.h>
#endif
#include "Stream.h"

Stream::Stream()
      : size(4096),
      buffer(size),
      writeOffset(0),
      readOffset(0)
{

}

Stream::Stream( byte_t* data, ushort_t rsize)
  : size(rsize),
  buffer(size),
  readOffset(0),
  writeOffset(0)
{
  std::memcpy(buffer.data(), data, size);
  writeOffset += size;
}

void Stream::Buffer(byte_t* data, ushort_t rsize)
{
  std::memcpy(buffer.data()+writeOffset, data, rsize);
  writeOffset += rsize;
}

byte_t* Stream::Buffer()
{
  return buffer.data();
}

ushort_t Stream::Size() const
{
  return writeOffset;
}

Stream& Stream::operator=(const Stream& s)
{
	this->buffer = s.buffer;
	this->writeOffset = s.writeOffset;
	this->readOffset = s.readOffset;
	return *this;
}

Stream& Stream::operator=(const Stream&& s)
{
	this->buffer = std::move(s.buffer);
	this->writeOffset = std::move(s.writeOffset);
	this->readOffset = std::move(s.readOffset);
	return *this;
}

Stream& Stream::operator << (const int data)
{
  int source = htonl(data);
  return write(source);
}
Stream& Stream::operator >> ( int& data)
{
  int dst = 0;
  read(dst);
  data=ntohl(dst);
  return *this;
}

Stream& Stream::operator << ( const uint_t data)
{
	 int source = htonl(data);
  return write(source);
}

Stream& Stream::operator << ( const unsigned long data)
{
	 int source = htonl(data);
  return write(source);
}


Stream& Stream::operator >> ( uint_t& data )
{
	uint_t dst = 0;
	read(dst);
	data = ntohl(dst);
	return *this;
}

Stream& Stream::operator >> ( unsigned long& data )
{
	uint_t dst = 0;
	read(dst);
	data = ntohl(dst);
	return *this;
}

Stream& Stream::operator >> (byte_t& data)
{
  return read(data);
}

Stream& Stream::operator << ( const std::string data)
{
  byte_t length= data.size();
  write(length);
  std::memcpy(buffer.data()+writeOffset, data.c_str(), data.size());
  writeOffset += data.size();
  return *this;
}

Stream& Stream::operator << (  Stream& s)
{
  Buffer(s.Buffer(), s.Size());
  return *this;
}
Stream& Stream::operator >> ( std::string& data)
{
  byte_t length = 0;
  read(length);
  void* buff = buffer.data();
  data=std::string(static_cast<char*>(buff)+readOffset, length);
  readOffset+=length;
  return *this;
}

Stream& Stream::operator << (const byte_t data)
{
  return write(data);
}

Stream& Stream::operator << (const ushort_t data)
{
  ushort_t source = htons(data);
  return write(source);
}
Stream& Stream::operator << (const short data)
{
  ushort_t source = htons(data);
  return write(source);
}

Stream& Stream::operator >> ( ushort_t& data)
{
  ushort_t dst = 0;
  read(dst);
  data=ntohs(dst);
  return *this;
}

Stream& Stream::operator >> ( short& data)
{
  ushort_t dst = 0;
  read(dst);
  data=ntohs(dst);
  return *this;
}

Stream& Stream::operator >> ( type::dnstring& data)
{
	byte_t length = 0;
	read(length);

	ushort_t dn_start = readOffset;
	byte_t dn_length = length;
	if(length > 0 ) 
	{
		dn_length -=1;
std::memcpy(&data.type, buffer.data()+readOffset, sizeof(data.type));
		dn_start += 1;
	}

  void* buff = buffer.data();
	data.dn=std::string(static_cast<char*>(buff)+dn_start, dn_length);
	readOffset+=length;
	return *this;
}

Stream& Stream::operator >> ( type::ustring& data)
{
  ushort_t length = 0;
  read(length);
  data.length = ntohs(length);
  void* buff = buffer.data();

  data.data = std::string(static_cast<char*>(buff)+readOffset, data.length);
  readOffset += data.length;
  return *this;
}

Stream& Stream::operator <<( type::ustring& data)
{
  write(htons(data.length));
  std::memcpy(buffer.data() + writeOffset, data.data.c_str(), data.length);
  writeOffset += data.length;
  return *this;
}
