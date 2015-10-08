#include "IstreamInputStream.h"

IstreamInputStream::IstreamInputStream(istream* input, int block_size)
  : copying_input_(input),
    impl_(&copying_input_, block_size) {
}

IstreamInputStream::~IstreamInputStream() {}

bool IstreamInputStream::Next(const void** data, int* size) {
  return impl_.Next(data, size);
}

void IstreamInputStream::BackUp(int count) {
  impl_.BackUp(count);
}

bool IstreamInputStream::Skip(int count) {
  return impl_.Skip(count);
}

int64 IstreamInputStream::ByteCount() const {
  return impl_.ByteCount();
}

IstreamInputStream::CopyingIstreamInputStream::CopyingIstreamInputStream(
    istream* input)
  : input_(input) {
}

IstreamInputStream::CopyingIstreamInputStream::~CopyingIstreamInputStream() {
	input_ = NULL;
}

int IstreamInputStream::CopyingIstreamInputStream::Read(
    void* buffer, int size) {
  input_->read(reinterpret_cast<char*>(buffer), (streamsize)size);
  int result = (int)(input_->gcount());
  if (result == 0 && input_->fail() && !input_->eof()) {
    return -1;
  }
  return result;
}
