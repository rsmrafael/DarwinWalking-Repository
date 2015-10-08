/*
 * OstreamOutputStream.h
 *
 *  Created on: 15.08.2012
 *      Author: Stefan Krupop, stolen from Google
 */

#ifndef OSTREAMOUTPUTSTREAM_H_
#define OSTREAMOUTPUTSTREAM_H_

#include <iostream>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/stubs/common.h>

using namespace google::protobuf::io;
using namespace google::protobuf;
using namespace std;

// A ZeroCopyOutputStream which writes to a C++ ostream.
//
// Note that for writing files (or anything represented by a file descriptor),
// FileOutputStream is more efficient.
class OstreamOutputStream : public ZeroCopyOutputStream {
 public:
  // Creates a stream that writes to the given C++ ostream.
  // If a block_size is given, it specifies the size of the buffers
  // that should be returned by Next().  Otherwise, a reasonable default
  // is used.
  explicit OstreamOutputStream(ostream* stream, int block_size = -1);
  ~OstreamOutputStream();

  // implements ZeroCopyOutputStream ---------------------------------
  bool Next(void** data, int* size);
  void BackUp(int count);
  int64 ByteCount() const;

 private:
  class CopyingOstreamOutputStream : public CopyingOutputStream {
   public:
    CopyingOstreamOutputStream(ostream* output);
    ~CopyingOstreamOutputStream();

    // implements CopyingOutputStream --------------------------------
    bool Write(const void* buffer, int size);

   private:
    // The stream.
    ostream* output_;

    //lint -e(1704)
    CopyingOstreamOutputStream(const CopyingOstreamOutputStream&);
    CopyingOstreamOutputStream& operator=(const CopyingOstreamOutputStream&);
  };

  CopyingOstreamOutputStream copying_output_;
  CopyingOutputStreamAdaptor impl_;

  //lint -e(1704)
  OstreamOutputStream(const OstreamOutputStream&);
  OstreamOutputStream& operator=(const OstreamOutputStream&);
};

#endif /* OSTREAMOUTPUTSTREAM_H_ */
