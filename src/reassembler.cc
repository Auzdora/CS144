#include "reassembler.hh"
#include <iostream>

using namespace std;

void Reassembler::merge_and_insert(uint64_t first_index, std::string data) {
  // insert first
  auto entry = unassemble_map_.find(first_index);
  if (entry == unassemble_map_.end()) {
    unassemble_map_[first_index] = data;
  } else {
    if (data.length() > entry->second.length()) {
      unassemble_map_[first_index] = data;  
    }
  }

  auto it = unassemble_map_.begin();
  while (it != unassemble_map_.end()) {
    auto next_it = std::next(it);
    if (next_it == unassemble_map_.end()) break;

    uint64_t data_len = it->second.length();
    uint64_t idx = it->first;

    if (data_len + idx == next_it->first) {
      unassemble_map_[idx].append(next_it->second);
      unassemble_map_.erase(next_it);

    } else if (data_len + idx >= next_it->first + next_it->second.length()) {
      unassemble_map_.erase(next_it);

    } else if (data_len + idx > next_it->first) {
      uint64_t overlap_len = data_len + idx - next_it->first;
      std::string surpass_str = next_it->second.substr(overlap_len);
      unassemble_map_[idx].append(surpass_str);
      unassemble_map_.erase(next_it);

    } else {
      ++it;
    }
  }
}

void Reassembler::push(uint64_t first_index, string data) {
  // check if its beyond capacity
  uint64_t cap = output_.writer().available_capacity();
  if (cap == 0 || first_index >= current_index_ + cap) {
    return;
  }

  if ((first_index + data.length()) > (current_index_ + cap)) {
    uint64_t end_index = current_index_ + cap;
    data = data.substr(0, end_index - first_index);
  }

  if (current_index_ < first_index) {
    merge_and_insert(first_index, data);
    return;
  }

  if (current_index_ == first_index) {
    output_.writer().push(data);
    current_index_ += data.length();
    return;
  }

  // current_index > first_index
  uint64_t overlap_len = current_index_ - first_index;
  uint64_t byte_len = data.length();

  if (byte_len <= overlap_len) {
    // discard this data
    return;
  }

  // byte_len > overlap_len
  std::string rest = data.substr(overlap_len);
  output_.writer().push(rest);
  current_index_+= rest.length();
  return;
}

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  if (is_last_substring) {
    ready_to_close_ = true;
    full_data_len_ = first_index + data.length();
  }
  // Your code here.
  push(first_index, data);

  // check unassemble map and push data if its qualified immediately
  uint64_t min_index = unassemble_map_.begin()->first;
  while(unassemble_map_.size() != 0 && current_index_ >= min_index) {
    push(min_index, unassemble_map_[min_index]);
    unassemble_map_.erase(min_index);
    min_index = unassemble_map_.begin()->first;
  }

  if (ready_to_close_ && output_.writer().bytes_pushed() == full_data_len_) {
    output_.writer().close();
  }
  return;
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  uint64_t pending = 0;
  for (const auto& pair : unassemble_map_) {
    pending += pair.second.length();
  }
  return pending;
}
