/*---[BufferOutputStream.cpp]----------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::BufferOutputStream::*
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 *//////////////////////////////////////////////////////////////////////////////

// Std
#include <cstdio>
#include <cstring>

// Internal
#include "System.h"
#include "Ptr.h"
#include "InputStream.h"

// Self
#include "BufferOutputStream.h"

#define BUFFER_GROWTH_RATE 2

namespace kfoundation {
  
  string BufferOutputStream::toBinaryString(const kf_octet_t *data, int size) {
    using namespace std;
    char* chars = new char[size*3 + 3];
    char* p = chars;
    for(int i = 0; i < size; i++) {
      p += sprintf(p, "%02X ", (int)data[i]);
    }
    string str(chars);
    delete[] chars;
    return str;
  }
  
  
  /**
   * Constructor, creates a new stream with an internal buffer of given
   * capacity. If more octets than the given capacity is written to it, the
   * buffer size will grow exponentially.
   *
   * @param capacity The initial capacity of the internal buffer.
   */
  
  BufferOutputStream::BufferOutputStream(const kf_int32_t capacity) {
    _capacity = capacity;
    _size = 0;
    _data = new kf_octet_t[_capacity];
  }
  
  
  /**
   * Deconstructor.
   */
  
  BufferOutputStream::~BufferOutputStream() {
    delete[] _data;
  }
  
  
  void BufferOutputStream::grow() {
    kf_int32_t newCapacity = _capacity * BUFFER_GROWTH_RATE;
    kf_octet_t* newData = new kf_octet_t[newCapacity];
    memcpy(newData, _data, _size);
    delete[] _data;
    _data = newData;
    _capacity = newCapacity;
  }
  
  
  /**
   * Returns the pointer to the first byte of the internal buffer. 
   * @note The returned value might change as the internal buffer expands.
   */
  
  kf_octet_t* BufferOutputStream::getData() const {
    return _data;
  }
  
  
  /**
   * Returns the number of octets written to this stream.
   */
  
  kf_int32_t BufferOutputStream::getSize() const {
    return _size;
  }
  
  
  // Inherited from InputStream
  
  bool BufferOutputStream::isBigEndian() const {
    return System::isBigEndian();
  }
  
  
  void BufferOutputStream::write(const kf_octet_t* buffer,
      const kf_int32_t nBytes)
  {
    while(_size + nBytes > _capacity) {
      grow();
    }
    
    memcpy(_data + _size, buffer, nBytes);
    _size += nBytes;
  }
  
  
  void BufferOutputStream::write(kf_octet_t byte) {
    if(_size >= _capacity) {
      grow();
    }
    
    _data[_size] = byte;
    _size++;
  }
  
  
  void BufferOutputStream::write(PPtr<InputStream> is) {
    kf_octet_t tmp[1024];
    
    while(!is->isEof()) {
      int s = is->read(tmp, 1024);
      write(tmp, s);
    }
  }
  
  
  void BufferOutputStream::close() {
    // Nothing
  }
  
} // namespace kfoundation

