// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ObjectList.proto

#ifndef PROTOBUF_ObjectList_2eproto__INCLUDED
#define PROTOBUF_ObjectList_2eproto__INCLUDED

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
#include "Object.pb.h"
// @@protoc_insertion_point(includes)

namespace protobuf {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_ObjectList_2eproto();
void protobuf_AssignDesc_ObjectList_2eproto();
void protobuf_ShutdownFile_ObjectList_2eproto();

class ObjectList;

// ===================================================================

class ObjectList : public ::google::protobuf::MessageLite {
 public:
  ObjectList();
  virtual ~ObjectList();
  
  ObjectList(const ObjectList& from);
  
  inline ObjectList& operator=(const ObjectList& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const ObjectList& default_instance();
  
  void Swap(ObjectList* other);
  
  // implements Message ----------------------------------------------
  
  ObjectList* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const ObjectList& from);
  void MergeFrom(const ObjectList& from);
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
  
  // repeated .protobuf.Object object = 1;
  inline int object_size() const;
  inline void clear_object();
  static const int kObjectFieldNumber = 1;
  inline const ::protobuf::Object& object(int index) const;
  inline ::protobuf::Object* mutable_object(int index);
  inline ::protobuf::Object* add_object();
  inline const ::google::protobuf::RepeatedPtrField< ::protobuf::Object >&
      object() const;
  inline ::google::protobuf::RepeatedPtrField< ::protobuf::Object >*
      mutable_object();
  
  // @@protoc_insertion_point(class_scope:protobuf.ObjectList)
 private:
  
  ::google::protobuf::RepeatedPtrField< ::protobuf::Object > object_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_ObjectList_2eproto();
  friend void protobuf_AssignDesc_ObjectList_2eproto();
  friend void protobuf_ShutdownFile_ObjectList_2eproto();
  
  void InitAsDefaultInstance();
  static ObjectList* default_instance_;
};
// ===================================================================


// ===================================================================

// ObjectList

// repeated .protobuf.Object object = 1;
inline int ObjectList::object_size() const {
  return object_.size();
}
inline void ObjectList::clear_object() {
  object_.Clear();
}
inline const ::protobuf::Object& ObjectList::object(int index) const {
  return object_.Get(index);
}
inline ::protobuf::Object* ObjectList::mutable_object(int index) {
  return object_.Mutable(index);
}
inline ::protobuf::Object* ObjectList::add_object() {
  return object_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::protobuf::Object >&
ObjectList::object() const {
  return object_;
}
inline ::google::protobuf::RepeatedPtrField< ::protobuf::Object >*
ObjectList::mutable_object() {
  return &object_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ObjectList_2eproto__INCLUDED
