#include "byte_stream.hh"
#include <iostream>
#include <mutex>
#include <string_view>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), stream_() {}

bool Writer::is_closed() const
{
  return closed_;
}

void Writer::push( string data )
{
  if (is_closed()) {
    return;
  }

  uint64_t tmp = available_capacity();
  if (tmp < data.size()) {
    stream_.append(data.substr(0, tmp));
    total_bytes_pushed_ += tmp;
  } else {
    stream_.append(data);
    total_bytes_pushed_ += data.size();
  }
  
  return;
}

void Writer::close()
{
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - stream_.size();
}

uint64_t Writer::bytes_pushed() const
{
  return total_bytes_pushed_;
}

bool Reader::is_finished() const
{
  return closed_ && stream_.size() == 0;
}

uint64_t Reader::bytes_popped() const
{
  return total_bytes_poped_;
}

string_view Reader::peek() const
{
  return string_view(stream_);
}

void Reader::pop( uint64_t len )
{
  stream_.erase(0, len);
  total_bytes_poped_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  return stream_.size();
}
