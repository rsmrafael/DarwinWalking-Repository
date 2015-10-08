// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Histogramm.proto

package de.ostfalia.robocup.message;

public final class MsgHistogramm {
  private MsgHistogramm() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }
  public interface HistogrammOrBuilder
      extends com.google.protobuf.MessageLiteOrBuilder {
    
    // repeated .protobuf.Histogramm.Data hist = 1;
    java.util.List<de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data> 
        getHistList();
    de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data getHist(int index);
    int getHistCount();
  }
  public static final class Histogramm extends
      com.google.protobuf.GeneratedMessageLite
      implements HistogrammOrBuilder {
    // Use Histogramm.newBuilder() to construct.
    private Histogramm(Builder builder) {
      super(builder);
    }
    private Histogramm(boolean noInit) {}
    
    private static final Histogramm defaultInstance;
    public static Histogramm getDefaultInstance() {
      return defaultInstance;
    }
    
    public Histogramm getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    public interface DataOrBuilder
        extends com.google.protobuf.MessageLiteOrBuilder {
      
      // required double key = 1;
      boolean hasKey();
      double getKey();
      
      // required int32 value = 2;
      boolean hasValue();
      int getValue();
    }
    public static final class Data extends
        com.google.protobuf.GeneratedMessageLite
        implements DataOrBuilder {
      // Use Data.newBuilder() to construct.
      private Data(Builder builder) {
        super(builder);
      }
      private Data(boolean noInit) {}
      
      private static final Data defaultInstance;
      public static Data getDefaultInstance() {
        return defaultInstance;
      }
      
      public Data getDefaultInstanceForType() {
        return defaultInstance;
      }
      
      private int bitField0_;
      // required double key = 1;
      public static final int KEY_FIELD_NUMBER = 1;
      private double key_;
      public boolean hasKey() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      public double getKey() {
        return key_;
      }
      
      // required int32 value = 2;
      public static final int VALUE_FIELD_NUMBER = 2;
      private int value_;
      public boolean hasValue() {
        return ((bitField0_ & 0x00000002) == 0x00000002);
      }
      public int getValue() {
        return value_;
      }
      
      private void initFields() {
        key_ = 0D;
        value_ = 0;
      }
      private byte memoizedIsInitialized = -1;
      public final boolean isInitialized() {
        byte isInitialized = memoizedIsInitialized;
        if (isInitialized != -1) return isInitialized == 1;
        
        if (!hasKey()) {
          memoizedIsInitialized = 0;
          return false;
        }
        if (!hasValue()) {
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
          output.writeDouble(1, key_);
        }
        if (((bitField0_ & 0x00000002) == 0x00000002)) {
          output.writeInt32(2, value_);
        }
      }
      
      private int memoizedSerializedSize = -1;
      public int getSerializedSize() {
        int size = memoizedSerializedSize;
        if (size != -1) return size;
      
        size = 0;
        if (((bitField0_ & 0x00000001) == 0x00000001)) {
          size += com.google.protobuf.CodedOutputStream
            .computeDoubleSize(1, key_);
        }
        if (((bitField0_ & 0x00000002) == 0x00000002)) {
          size += com.google.protobuf.CodedOutputStream
            .computeInt32Size(2, value_);
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
      
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseFrom(
          com.google.protobuf.ByteString data)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return newBuilder().mergeFrom(data).buildParsed();
      }
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseFrom(
          com.google.protobuf.ByteString data,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return newBuilder().mergeFrom(data, extensionRegistry)
                 .buildParsed();
      }
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseFrom(byte[] data)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return newBuilder().mergeFrom(data).buildParsed();
      }
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseFrom(
          byte[] data,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return newBuilder().mergeFrom(data, extensionRegistry)
                 .buildParsed();
      }
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseFrom(java.io.InputStream input)
          throws java.io.IOException {
        return newBuilder().mergeFrom(input).buildParsed();
      }
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseFrom(
          java.io.InputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        return newBuilder().mergeFrom(input, extensionRegistry)
                 .buildParsed();
      }
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseDelimitedFrom(java.io.InputStream input)
          throws java.io.IOException {
        Builder builder = newBuilder();
        if (builder.mergeDelimitedFrom(input)) {
          return builder.buildParsed();
        } else {
          return null;
        }
      }
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseDelimitedFrom(
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
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseFrom(
          com.google.protobuf.CodedInputStream input)
          throws java.io.IOException {
        return newBuilder().mergeFrom(input).buildParsed();
      }
      public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data parseFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        return newBuilder().mergeFrom(input, extensionRegistry)
                 .buildParsed();
      }
      
      public static Builder newBuilder() { return Builder.create(); }
      public Builder newBuilderForType() { return newBuilder(); }
      public static Builder newBuilder(de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data prototype) {
        return newBuilder().mergeFrom(prototype);
      }
      public Builder toBuilder() { return newBuilder(this); }
      
      public static final class Builder extends
          com.google.protobuf.GeneratedMessageLite.Builder<
            de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data, Builder>
          implements de.ostfalia.robocup.message.MsgHistogramm.Histogramm.DataOrBuilder {
        // Construct using de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data.newBuilder()
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
          key_ = 0D;
          bitField0_ = (bitField0_ & ~0x00000001);
          value_ = 0;
          bitField0_ = (bitField0_ & ~0x00000002);
          return this;
        }
        
        public Builder clone() {
          return create().mergeFrom(buildPartial());
        }
        
        public de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data getDefaultInstanceForType() {
          return de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data.getDefaultInstance();
        }
        
        public de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data build() {
          de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data result = buildPartial();
          if (!result.isInitialized()) {
            throw newUninitializedMessageException(result);
          }
          return result;
        }
        
        private de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data buildParsed()
            throws com.google.protobuf.InvalidProtocolBufferException {
          de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data result = buildPartial();
          if (!result.isInitialized()) {
            throw newUninitializedMessageException(
              result).asInvalidProtocolBufferException();
          }
          return result;
        }
        
        public de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data buildPartial() {
          de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data result = new de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data(this);
          int from_bitField0_ = bitField0_;
          int to_bitField0_ = 0;
          if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
            to_bitField0_ |= 0x00000001;
          }
          result.key_ = key_;
          if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
            to_bitField0_ |= 0x00000002;
          }
          result.value_ = value_;
          result.bitField0_ = to_bitField0_;
          return result;
        }
        
        public Builder mergeFrom(de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data other) {
          if (other == de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data.getDefaultInstance()) return this;
          if (other.hasKey()) {
            setKey(other.getKey());
          }
          if (other.hasValue()) {
            setValue(other.getValue());
          }
          return this;
        }
        
        public final boolean isInitialized() {
          if (!hasKey()) {
            
            return false;
          }
          if (!hasValue()) {
            
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
              case 9: {
                bitField0_ |= 0x00000001;
                key_ = input.readDouble();
                break;
              }
              case 16: {
                bitField0_ |= 0x00000002;
                value_ = input.readInt32();
                break;
              }
            }
          }
        }
        
        private int bitField0_;
        
        // required double key = 1;
        private double key_ ;
        public boolean hasKey() {
          return ((bitField0_ & 0x00000001) == 0x00000001);
        }
        public double getKey() {
          return key_;
        }
        public Builder setKey(double value) {
          bitField0_ |= 0x00000001;
          key_ = value;
          
          return this;
        }
        public Builder clearKey() {
          bitField0_ = (bitField0_ & ~0x00000001);
          key_ = 0D;
          
          return this;
        }
        
        // required int32 value = 2;
        private int value_ ;
        public boolean hasValue() {
          return ((bitField0_ & 0x00000002) == 0x00000002);
        }
        public int getValue() {
          return value_;
        }
        public Builder setValue(int value) {
          bitField0_ |= 0x00000002;
          value_ = value;
          
          return this;
        }
        public Builder clearValue() {
          bitField0_ = (bitField0_ & ~0x00000002);
          value_ = 0;
          
          return this;
        }
        
        // @@protoc_insertion_point(builder_scope:protobuf.Histogramm.Data)
      }
      
      static {
        defaultInstance = new Data(true);
        defaultInstance.initFields();
      }
      
      // @@protoc_insertion_point(class_scope:protobuf.Histogramm.Data)
    }
    
    // repeated .protobuf.Histogramm.Data hist = 1;
    public static final int HIST_FIELD_NUMBER = 1;
    private java.util.List<de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data> hist_;
    public java.util.List<de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data> getHistList() {
      return hist_;
    }
    public java.util.List<? extends de.ostfalia.robocup.message.MsgHistogramm.Histogramm.DataOrBuilder> 
        getHistOrBuilderList() {
      return hist_;
    }
    public int getHistCount() {
      return hist_.size();
    }
    public de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data getHist(int index) {
      return hist_.get(index);
    }
    public de.ostfalia.robocup.message.MsgHistogramm.Histogramm.DataOrBuilder getHistOrBuilder(
        int index) {
      return hist_.get(index);
    }
    
    private void initFields() {
      hist_ = java.util.Collections.emptyList();
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;
      
      for (int i = 0; i < getHistCount(); i++) {
        if (!getHist(i).isInitialized()) {
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
      for (int i = 0; i < hist_.size(); i++) {
        output.writeMessage(1, hist_.get(i));
      }
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      for (int i = 0; i < hist_.size(); i++) {
        size += com.google.protobuf.CodedOutputStream
          .computeMessageSize(1, hist_.get(i));
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
    
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseDelimitedFrom(
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
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static de.ostfalia.robocup.message.MsgHistogramm.Histogramm parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(de.ostfalia.robocup.message.MsgHistogramm.Histogramm prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          de.ostfalia.robocup.message.MsgHistogramm.Histogramm, Builder>
        implements de.ostfalia.robocup.message.MsgHistogramm.HistogrammOrBuilder {
      // Construct using de.ostfalia.robocup.message.MsgHistogramm.Histogramm.newBuilder()
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
        hist_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }
      
      public de.ostfalia.robocup.message.MsgHistogramm.Histogramm getDefaultInstanceForType() {
        return de.ostfalia.robocup.message.MsgHistogramm.Histogramm.getDefaultInstance();
      }
      
      public de.ostfalia.robocup.message.MsgHistogramm.Histogramm build() {
        de.ostfalia.robocup.message.MsgHistogramm.Histogramm result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }
      
      private de.ostfalia.robocup.message.MsgHistogramm.Histogramm buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        de.ostfalia.robocup.message.MsgHistogramm.Histogramm result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return result;
      }
      
      public de.ostfalia.robocup.message.MsgHistogramm.Histogramm buildPartial() {
        de.ostfalia.robocup.message.MsgHistogramm.Histogramm result = new de.ostfalia.robocup.message.MsgHistogramm.Histogramm(this);
        if (((bitField0_ & 0x00000001) == 0x00000001)) {
          hist_ = java.util.Collections.unmodifiableList(hist_);
          bitField0_ = (bitField0_ & ~0x00000001);
        }
        result.hist_ = hist_;
        return result;
      }
      
      public Builder mergeFrom(de.ostfalia.robocup.message.MsgHistogramm.Histogramm other) {
        if (other == de.ostfalia.robocup.message.MsgHistogramm.Histogramm.getDefaultInstance()) return this;
        if (!other.hist_.isEmpty()) {
          if (hist_.isEmpty()) {
            hist_ = other.hist_;
            bitField0_ = (bitField0_ & ~0x00000001);
          } else {
            ensureHistIsMutable();
            hist_.addAll(other.hist_);
          }
          
        }
        return this;
      }
      
      public final boolean isInitialized() {
        for (int i = 0; i < getHistCount(); i++) {
          if (!getHist(i).isInitialized()) {
            
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
              de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data.Builder subBuilder = de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data.newBuilder();
              input.readMessage(subBuilder, extensionRegistry);
              addHist(subBuilder.buildPartial());
              break;
            }
          }
        }
      }
      
      private int bitField0_;
      
      // repeated .protobuf.Histogramm.Data hist = 1;
      private java.util.List<de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data> hist_ =
        java.util.Collections.emptyList();
      private void ensureHistIsMutable() {
        if (!((bitField0_ & 0x00000001) == 0x00000001)) {
          hist_ = new java.util.ArrayList<de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data>(hist_);
          bitField0_ |= 0x00000001;
         }
      }
      
      public java.util.List<de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data> getHistList() {
        return java.util.Collections.unmodifiableList(hist_);
      }
      public int getHistCount() {
        return hist_.size();
      }
      public de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data getHist(int index) {
        return hist_.get(index);
      }
      public Builder setHist(
          int index, de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data value) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureHistIsMutable();
        hist_.set(index, value);
        
        return this;
      }
      public Builder setHist(
          int index, de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data.Builder builderForValue) {
        ensureHistIsMutable();
        hist_.set(index, builderForValue.build());
        
        return this;
      }
      public Builder addHist(de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data value) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureHistIsMutable();
        hist_.add(value);
        
        return this;
      }
      public Builder addHist(
          int index, de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data value) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureHistIsMutable();
        hist_.add(index, value);
        
        return this;
      }
      public Builder addHist(
          de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data.Builder builderForValue) {
        ensureHistIsMutable();
        hist_.add(builderForValue.build());
        
        return this;
      }
      public Builder addHist(
          int index, de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data.Builder builderForValue) {
        ensureHistIsMutable();
        hist_.add(index, builderForValue.build());
        
        return this;
      }
      public Builder addAllHist(
          java.lang.Iterable<? extends de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data> values) {
        ensureHistIsMutable();
        super.addAll(values, hist_);
        
        return this;
      }
      public Builder clearHist() {
        hist_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
        
        return this;
      }
      public Builder removeHist(int index) {
        ensureHistIsMutable();
        hist_.remove(index);
        
        return this;
      }
      
      // @@protoc_insertion_point(builder_scope:protobuf.Histogramm)
    }
    
    static {
      defaultInstance = new Histogramm(true);
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:protobuf.Histogramm)
  }
  
  
  static {
  }
  
  // @@protoc_insertion_point(outer_class_scope)
}
