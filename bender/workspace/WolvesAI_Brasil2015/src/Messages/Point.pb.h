// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Point.proto

#ifndef PROTOBUF_Point_2eproto__INCLUDED
#define PROTOBUF_Point_2eproto__INCLUDED

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
void  protobuf_AddDesc_Point_2eproto();
void protobuf_AssignDesc_Point_2eproto();
void protobuf_ShutdownFile_Point_2eproto();

class Point;

// ===================================================================

class Point : public ::google::protobuf::MessageLite {
 public:
  Point();
  virtual ~Point();
  
  Point(const Point& from);
  
  inline Point& operator=(const Point& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const Point& default_instance();
  
  void Swap(Point* other);
  
  // implements Message ----------------------------------------------
  
  Point* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const Point& from);
  void MergeFrom(const Point& from);
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
  
  // required int32 x = 1;
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 1;
  inline ::google::protobuf::int32 x() const;
  inline void set_x(::google::protobuf::int32 value);
  
  // required int32 y = 2;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 2;
  inline ::google::protobuf::int32 y() const;
  inline void set_y(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:protobuf.Point)
 private:
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();
  
  ::google::protobuf::int32 x_;
  ::google::protobuf::int32 y_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_Point_2eproto();
  friend void protobuf_AssignDesc_Point_2eproto();
  friend void protobuf_ShutdownFile_Point_2eproto();
  
  void InitAsDefaultInstance();
  static Point* default_instance_;
};
// ===================================================================


// ===================================================================

// Point

// required int32 x = 1;
inline bool Point::has_x() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Point::set_has_x() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Point::clear_has_x() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Point::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline ::google::protobuf::int32 Point::x() const {
  return x_;
}
inline void Point::set_x(::google::protobuf::int32 value) {
  set_has_x();
  x_ = value;
}

// required int32 y = 2;
inline bool Point::has_y() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Point::set_has_y() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Point::clear_has_y() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Point::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline ::google::protobuf::int32 Point::y() const {
  return y_;
}
inline void Point::set_y(::google::protobuf::int32 value) {
  set_has_y();
  y_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Point_2eproto__INCLUDED
