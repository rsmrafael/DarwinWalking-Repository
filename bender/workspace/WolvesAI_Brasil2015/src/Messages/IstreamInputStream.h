/*
 * IstreamInputStream.h
 *
 *  Created on: 22.08.2012
 *      Author: Stefan Krupop, stolen from Google
 */

#ifndef ISTREAMINPUTSTREAM_H_
#define ISTREAMINPUTSTREAM_H_

#include <iostream>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/stubs/common.h>

using namespace google::protobuf::io;
using namespace google::protobuf;
using namespace std;

// A ZeroCopyInputStream which reads from a C++ istream.
//
// Note that for reading files (or anything represented by a file descriptor),
// FileInputStream is more efficient.
class IstreamInputStream : public ZeroCopyInputStream {
 public:
  // Creates a stream that reads from the given C++ istream.
  // If a block_size is given, it specifies the number of bytes that
  // should be read and returned with each call to Next().  Otherwise,
  // a reasonable default is used.
  explicit IstreamInputStream(istream* stream, int block_size = -1);
  ~IstreamInputStream();

  // implements ZeroCopyInputStream ----------------------------------
  bool Next(const void** data, int* size);
  void BackUp(int count);
  bool Skip(int count);
  int64 ByteCount() const;

 private:
  class CopyingIstreamInputStream : public CopyingInputStream {
   public:
    CopyingIstreamInputStream(istream* input);
    ~CopyingIstreamInputStream();

    // implements CopyingInputStream ---------------------------------
    int Read(void* buffer, int size);
    // (We use the default implementation of Skip().)

   private:
    // The stream.
    istream* input_;

    //lint -e(1704)
    CopyingIstreamInputStream(const CopyingIstreamInputStream&);
    CopyingIstreamInputStream& operator=(const CopyingIstreamInputStream&);
  };

  CopyingIstreamInputStream copying_input_;
  CopyingInputStreamAdaptor impl_;

  //lint -e(1704)
  IstreamInputStream(const IstreamInputStream&);
  IstreamInputStream& operator=(const IstreamInputStream&);
};

#endif /* ISTREAMINPUTSTREAM_H_ */
