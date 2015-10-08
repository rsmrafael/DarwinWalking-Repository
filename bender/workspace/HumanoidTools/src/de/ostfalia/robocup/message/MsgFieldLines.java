// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FieldLines.proto

package de.ostfalia.robocup.message;

public final class MsgFieldLines {
  private MsgFieldLines() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }
  public interface FieldLinesOrBuilder
      extends com.google.protobuf.MessageLiteOrBuilder {
    
    // repeated .protobuf.Line line = 1;
    java.util.List<de.ostfalia.robocup.message.MsgLine.Line> 
        getLineList();
    de.ostfalia.robocup.message.MsgLine.Line getLine(int index);
    int getLineCount();
  }
  public static final class FieldLines extends
      com.google.protobuf.GeneratedMessageLite
      implements FieldLinesOrBuilder {
    // Use FieldLines.newBuilder() to construct.
    private FieldLines(Builder builder) {
      super(builder);
    }
    private FieldLines(boolean noInit) {}
    
    private static final FieldLines defaultInstance;
    public static FieldLines getDefaultInstance() {
      return defaultInstance;
    }
    
    public FieldLines getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    // repeated .protobuf.Line line = 1;
    public static final int LINE_FIELD_NUMBER = 1;
    private java.util.List<de.ostfalia.robocup.message.MsgLine.Line> line_;
    public java.util.List<de.ostfalia.robocup.message.MsgLine.Line> getLineList() {
      return line_;
    }
    public java.util.List<? extends de.ostfalia.robocup.message.MsgLine.LineOrBuilder> 
        getLineOrBuilderList() {
      return line_;
    }
    public int getLineCount() {
      return line_.size();
    }
    public de.ostfalia.robocup.message.MsgLine.Line getLine(int index) {
      return line_.get(index);
    }
    public de.ostfalia.robocup.message.MsgLine.LineOrBuilder getLineOrBuilder(
        int index) {
      return line_.get(index);
    }
    
    private void initFields() {
      line_ = java.util.Collections.emptyList();
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;
      
      for (int i = 0; i < getLineCount(); i++) {
        if (!getLine(i).isInitialized()) {
          memoizedIsInitialized = 0;
          return false;
        }
      }
      memoizedIsInitialized = 1;
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      for (int i = 0; i < line_.size(); i++) {
        output.writeMessage(1, line_.get(i));
      }
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      for (int i = 0; i < line_.size(); i++) {
        size += com.google.protobuf.CodedOutputStream
          .computeMessageSize(1, line_.get(i));
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
    
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseDelimitedFrom(
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
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgFieldLines.FieldLines parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(de.ostfalia.robocup.message.MsgFieldLines.FieldLines prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          de.ostfalia.robocup.message.MsgFieldLines.FieldLines, Builder>
        implements de.ostfalia.robocup.message.MsgFieldLines.FieldLinesOrBuilder {
      // Construct using de.ostfalia.robocup.message.MsgFieldLines.FieldLines.newBuilder()
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
        line_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }
      
      public de.ostfalia.robocup.message.MsgFieldLines.FieldLines getDefaultInstanceForType() {
        return de.ostfalia.robocup.message.MsgFieldLines.FieldLines.getDefaultInstance();
      }
      
      public de.ostfalia.robocup.message.MsgFieldLines.FieldLines build() {
        de.ostfalia.robocup.message.MsgFieldLines.FieldLines result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }
      
      private de.ostfalia.robocup.message.MsgFieldLines.FieldLines buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        de.ostfalia.robocup.message.MsgFieldLines.FieldLines result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return result;
      }
      
      public de.ostfalia.robocup.message.MsgFieldLines.FieldLines buildPartial() {
        de.ostfalia.robocup.message.MsgFieldLines.FieldLines result = new de.ostfalia.robocup.message.MsgFieldLines.FieldLines(this);
        if (((bitField0_ & 0x00000001) == 0x00000001)) {
          line_ = java.util.Collections.unmodifiableList(line_);
          bitField0_ = (bitField0_ & ~0x00000001);
        }
        result.line_ = line_;
        return result;
      }
      
      public Builder mergeFrom(de.ostfalia.robocup.message.MsgFieldLines.FieldLines other) {
        if (other == de.ostfalia.robocup.message.MsgFieldLines.FieldLines.getDefaultInstance()) return this;
        if (!other.line_.isEmpty()) {
          if (line_.isEmpty()) {
            line_ = other.line_;
            bitField0_ = (bitField0_ & ~0x00000001);
          } else {
            ensureLineIsMutable();
            line_.addAll(other.line_);
          }
          
        }
        return this;
      }
      
      public final boolean isInitialized() {
        for (int i = 0; i < getLineCount(); i++) {
          if (!getLine(i).isInitialized()) {
            
            return false;
          }
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
              de.ostfalia.robocup.message.MsgLine.Line.Builder subBuilder = de.ostfalia.robocup.message.MsgLine.Line.newBuilder();
              input.readMessage(subBuilder, extensionRegistry);
              addLine(subBuilder.buildPartial());
              break;
            }
          }
        }
      }
      
      private int bitField0_;
      
      // repeated .protobuf.Line line = 1;
      private java.util.List<de.ostfalia.robocup.message.MsgLine.Line> line_ =
        java.util.Collections.emptyList();
      private void ensureLineIsMutable() {
        if (!((bitField0_ & 0x00000001) == 0x00000001)) {
          line_ = new java.util.ArrayList<de.ostfalia.robocup.message.MsgLine.Line>(line_);
          bitField0_ |= 0x00000001;
         }
      }
      
      public java.util.List<de.ostfalia.robocup.message.MsgLine.Line> getLineList() {
        return java.util.Collections.unmodifiableList(line_);
      }
      public int getLineCount() {
        return line_.size();
      }
      public de.ostfalia.robocup.message.MsgLine.Line getLine(int index) {
        return line_.get(index);
      }
      public Builder setLine(
          int index, de.ostfalia.robocup.message.MsgLine.Line value) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureLineIsMutable();
        line_.set(index, value);
        
        return this;
      }
      public Builder setLine(
          int index, de.ostfalia.robocup.message.MsgLine.Line.Builder builderForValue) {
        ensureLineIsMutable();
        line_.set(index, builderForValue.build());
        
        return this;
      }
      public Builder addLine(de.ostfalia.robocup.message.MsgLine.Line value) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureLineIsMutable();
        line_.add(value);
        
        return this;
      }
      public Builder addLine(
          int index, de.ostfalia.robocup.message.MsgLine.Line value) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureLineIsMutable();
        line_.add(index, value);
        
        return this;
      }
      public Builder addLine(
          de.ostfalia.robocup.message.MsgLine.Line.Builder builderForValue) {
        ensureLineIsMutable();
        line_.add(builderForValue.build());
        
        return this;
      }
      public Builder addLine(
          int index, de.ostfalia.robocup.message.MsgLine.Line.Builder builderForValue) {
        ensureLineIsMutable();
        line_.add(index, builderForValue.build());
        
        return this;
      }
      public Builder addAllLine(
          java.lang.Iterable<? extends de.ostfalia.robocup.message.MsgLine.Line> values) {
        ensureLineIsMutable();
        super.addAll(values, line_);
        
        return this;
      }
      public Builder clearLine() {
        line_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
        
        return this;
      }
      public Builder removeLine(int index) {
        ensureLineIsMutable();
        line_.remove(index);
        
        return this;
      }
      
      // @@protoc_insertion_point(builder_scope:protobuf.FieldLines)
    }
    
    static {
      defaultInstance = new FieldLines(true);
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:protobuf.FieldLines)
  }
  
  
  static {
  }
  
  // @@protoc_insertion_point(outer_class_scope)
}
