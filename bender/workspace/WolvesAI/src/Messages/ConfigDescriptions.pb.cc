// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ConfigDescriptions.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace protobuf {

void protobuf_ShutdownFile_ConfigDescriptions_2eproto() {
  delete ConfigDescriptions::default_instance_;
  delete ConfigDescriptions_ConfigDescription::default_instance_;
}

void protobuf_AddDesc_ConfigDescriptions_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ConfigDescriptions::default_instance_ = new ConfigDescriptions();
  ConfigDescriptions_ConfigDescription::default_instance_ = new ConfigDescriptions_ConfigDescription();
  ConfigDescriptions::default_instance_->InitAsDefaultInstance();
  ConfigDescriptions_ConfigDescription::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ConfigDescriptions_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ConfigDescriptions_2eproto {
  StaticDescriptorInitializer_ConfigDescriptions_2eproto() {
    protobuf_AddDesc_ConfigDescriptions_2eproto();
  }
} static_descriptor_initializer_ConfigDescriptions_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int ConfigDescriptions_ConfigDescription::kSettingFieldNumber;
const int ConfigDescriptions_ConfigDescription::kDescriptionFieldNumber;
#endif  // !_MSC_VER

ConfigDescriptions_ConfigDescription::ConfigDescriptions_ConfigDescription()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void ConfigDescriptions_ConfigDescription::InitAsDefaultInstance() {
}

ConfigDescriptions_ConfigDescription::ConfigDescriptions_ConfigDescription(const ConfigDescriptions_ConfigDescription& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void ConfigDescriptions_ConfigDescription::SharedCtor() {
  _cached_size_ = 0;
  setting_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  description_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ConfigDescriptions_ConfigDescription::~ConfigDescriptions_ConfigDescription() {
  SharedDtor();
}

void ConfigDescriptions_ConfigDescription::SharedDtor() {
  if (setting_ != &::google::protobuf::internal::kEmptyString) {
    delete setting_;
  }
  if (description_ != &::google::protobuf::internal::kEmptyString) {
    delete description_;
  }
  if (this != default_instance_) {
  }
}

void ConfigDescriptions_ConfigDescription::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ConfigDescriptions_ConfigDescription& ConfigDescriptions_ConfigDescription::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ConfigDescriptions_2eproto();  return *default_instance_;
}

ConfigDescriptions_ConfigDescription* ConfigDescriptions_ConfigDescription::default_instance_ = NULL;

ConfigDescriptions_ConfigDescription* ConfigDescriptions_ConfigDescription::New() const {
  return new ConfigDescriptions_ConfigDescription;
}

void ConfigDescriptions_ConfigDescription::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_setting()) {
      if (setting_ != &::google::protobuf::internal::kEmptyString) {
        setting_->clear();
      }
    }
    if (has_description()) {
      if (description_ != &::google::protobuf::internal::kEmptyString) {
        description_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool ConfigDescriptions_ConfigDescription::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string setting = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_setting()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_description;
        break;
      }
      
      // required string description = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_description:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_description()));
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

void ConfigDescriptions_ConfigDescription::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string setting = 1;
  if (has_setting()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->setting(), output);
  }
  
  // required string description = 2;
  if (has_description()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->description(), output);
  }
  
}

int ConfigDescriptions_ConfigDescription::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string setting = 1;
    if (has_setting()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->setting());
    }
    
    // required string description = 2;
    if (has_description()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->description());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ConfigDescriptions_ConfigDescription::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const ConfigDescriptions_ConfigDescription*>(&from));
}

void ConfigDescriptions_ConfigDescription::MergeFrom(const ConfigDescriptions_ConfigDescription& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_setting()) {
      set_setting(from.setting());
    }
    if (from.has_description()) {
      set_description(from.description());
    }
  }
}

void ConfigDescriptions_ConfigDescription::CopyFrom(const ConfigDescriptions_ConfigDescription& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ConfigDescriptions_ConfigDescription::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  return true;
}

void ConfigDescriptions_ConfigDescription::Swap(ConfigDescriptions_ConfigDescription* other) {
  if (other != this) {
    std::swap(setting_, other->setting_);
    std::swap(description_, other->description_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string ConfigDescriptions_ConfigDescription::GetTypeName() const {
  return "protobuf.ConfigDescriptions.ConfigDescription";
}


// -------------------------------------------------------------------

#ifndef _MSC_VER
const int ConfigDescriptions::kDescriptionFieldNumber;
#endif  // !_MSC_VER

ConfigDescriptions::ConfigDescriptions()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void ConfigDescriptions::InitAsDefaultInstance() {
}

ConfigDescriptions::ConfigDescriptions(const ConfigDescriptions& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void ConfigDescriptions::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ConfigDescriptions::~ConfigDescriptions() {
  SharedDtor();
}

void ConfigDescriptions::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ConfigDescriptions::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ConfigDescriptions& ConfigDescriptions::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ConfigDescriptions_2eproto();  return *default_instance_;
}

ConfigDescriptions* ConfigDescriptions::default_instance_ = NULL;

ConfigDescriptions* ConfigDescriptions::New() const {
  return new ConfigDescriptions;
}

void ConfigDescriptions::Clear() {
  description_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool ConfigDescriptions::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .protobuf.ConfigDescriptions.ConfigDescription description = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_description:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_description()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_description;
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

void ConfigDescriptions::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .protobuf.ConfigDescriptions.ConfigDescription description = 1;
  for (int i = 0; i < this->description_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->description(i), output);
  }
  
}

int ConfigDescriptions::ByteSize() const {
  int total_size = 0;
  
  // repeated .protobuf.ConfigDescriptions.ConfigDescription description = 1;
  total_size += 1 * this->description_size();
  for (int i = 0; i < this->description_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->description(i));
  }
  
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ConfigDescriptions::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const ConfigDescriptions*>(&from));
}

void ConfigDescriptions::MergeFrom(const ConfigDescriptions& from) {
  GOOGLE_CHECK_NE(&from, this);
  description_.MergeFrom(from.description_);
}

void ConfigDescriptions::CopyFrom(const ConfigDescriptions& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ConfigDescriptions::IsInitialized() const {
  
  for (int i = 0; i < description_size(); i++) {
    if (!this->description(i).IsInitialized()) return false;
  }
  return true;
}

void ConfigDescriptions::Swap(ConfigDescriptions* other) {
  if (other != this) {
    description_.Swap(&other->description_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string ConfigDescriptions::GetTypeName() const {
  return "protobuf.ConfigDescriptions";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)