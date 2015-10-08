// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BallModel.proto

package de.ostfalia.robocup.message;

public final class MsgBallModel {
  private MsgBallModel() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }
  public interface BallModelOrBuilder
      extends com.google.protobuf.MessageLiteOrBuilder {
    
    // required int32 x = 1;
    boolean hasX();
    int getX();
    
    // required int32 y = 2;
    boolean hasY();
    int getY();
  }
  public static final class BallModel extends
      com.google.protobuf.GeneratedMessageLite
      implements BallModelOrBuilder {
    // Use BallModel.newBuilder() to construct.
    private BallModel(Builder builder) {
      super(builder);
    }
    private BallModel(boolean noInit) {}
    
    private static final BallModel defaultInstance;
    public static BallModel getDefaultInstance() {
      return defaultInstance;
    }
    
    public BallModel getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    private int bitField0_;
    // required int32 x = 1;
    public static final int X_FIELD_NUMBER = 1;
    private int x_;
    public boolean hasX() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    public int getX() {
      return x_;
    }
    
    // required int32 y = 2;
    public static final int Y_FIELD_NUMBER = 2;
    private int y_;
    public boolean hasY() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    public int getY() {
      return y_;
    }
    
    private void initFields() {
      x_ = 0;
      y_ = 0;
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;
      
      if (!hasX()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!hasY()) {
        memoizedIsInitialized = 0;
        return false;
      }
      memoizedIsInitialized = 1;
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeInt32(1, x_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeInt32(2, y_);
      }
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(1, x_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(2, y_);
      }
      memoizedSerializedSize = size;
      return size;
    }
    
    private static final long serialVersionUID = 0L;
    @java.lang.Override
    protected java.lang.Object writeReplace()
        throws java.io.ObjectStreamException {
      return super.writeReplace();
    }
    
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input, extensionRegistry)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgBallModel.BallModel parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(de.ostfalia.robocup.message.MsgBallModel.BallModel prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          de.ostfalia.robocup.message.MsgBallModel.BallModel, Builder>
        implements de.ostfalia.robocup.message.MsgBallModel.BallModelOrBuilder {
      // Construct using de.ostfalia.robocup.message.MsgBallModel.BallModel.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }
      
      private void maybeForceBuilderInitialization() {
      }
      private static Builder create() {
        return new Builder();
      }
      
      public Builder clear() {
        super.clear();
        x_ = 0;
        bitField0_ = (bitField0_ & ~0x00000001);
        y_ = 0;
        bitField0_ = (bitField0_ & ~0x00000002);
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }
      
      public de.ostfalia.robocup.message.MsgBallModel.BallModel getDefaultInstanceForType() {
        return de.ostfalia.robocup.message.MsgBallModel.BallModel.getDefaultInstance();
      }
      
      public de.ostfalia.robocup.message.MsgBallModel.BallModel build() {
        de.ostfalia.robocup.message.MsgBallModel.BallModel result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }
      
      private de.ostfalia.robocup.message.MsgBallModel.BallModel buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        de.ostfalia.robocup.message.MsgBallModel.BallModel result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return result;
      }
      
      public de.ostfalia.robocup.message.MsgBallModel.BallModel buildPartial() {
        de.ostfalia.robocup.message.MsgBallModel.BallModel result = new de.ostfalia.robocup.message.MsgBallModel.BallModel(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.x_ = x_;
        if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
          to_bitField0_ |= 0x00000002;
        }
        result.y_ = y_;
        result.bitField0_ = to_bitField0_;
        return result;
      }
      
      public Builder mergeFrom(de.ostfalia.robocup.message.MsgBallModel.BallModel other) {
        if (other == de.ostfalia.robocup.message.MsgBallModel.BallModel.getDefaultInstance()) return this;
        if (other.hasX()) {
          setX(other.getX());
        }
        if (other.hasY()) {
          setY(other.getY());
        }
        return this;
      }
      
      public final boolean isInitialized() {
        if (!hasX()) {
          
          return false;
        }
        if (!hasY()) {
          
          return false;
        }
        return true;
      }
      
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        while (true) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              
              return this;
            default: {
              if (!parseUnknownField(input, extensionRegistry, tag)) {
                
                return this;
              }
              break;
            }
            case 8: {
              bitField0_ |= 0x00000001;
              x_ = input.readInt32();
              break;
            }
            case 16: {
              bitField0_ |= 0x00000002;
              y_ = input.readInt32();
              break;
            }
          }
        }
      }
      
      private int bitField0_;
      
      // required int32 x = 1;
      private int x_ ;
      public boolean hasX() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      public int getX() {
        return x_;
      }
      public Builder setX(int value) {
        bitField0_ |= 0x00000001;
        x_ = value;
        
        return this;
      }
      public Builder clearX() {
        bitField0_ = (bitField0_ & ~0x00000001);
        x_ = 0;
        
        return this;
      }
      
      // required int32 y = 2;
      private int y_ ;
      public boolean hasY() {
        return ((bitField0_ & 0x00000002) == 0x00000002);
      }
      public int getY() {
        return y_;
      }
      public Builder setY(int value) {
        bitField0_ |= 0x00000002;
        y_ = value;
        
        return this;
      }
      public Builder clearY() {
        bitField0_ = (bitField0_ & ~0x00000002);
        y_ = 0;
        
        return this;
      }
      
      // @@protoc_insertion_point(builder_scope:protobuf.BallModel)
    }
    
    static {
      defaultInstance = new BallModel(true);
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:protobuf.BallModel)
  }
  
  
  static {
  }
  
  // @@protoc_insertion_point(outer_class_scope)
}
