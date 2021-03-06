// Copyright 2016 Husky Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cassert>
#include <cstring>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace base {

class BinStream {
   public:
    BinStream();
    explicit BinStream(size_t sz);
    BinStream(const char* src, size_t sz);
    BinStream(const BinStream& stream);
    BinStream(BinStream&& stream);
    BinStream& operator=(BinStream&& stream);
    size_t hash();

    void clear();
    void purge();
    void resize(size_t size);
    void seek(size_t pos);

    void push_back_bytes(const char* src, size_t sz);
    void* pop_front_bytes(size_t sz);
    void append(const BinStream& m);

    /// Note that this method just returns the pointer pointing to the very
    /// beginning of the buffer_, and doesn't care about how much data have
    /// been read.
    inline char* get_buffer() { return &buffer_[0]; }
    inline const char* get_remained_buffer() const { return (&buffer_[0]) + front_; }
    inline size_t size() const { return buffer_.size() - front_; }
    inline std::string to_string() const { return std::string(buffer_.begin() + front_, buffer_.end()); }

   private:
    size_t front_;
    std::vector<char> buffer_;
};

template <typename InputT>
BinStream& operator<<(BinStream& stream, const InputT& x) {
    stream.push_back_bytes((char*)&x, sizeof(InputT));
    return stream;
}

template <typename OutputT>
BinStream& operator>>(BinStream& stream, OutputT& x) {
    x = *(OutputT*)(stream.pop_front_bytes(sizeof(OutputT)));
    return stream;
}

template <typename InputT>
BinStream& operator<<(BinStream& stream, const std::vector<InputT>& v) {
    size_t len = v.size();
    stream << len;
    for (int i = 0; i < v.size(); ++i)
        stream << v[i];
    return stream;
}

template <typename OutputT>
BinStream& operator>>(BinStream& stream, std::vector<OutputT>& v) {
    size_t len;
    stream >> len;
    v.clear();
    v.resize(len);
    for (int i = 0; i < v.size(); ++i)
        stream >> v[i];
    return stream;
}

template <typename K, typename V>
BinStream& operator<<(BinStream& stream, const std::map<K, V>& map) {
    size_t len = map.size();
    stream << len;
    for (auto& elem : map)
        stream << elem;
    return stream;
}

template <typename K, typename V>
BinStream& operator>>(BinStream& stream, std::map<K, V>& map) {
    size_t len;
    stream >> len;
    map.clear();
    for (int i = 0; i < len; i++) {
        std::pair<K, V> elem;
        stream >> elem;
        map[elem.first] = elem.second;
    }
    return stream;
}

template <typename InputT>
BinStream& operator<<(BinStream& stream, const std::basic_string<InputT>& v) {
    size_t len = v.size();
    stream << len;
    for (auto& elem : v)
        stream << elem;
    return stream;
}

template <typename OutputT>
BinStream& operator>>(BinStream& stream, std::basic_string<OutputT>& v) {
    size_t len;
    stream >> len;
    v.clear();
    try {
        v.resize(len);
    } catch (std::exception e) {
        assert(false);
    }
    for (auto& elem : v)
        stream >> elem;
    return stream;
}

template <typename FirstT, typename SecondT>
BinStream& operator<<(BinStream& stream, const std::pair<FirstT, SecondT>& p) {
    stream << p.first << p.second;
    return stream;
}

template <typename FirstT, typename SecondT>
BinStream& operator>>(BinStream& stream, std::pair<FirstT, SecondT>& p) {
    stream >> p.first >> p.second;
    return stream;
}

BinStream& operator<<(BinStream& stream, const BinStream& bin);
BinStream& operator>>(BinStream& stream, BinStream& bin);

BinStream& operator<<(BinStream& stream, const std::string& x);
BinStream& operator>>(BinStream& stream, std::string& x);

BinStream& operator<<(BinStream& stream, const std::vector<bool>& v);
BinStream& operator>>(BinStream& stream, std::vector<bool>& v);

}  // namespace base
