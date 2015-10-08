// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Line.proto

package de.ostfalia.robocup.message;

public final class MsgLine {
  private MsgLine() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }
  public interface LineOrBuilder
      extends com.google.protobuf.MessageLiteOrBuilder {
    
    // required .protobuf.Point startPoint = 1;
    boolean hasStartPoint();
    de.ostfalia.robocup.message.MsgPoint.Point getStartPoint();
    
    // required .protobuf.Point endPoint = 2;
    boolean hasEndPoint();
    de.ostfalia.robocup.message.MsgPoint.Point getEndPoint();
  }
  public static final class Line extends
      com.google.protobuf.GeneratedMessageLite
      implements LineOrBuilder {
    // Use Line.newBuilder() to construct.
    private Line(Builder builder) {
      super(builder);
    }
    private Line(boolean noInit) {}
    
    private static final Line defaultInstance;
    public static Line getDefaultInstance() {
      return defaultInstance;
    }
    
    public Line getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    private int bitField0_;
    // required .protobuf.Point startPoint = 1;
    public static final int STARTPOINT_FIELD_NUMBER = 1;
    private de.ostfalia.robocup.message.MsgPoint.Point startPoint_;
    public boolean hasStartPoint() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    public de.ostfalia.robocup.message.MsgPoint.Point getStartPoint() {
      return startPoint_;
    }
    
    // required .protobuf.Point endPoint = 2;
    public static final int ENDPOINT_FIELD_NUMBER = 2;
    private de.ostfalia.robocup.message.MsgPoint.Point endPoint_;
    public boolean hasEndPoint() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    public de.ostfalia.robocup.message.MsgPoint.Point getEndPoint() {
      return endPoint_;
    }
    
    private void initFields() {
      startPoint_ = de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance();
      endPoint_ = de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance();
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;
      
      if (!hasStartPoint()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!hasEndPoint()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!getStartPoint().isInitialized()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!getEndPoint().isInitialized()) {
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
        output.writeMessage(1, startPoint_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeMessage(2, endPoint_);
      }
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeMessageSize(1, startPoint_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeMessageSize(2, endPoint_);
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
    
    public static de.ostfalia.robocup.message.MsgLine.Line parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgLine.Line parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgLine.Line parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgLine.Line parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgLine.Line parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgLine.Line parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgLine.Line parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static de.ostfalia.robocup.message.MsgLine.Line parseDelimitedFrom(
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
    public static de.ostfalia.robocup.message.MsgLine.Line parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgLine.Line parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(de.ostfalia.robocup.message.MsgLine.Line prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          de.ostfalia.robocup.message.MsgLine.Line, Builder>
        implements de.ostfalia.robocup.message.MsgLine.LineOrBuilder {
      // Construct using de.ostfalia.robocup.message.MsgLine.Line.newBuilder()
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
        startPoint_ = de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance();
        bitField0_ = (bitField0_ & ~0x00000001);
        endPoint_ = de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance();
        bitField0_ = (bitField0_ & ~0x00000002);
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }
      
      public de.ostfalia.robocup.message.MsgLine.Line getDefaultInstanceForType() {
        return de.ostfalia.robocup.message.MsgLine.Line.getDefaultInstance();
      }
      
      public de.ostfalia.robocup.message.MsgLine.Line build() {
        de.ostfalia.robocup.message.MsgLine.Line result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }
      
      private de.ostfalia.robocup.message.MsgLine.Line buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        de.ostfalia.robocup.message.MsgLine.Line result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return result;
      }
      
      public de.ostfalia.robocup.message.MsgLine.Line buildPartial() {
        de.ostfalia.robocup.message.MsgLine.Line result = new de.ostfalia.robocup.message.MsgLine.Line(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.startPoint_ = startPoint_;
        if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
          to_bitField0_ |= 0x00000002;
        }
        result.endPoint_ = endPoint_;
        result.bitField0_ = to_bitField0_;
        return result;
      }
      
      public Builder mergeFrom(de.ostfalia.robocup.message.MsgLine.Line other) {
        if (other == de.ostfalia.robocup.message.MsgLine.Line.getDefaultInstance()) return this;
        if (other.hasStartPoint()) {
          mergeStartPoint(other.getStartPoint());
        }
        if (other.hasEndPoint()) {
          mergeEndPoint(other.getEndPoint());
        }
        return this;
      }
      
      public final boolean isInitialized() {
        if (!hasStartPoint()) {
          
          return false;
        }
        if (!hasEndPoint()) {
          
          return false;
        }
        if (!getStartPoint().isInitialized()) {
          
          return false;
        }
        if (!getEndPoint().isInitialized()) {
          
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
            case 10: {
              de.ostfalia.robocup.message.MsgPoint.Point.Builder subBuilder = de.ostfalia.robocup.message.MsgPoint.Point.newBuilder();
              if (hasStartPoint()) {
                subBuilder.mergeFrom(getStartPoint());
              }
              input.readMessage(subBuilder, extensionRegistry);
              setStartPoint(subBuilder.buildPartial());
              break;
            }
            case 18: {
              de.ostfalia.robocup.message.MsgPoint.Point.Builder subBuilder = de.ostfalia.robocup.message.MsgPoint.Point.newBuilder();
              if (hasEndPoint()) {
                subBuilder.mergeFrom(getEndPoint());
              }
              input.readMessage(subBuilder, extensionRegistry);
              setEndPoint(subBuilder.buildPartial());
              break;
            }
          }
        }
      }
      
      private int bitField0_;
      
      // required .protobuf.Point startPoint = 1;
      private de.ostfalia.robocup.message.MsgPoint.Point startPoint_ = de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance();
      public boolean hasStartPoint() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      public de.ostfalia.robocup.message.MsgPoint.Point getStartPoint() {
        return startPoint_;
      }
      public Builder setStartPoint(de.ostfalia.robocup.message.MsgPoint.Point value) {
        if (value == null) {
          throw new NullPointerException();
        }
        startPoint_ = value;
        
        bitField0_ |= 0x00000001;
        return this;
      }
      public Builder setStartPoint(
          de.ostfalia.robocup.message.MsgPoint.Point.Builder builderForValue) {
        startPoint_ = builderForValue.build();
        
        bitField0_ |= 0x00000001;
        return this;
      }
      public Builder mergeStartPoint(de.ostfalia.robocup.message.MsgPoint.Point value) {
        if (((bitField0_ & 0x00000001) == 0x00000001) &&
            startPoint_ != de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance()) {
          startPoint_ =
            de.ostfalia.robocup.message.MsgPoint.Point.newBuilder(startPoint_).mergeFrom(value).buildPartial();
        } else {
          startPoint_ = value;
        }
        
        bitField0_ |= 0x00000001;
        return this;
      }
      public Builder clearStartPoint() {
        startPoint_ = de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance();
        
        bitField0_ = (bitField0_ & ~0x00000001);
        return this;
      }
      
      // required .protobuf.Point endPoint = 2;
      private de.ostfalia.robocup.message.MsgPoint.Point endPoint_ = de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance();
      public boolean hasEndPoint() {
        return ((bitField0_ & 0x00000002) == 0x00000002);
      }
      public de.ostfalia.robocup.message.MsgPoint.Point getEndPoint() {
        return endPoint_;
      }
      public Builder setEndPoint(de.ostfalia.robocup.message.MsgPoint.Point value) {
        if (value == null) {
          throw new NullPointerException();
        }
        endPoint_ = value;
        
        bitField0_ |= 0x00000002;
        return this;
      }
      public Builder setEndPoint(
          de.ostfalia.robocup.message.MsgPoint.Point.Builder builderForValue) {
        endPoint_ = builderForValue.build();
        
        bitField0_ |= 0x00000002;
        return this;
      }
      public Builder mergeEndPoint(de.ostfalia.robocup.message.MsgPoint.Point value) {
        if (((bitField0_ & 0x00000002) == 0x00000002) &&
            endPoint_ != de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance()) {
          endPoint_ =
            de.ostfalia.robocup.message.MsgPoint.Point.newBuilder(endPoint_).mergeFrom(value).buildPartial();
        } else {
          endPoint_ = value;
        }
        
        bitField0_ |= 0x00000002;
        return this;
      }
      public Builder clearEndPoint() {
        endPoint_ = de.ostfalia.robocup.message.MsgPoint.Point.getDefaultInstance();
        
        bitField0_ = (bitField0_ & ~0x00000002);
        return this;
      }
      
      // @@protoc_insertion_point(builder_scope:protobuf.Line)
    }
    
    static {
      defaultInstance = new Line(true);
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:protobuf.Line)
  }
  
  
  static {
  }
  
  // @@protoc_insertion_point(outer_class_scope)
}
