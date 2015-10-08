// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "BodyAction.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace protobuf {

void protobuf_ShutdownFile_BodyAction_2eproto() {
  delete BodyAction::default_instance_;
}

void protobuf_AddDesc_BodyAction_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf::protobuf_AddDesc_Point3D_2eproto();
  BodyAction::default_instance_ = new BodyAction();
  BodyAction::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_BodyAction_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_BodyAction_2eproto {
  StaticDescriptorInitializer_BodyAction_2eproto() {
    protobuf_AddDesc_BodyAction_2eproto();
  }
} static_descriptor_initializer_BodyAction_2eproto_;


// ===================================================================

bool BodyAction_StaticMotion_IsValid(int value) {
  switch(value) {
    case 0:
    case 6:
    case 7:
    case 8:
    case 11:
    case 12:
    case 14:
    case 15:
    case 32:
    case 33:
    case 34:
    case 50:
    case 51:
    case 52:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const BodyAction_StaticMotion BodyAction::Stop;
const BodyAction_StaticMotion BodyAction::KickLeft;
const BodyAction_StaticMotion BodyAction::KickRight;
const BodyAction_StaticMotion BodyAction::CancelKick;
const BodyAction_StaticMotion BodyAction::GoaliMoveLeft;
const BodyAction_StaticMotion BodyAction::GoaliMoveRight;
const BodyAction_StaticMotion BodyAction::MoveHi;
const BodyAction_StaticMotion BodyAction::YeahGoal;
const BodyAction_StaticMotion BodyAction::RaiseLeftHand;
const BodyAction_StaticMotion BodyAction::RaiseRightHand;
const BodyAction_StaticMotion BodyAction::RaiseBothHands;
const BodyAction_StaticMotion BodyAction::KickToTarget;
const BodyAction_StaticMotion BodyAction::KickBall;
const BodyAction_StaticMotion BodyAction::KickAdvanced;
const BodyAction_StaticMotion BodyAction::StaticMotion_MIN;
const BodyAction_StaticMotion BodyAction::StaticMotion_MAX;
const int BodyAction::StaticMotion_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int BodyAction::kWalkerCommandFieldNumber;
const int BodyAction::kWalkerXFieldNumber;
const int BodyAction::kWalkerYFieldNumber;
const int BodyAction::kWalkerRFieldNumber;
const int BodyAction::kWalkerTurningAimFieldNumber;
const int BodyAction::kMotionFieldNumber;
const int BodyAction::kBallFieldNumber;
const int BodyAction::kTargetFieldNumber;
#endif  // !_MSC_VER

BodyAction::BodyAction()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void BodyAction::InitAsDefaultInstance() {
  ball_ = const_cast< ::protobuf::Point3D*>(&::protobuf::Point3D::default_instance());
  target_ = const_cast< ::protobuf::Point3D*>(&::protobuf::Point3D::default_instance());
}

BodyAction::BodyAction(const BodyAction& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void BodyAction::SharedCtor() {
  _cached_size_ = 0;
  walkercommand_ = false;
  walkerx_ = 0;
  walkery_ = 0;
  walkerr_ = 0;
  walkerturningaim_ = false;
  motion_ = 0;
  ball_ = NULL;
  target_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BodyAction::~BodyAction() {
  SharedDtor();
}

void BodyAction::SharedDtor() {
  if (this != default_instance_) {
    delete ball_;
    delete target_;
  }
}

void BodyAction::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const BodyAction& BodyAction::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_BodyAction_2eproto();  return *default_instance_;
}

BodyAction* BodyAction::default_instance_ = NULL;

BodyAction* BodyAction::New() const {
  return new BodyAction;
}

void BodyAction::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    walkercommand_ = false;
    walkerx_ = 0;
    walkery_ = 0;
    walkerr_ = 0;
    walkerturningaim_ = false;
    motion_ = 0;
    if (has_ball()) {
      if (ball_ != NULL) ball_->::protobuf::Point3D::Clear();
    }
    if (has_target()) {
      if (target_ != NULL) target_->::protobuf::Point3D::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool BodyAction::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required bool WalkerCommand = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &walkercommand_)));
          set_has_walkercommand();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_WalkerX;
        break;
      }
      
      // optional sint32 WalkerX = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_WalkerX:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_SINT32>(
                 input, &walkerx_)));
          set_has_walkerx();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_WalkerY;
        break;
      }
      
      // optional sint32 WalkerY = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_WalkerY:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_SINT32>(
                 input, &walkery_)));
          set_has_walkery();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_WalkerR;
        break;
      }
      
      // optional sint32 WalkerR = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_WalkerR:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_SINT32>(
                 input, &walkerr_)));
          set_has_walkerr();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_WalkerTurningAim;
        break;
      }
      
      // optional bool WalkerTurningAim = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_WalkerTurningAim:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &walkerturningaim_)));
          set_has_walkerturningaim();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_Motion;
        break;
      }
      
      // optional .protobuf.BodyAction.StaticMotion Motion = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Motion:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::protobuf::BodyAction_StaticMotion_IsValid(value)) {
            set_motion(static_cast< ::protobuf::BodyAction_StaticMotion >(value));
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_Ball;
        break;
      }
      
      // optional .protobuf.Point3D Ball = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_Ball:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_ball()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_Target;
        break;
      }
      
      // optional .protobuf.Point3D Target = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_Target:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_target()));
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

void BodyAction::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required bool WalkerCommand = 1;
  if (has_walkercommand()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(1, this->walkercommand(), output);
  }
  
  // optional sint32 WalkerX = 2;
  if (has_walkerx()) {
    ::google::protobuf::internal::WireFormatLite::WriteSInt32(2, this->walkerx(), output);
  }
  
  // optional sint32 WalkerY = 3;
  if (has_walkery()) {
    ::google::protobuf::internal::WireFormatLite::WriteSInt32(3, this->walkery(), output);
  }
  
  // optional sint32 WalkerR = 4;
  if (has_walkerr()) {
    ::google::protobuf::internal::WireFormatLite::WriteSInt32(4, this->walkerr(), output);
  }
  
  // optional bool WalkerTurningAim = 5;
  if (has_walkerturningaim()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(5, this->walkerturningaim(), output);
  }
  
  // optional .protobuf.BodyAction.StaticMotion Motion = 6;
  if (has_motion()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      6, this->motion(), output);
  }
  
  // optional .protobuf.Point3D Ball = 7;
  if (has_ball()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      7, this->ball(), output);
  }
  
  // optional .protobuf.Point3D Target = 8;
  if (has_target()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      8, this->target(), output);
  }
  
}

int BodyAction::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required bool WalkerCommand = 1;
    if (has_walkercommand()) {
      total_size += 1 + 1;
    }
    
    // optional sint32 WalkerX = 2;
    if (has_walkerx()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::SInt32Size(
          this->walkerx());
    }
    
    // optional sint32 WalkerY = 3;
    if (has_walkery()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::SInt32Size(
          this->walkery());
    }
    
    // optional sint32 WalkerR = 4;
    if (has_walkerr()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::SInt32Size(
          this->walkerr());
    }
    
    // optional bool WalkerTurningAim = 5;
    if (has_walkerturningaim()) {
      total_size += 1 + 1;
    }
    
    // optional .protobuf.BodyAction.StaticMotion Motion = 6;
    if (has_motion()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->motion());
    }
    
    // optional .protobuf.Point3D Ball = 7;
    if (has_ball()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->ball());
    }
    
    // optional .protobuf.Point3D Target = 8;
    if (has_target()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->target());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BodyAction::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const BodyAction*>(&from));
}

void BodyAction::MergeFrom(const BodyAction& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_walkercommand()) {
      set_walkercommand(from.walkercommand());
    }
    if (from.has_walkerx()) {
      set_walkerx(from.walkerx());
    }
    if (from.has_walkery()) {
      set_walkery(from.walkery());
    }
    if (from.has_walkerr()) {
      set_walkerr(from.walkerr());
    }
    if (from.has_walkerturningaim()) {
      set_walkerturningaim(from.walkerturningaim());
    }
    if (from.has_motion()) {
      set_motion(from.motion());
    }
    if (from.has_ball()) {
      mutable_ball()->::protobuf::Point3D::MergeFrom(from.ball());
    }
    if (from.has_target()) {
      mutable_target()->::protobuf::Point3D::MergeFrom(from.target());
    }
  }
}

void BodyAction::CopyFrom(const BodyAction& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BodyAction::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  if (has_ball()) {
    if (!this->ball().IsInitialized()) return false;
  }
  if (has_target()) {
    if (!this->target().IsInitialized()) return false;
  }
  return true;
}

void BodyAction::Swap(BodyAction* other) {
  if (other != this) {
    std::swap(walkercommand_, other->walkercommand_);
    std::swap(walkerx_, other->walkerx_);
    std::swap(walkery_, other->walkery_);
    std::swap(walkerr_, other->walkerr_);
    std::swap(walkerturningaim_, other->walkerturningaim_);
    std::swap(motion_, other->motion_);
    std::swap(ball_, other->ball_);
    std::swap(target_, other->target_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string BodyAction::GetTypeName() const {
  return "protobuf.BodyAction";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)