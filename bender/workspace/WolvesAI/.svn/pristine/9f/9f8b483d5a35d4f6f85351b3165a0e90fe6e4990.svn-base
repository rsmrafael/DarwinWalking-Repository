#include "OstreamOutputStream.h"

OstreamOutputStream::OstreamOutputStream(ostream* output, int block_size)
  : copying_output_(output),
    impl_(&copying_output_, block_size) {
}

OstreamOutputStream::~OstreamOutputStream() {
  impl_.Flush();
}

bool OstreamOutputStream::Next(void** data, int* size) {
  return impl_.Next(data, size);
}

void OstreamOutputStream::BackUp(int count) {
  impl_.BackUp(count);
}

int64 OstreamOutputStream::ByteCount() const {
  return impl_.ByteCount();
}

OstreamOutputStream::CopyingOstreamOutputStream::CopyingOstreamOutputStream(ostream* output)
  : output_(output) {
}

OstreamOutputStream::CopyingOstreamOutputStream::~CopyingOstreamOutputStream() {
	output_ = NULL;
}

bool OstreamOutputStream::CopyingOstreamOutputStream::Write(const void* buffer, int size) {
  output_->write(reinterpret_cast<const char*>(buffer), (streamsize)size);
  return output_->good();
}
