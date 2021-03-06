// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: HistogramServiceCommand.proto

#ifndef PROTOBUF_HistogramServiceCommand_2eproto__INCLUDED
#define PROTOBUF_HistogramServiceCommand_2eproto__INCLUDED

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
void  protobuf_AddDesc_HistogramServiceCommand_2eproto();
void protobuf_AssignDesc_HistogramServiceCommand_2eproto();
void protobuf_ShutdownFile_HistogramServiceCommand_2eproto();

class HistogramServiceCommand;

enum HistogramServiceCommand_ServiceAction {
  HistogramServiceCommand_ServiceAction_SAVE_OPPONENT = 0,
  HistogramServiceCommand_ServiceAction_SAVE_OWN = 1,
  HistogramServiceCommand_ServiceAction_DELETE_ALL = 2
};
bool HistogramServiceCommand_ServiceAction_IsValid(int value);
const HistogramServiceCommand_ServiceAction HistogramServiceCommand_ServiceAction_ServiceAction_MIN = HistogramServiceCommand_ServiceAction_SAVE_OPPONENT;
const HistogramServiceCommand_ServiceAction HistogramServiceCommand_ServiceAction_ServiceAction_MAX = HistogramServiceCommand_ServiceAction_DELETE_ALL;
const int HistogramServiceCommand_ServiceAction_ServiceAction_ARRAYSIZE = HistogramServiceCommand_ServiceAction_ServiceAction_MAX + 1;

// ===================================================================

class HistogramServiceCommand : public ::google::protobuf::MessageLite {
 public:
  HistogramServiceCommand();
  virtual ~HistogramServiceCommand();
  
  HistogramServiceCommand(const HistogramServiceCommand& from);
  
  inline HistogramServiceCommand& operator=(const HistogramServiceCommand& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const HistogramServiceCommand& default_instance();
  
  void Swap(HistogramServiceCommand* other);
  
  // implements Message ----------------------------------------------
  
  HistogramServiceCommand* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const HistogramServiceCommand& from);
  void MergeFrom(const HistogramServiceCommand& from);
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
  
  typedef HistogramServiceCommand_ServiceAction ServiceAction;
  static const ServiceAction SAVE_OPPONENT = HistogramServiceCommand_ServiceAction_SAVE_OPPONENT;
  static const ServiceAction SAVE_OWN = HistogramServiceCommand_ServiceAction_SAVE_OWN;
  static const ServiceAction DELETE_ALL = HistogramServiceCommand_ServiceAction_DELETE_ALL;
  static inline bool ServiceAction_IsValid(int value) {
    return HistogramServiceCommand_ServiceAction_IsValid(value);
  }
  static const ServiceAction ServiceAction_MIN =
    HistogramServiceCommand_ServiceAction_ServiceAction_MIN;
  static const ServiceAction ServiceAction_MAX =
    HistogramServiceCommand_ServiceAction_ServiceAction_MAX;
  static const int ServiceAction_ARRAYSIZE =
    HistogramServiceCommand_ServiceAction_ServiceAction_ARRAYSIZE;
  
  // accessors -------------------------------------------------------
  
  // required .protobuf.HistogramServiceCommand.ServiceAction desiredAction = 1;
  inline bool has_desiredaction() const;
  inline void clear_desiredaction();
  static const int kDesiredActionFieldNumber = 1;
  inline ::protobuf::HistogramServiceCommand_ServiceAction desiredaction() const;
  inline void set_desiredaction(::protobuf::HistogramServiceCommand_ServiceAction value);
  
  // @@protoc_insertion_point(class_scope:protobuf.HistogramServiceCommand)
 private:
  inline void set_has_desiredaction();
  inline void clear_has_desiredaction();
  
  int desiredaction_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_HistogramServiceCommand_2eproto();
  friend void protobuf_AssignDesc_HistogramServiceCommand_2eproto();
  friend void protobuf_ShutdownFile_HistogramServiceCommand_2eproto();
  
  void InitAsDefaultInstance();
  static HistogramServiceCommand* default_instance_;
};
// ===================================================================


// ===================================================================

// HistogramServiceCommand

// required .protobuf.HistogramServiceCommand.ServiceAction desiredAction = 1;
inline bool HistogramServiceCommand::has_desiredaction() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void HistogramServiceCommand::set_has_desiredaction() {
  _has_bits_[0] |= 0x00000001u;
}
inline void HistogramServiceCommand::clear_has_desiredaction() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void HistogramServiceCommand::clear_desiredaction() {
  desiredaction_ = 0;
  clear_has_desiredaction();
}
inline ::protobuf::HistogramServiceCommand_ServiceAction HistogramServiceCommand::desiredaction() const {
  return static_cast< ::protobuf::HistogramServiceCommand_ServiceAction >(desiredaction_);
}
inline void HistogramServiceCommand::set_desiredaction(::protobuf::HistogramServiceCommand_ServiceAction value) {
  GOOGLE_DCHECK(::protobuf::HistogramServiceCommand_ServiceAction_IsValid(value));
  set_has_desiredaction();
  desiredaction_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_HistogramServiceCommand_2eproto__INCLUDED
