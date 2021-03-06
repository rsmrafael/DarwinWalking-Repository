// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: PointCloud.proto

#ifndef PROTOBUF_PointCloud_2eproto__INCLUDED
#define PROTOBUF_PointCloud_2eproto__INCLUDED

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
#include "Point.pb.h"
// @@protoc_insertion_point(includes)

namespace protobuf {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_PointCloud_2eproto();
void protobuf_AssignDesc_PointCloud_2eproto();
void protobuf_ShutdownFile_PointCloud_2eproto();

class PointCloud;

// ===================================================================

class PointCloud : public ::google::protobuf::MessageLite {
 public:
  PointCloud();
  virtual ~PointCloud();
  
  PointCloud(const PointCloud& from);
  
  inline PointCloud& operator=(const PointCloud& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const PointCloud& default_instance();
  
  void Swap(PointCloud* other);
  
  // implements Message ----------------------------------------------
  
  PointCloud* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const PointCloud& from);
  void MergeFrom(const PointCloud& from);
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
  
  // repeated .protobuf.Point point = 1;
  inline int point_size() const;
  inline void clear_point();
  static const int kPointFieldNumber = 1;
  inline const ::protobuf::Point& point(int index) const;
  inline ::protobuf::Point* mutable_point(int index);
  inline ::protobuf::Point* add_point();
  inline const ::google::protobuf::RepeatedPtrField< ::protobuf::Point >&
      point() const;
  inline ::google::protobuf::RepeatedPtrField< ::protobuf::Point >*
      mutable_point();
  
  // @@protoc_insertion_point(class_scope:protobuf.PointCloud)
 private:
  
  ::google::protobuf::RepeatedPtrField< ::protobuf::Point > point_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_PointCloud_2eproto();
  friend void protobuf_AssignDesc_PointCloud_2eproto();
  friend void protobuf_ShutdownFile_PointCloud_2eproto();
  
  void InitAsDefaultInstance();
  static PointCloud* default_instance_;
};
// ===================================================================


// ===================================================================

// PointCloud

// repeated .protobuf.Point point = 1;
inline int PointCloud::point_size() const {
  return point_.size();
}
inline void PointCloud::clear_point() {
  point_.Clear();
}
inline const ::protobuf::Point& PointCloud::point(int index) const {
  return point_.Get(index);
}
inline ::protobuf::Point* PointCloud::mutable_point(int index) {
  return point_.Mutable(index);
}
inline ::protobuf::Point* PointCloud::add_point() {
  return point_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::protobuf::Point >&
PointCloud::point() const {
  return point_;
}
inline ::google::protobuf::RepeatedPtrField< ::protobuf::Point >*
PointCloud::mutable_point() {
  return &point_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_PointCloud_2eproto__INCLUDED
