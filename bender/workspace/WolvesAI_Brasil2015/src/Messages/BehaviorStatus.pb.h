// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BehaviorStatus.proto

#ifndef PROTOBUF_BehaviorStatus_2eproto__INCLUDED
#define PROTOBUF_BehaviorStatus_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_BehaviorStatus_2eproto();
void protobuf_AssignDesc_BehaviorStatus_2eproto();
void protobuf_ShutdownFile_BehaviorStatus_2eproto();

class BehaviorStatus;

// ===================================================================

class BehaviorStatus : public ::google::protobuf::MessageLite {
 public:
  BehaviorStatus();
  virtual ~BehaviorStatus();
  
  BehaviorStatus(const BehaviorStatus& from);
  
  inline BehaviorStatus& operator=(const BehaviorStatus& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const BehaviorStatus& default_instance();
  
  void Swap(BehaviorStatus* other);
  
  // implements Message ----------------------------------------------
  
  BehaviorStatus* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const BehaviorStatus& from);
  void MergeFrom(const BehaviorStatus& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string status = 1;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 1;
  inline const ::std::string& status() const;
  inline void set_status(const ::std::string& value);
  inline void set_status(const char* value);
  inline void set_status(const char* value, size_t size);
  inline ::std::string* mutable_status();
  inline ::std::string* release_status();
  
  // @@protoc_insertion_point(class_scope:protobuf.BehaviorStatus)
 private:
  inline void set_has_status();
  inline void clear_has_status();
  
  ::std::string* status_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_BehaviorStatus_2eproto();
  friend void protobuf_AssignDesc_BehaviorStatus_2eproto();
  friend void protobuf_ShutdownFile_BehaviorStatus_2eproto();
  
  void InitAsDefaultInstance();
  static BehaviorStatus* default_instance_;
};
// ===================================================================


// ===================================================================

// BehaviorStatus

// required string status = 1;
inline bool BehaviorStatus::has_status() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void BehaviorStatus::set_has_status() {
  _has_bits_[0] |= 0x00000001u;
}
inline void BehaviorStatus::clear_has_status() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void BehaviorStatus::clear_status() {
  if (status_ != &::google::protobuf::internal::kEmptyString) {
    status_->clear();
  }
  clear_has_status();
}
inline const ::std::string& BehaviorStatus::status() const {
  return *status_;
}
inline void BehaviorStatus::set_status(const ::std::string& value) {
  set_has_status();
  if (status_ == &::google::protobuf::internal::kEmptyString) {
    status_ = new ::std::string;
  }
  status_->assign(value);
}
inline void BehaviorStatus::set_status(const char* value) {
  set_has_status();
  if (status_ == &::google::protobuf::internal::kEmptyString) {
    status_ = new ::std::string;
  }
  status_->assign(value);
}
inline void BehaviorStatus::set_status(const char* value, size_t size) {
  set_has_status();
  if (status_ == &::google::protobuf::internal::kEmptyString) {
    status_ = new ::std::string;
  }
  status_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* BehaviorStatus::mutable_status() {
  set_has_status();
  if (status_ == &::google::protobuf::internal::kEmptyString) {
    status_ = new ::std::string;
  }
  return status_;
}
inline ::std::string* BehaviorStatus::release_status() {
  clear_has_status();
  if (status_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = status_;
    status_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_BehaviorStatus_2eproto__INCLUDED
