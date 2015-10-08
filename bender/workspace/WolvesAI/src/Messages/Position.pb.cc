// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Position.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace protobuf {

void protobuf_ShutdownFile_Position_2eproto() {
  delete Position::default_instance_;
}

void protobuf_AddDesc_Position_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  Position::default_instance_ = new Position();
  Position::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Position_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Position_2eproto {
  StaticDescriptorInitializer_Position_2eproto() {
    protobuf_AddDesc_Position_2eproto();
  }
} static_descriptor_initializer_Position_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int Position::kAngleFieldNumber;
const int Position::kLengthFieldNumber;
const int Position::kAccuracyFieldNumber;
#endif  // !_MSC_VER

Position::Position()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void Position::InitAsDefaultInstance() {
}

Position::Position(const Position& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void Position::SharedCtor() {
  _cached_size_ = 0;
  angle_ = 0;
  length_ = 0;
  accuracy_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Position::~Position() {
  SharedDtor();
}

void Position::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Position::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const Position& Position::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Position_2eproto();  return *default_instance_;
}

Position* Position::default_instance_ = NULL;

Position* Position::New() const {
  return new Position;
}

void Position::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    angle_ = 0;
    length_ = 0;
    accuracy_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool Position::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required double angle = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED64) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &angle_)));
          set_has_angle();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(17)) goto parse_length;
        break;
      }
      
      // required double length = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED64) {
         parse_length:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &length_)));
          set_has_length();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(25)) goto parse_accuracy;
        break;
      }
      
      // required double accuracy = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED64) {
         parse_accuracy:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &accuracy_)));
          set_has_accuracy();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Position::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required double angle = 1;
  if (has_angle()) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(1, this->angle(), output);
  }
  
  // required double length = 2;
  if (has_length()) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(2, this->length(), output);
  }
  
  // required double accuracy = 3;
  if (has_accuracy()) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(3, this->accuracy(), output);
  }
  
}

int Position::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required double angle = 1;
    if (has_angle()) {
      total_size += 1 + 8;
    }
    
    // required double length = 2;
    if (has_length()) {
      total_size += 1 + 8;
    }
    
    // required double accuracy = 3;
    if (has_accuracy()) {
      total_size += 1 + 8;
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Position::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const Position*>(&from));
}

void Position::MergeFrom(const Position& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_angle()) {
      set_angle(from.angle());
    }
    if (from.has_length()) {
      set_length(from.length());
    }
    if (from.has_accuracy()) {
      set_accuracy(from.accuracy());
    }
  }
}

void Position::CopyFrom(const Position& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Position::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;
  
  return true;
}

void Position::Swap(Position* other) {
  if (other != this) {
    std::swap(angle_, other->angle_);
    std::swap(length_, other->length_);
    std::swap(accuracy_, other->accuracy_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string Position::GetTypeName() const {
  return "protobuf.Position";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)
