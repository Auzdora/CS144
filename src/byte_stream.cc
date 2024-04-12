#include "byte_stream.hh"
#include <iostream>
#include <mutex>
#include <string_view>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), stream_() {}

bool Writer::is_closed() const
{
  // Your code here.
  return closed_;
}

void Writer::push( string data )
{
  // Your code here.
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
  // Your code here.
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - stream_.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return total_bytes_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return closed_ && stream_.size() == 0;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return total_bytes_poped_;
}

string_view Reader::peek() const
{
  // Your code here.
  return string_view(stream_);
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  stream_.erase(0, len);
  total_bytes_poped_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return stream_.size();
}
