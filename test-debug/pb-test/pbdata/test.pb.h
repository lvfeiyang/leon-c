// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test.proto

#ifndef PROTOBUF_INCLUDED_test_2eproto
#define PROTOBUF_INCLUDED_test_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_test_2eproto 

namespace protobuf_test_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_test_2eproto
namespace test {
class TestPb;
class TestPbDefaultTypeInternal;
extern TestPbDefaultTypeInternal _TestPb_default_instance_;
}  // namespace test
namespace google {
namespace protobuf {
template<> ::test::TestPb* Arena::CreateMaybeMessage<::test::TestPb>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace test {

// ===================================================================

class TestPb : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:test.TestPb) */ {
 public:
  TestPb();
  virtual ~TestPb();

  TestPb(const TestPb& from);

  inline TestPb& operator=(const TestPb& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  TestPb(TestPb&& from) noexcept
    : TestPb() {
    *this = ::std::move(from);
  }

  inline TestPb& operator=(TestPb&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const TestPb& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const TestPb* internal_default_instance() {
    return reinterpret_cast<const TestPb*>(
               &_TestPb_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(TestPb* other);
  friend void swap(TestPb& a, TestPb& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline TestPb* New() const final {
    return CreateMaybeMessage<TestPb>(NULL);
  }

  TestPb* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<TestPb>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const TestPb& from);
  void MergeFrom(const TestPb& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(TestPb* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string Sence = 1;
  void clear_sence();
  static const int kSenceFieldNumber = 1;
  const ::std::string& sence() const;
  void set_sence(const ::std::string& value);
  #if LANG_CXX11
  void set_sence(::std::string&& value);
  #endif
  void set_sence(const char* value);
  void set_sence(const char* value, size_t size);
  ::std::string* mutable_sence();
  ::std::string* release_sence();
  void set_allocated_sence(::std::string* sence);

  // string TableName = 9977;
  void clear_tablename();
  static const int kTableNameFieldNumber = 9977;
  const ::std::string& tablename() const;
  void set_tablename(const ::std::string& value);
  #if LANG_CXX11
  void set_tablename(::std::string&& value);
  #endif
  void set_tablename(const char* value);
  void set_tablename(const char* value, size_t size);
  ::std::string* mutable_tablename();
  ::std::string* release_tablename();
  void set_allocated_tablename(::std::string* tablename);

  // int32 Page = 2;
  void clear_page();
  static const int kPageFieldNumber = 2;
  ::google::protobuf::int32 page() const;
  void set_page(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:test.TestPb)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr sence_;
  ::google::protobuf::internal::ArenaStringPtr tablename_;
  ::google::protobuf::int32 page_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_test_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// TestPb

// string TableName = 9977;
inline void TestPb::clear_tablename() {
  tablename_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TestPb::tablename() const {
  // @@protoc_insertion_point(field_get:test.TestPb.TableName)
  return tablename_.GetNoArena();
}
inline void TestPb::set_tablename(const ::std::string& value) {
  
  tablename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:test.TestPb.TableName)
}
#if LANG_CXX11
inline void TestPb::set_tablename(::std::string&& value) {
  
  tablename_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:test.TestPb.TableName)
}
#endif
inline void TestPb::set_tablename(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  tablename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:test.TestPb.TableName)
}
inline void TestPb::set_tablename(const char* value, size_t size) {
  
  tablename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:test.TestPb.TableName)
}
inline ::std::string* TestPb::mutable_tablename() {
  
  // @@protoc_insertion_point(field_mutable:test.TestPb.TableName)
  return tablename_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TestPb::release_tablename() {
  // @@protoc_insertion_point(field_release:test.TestPb.TableName)
  
  return tablename_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TestPb::set_allocated_tablename(::std::string* tablename) {
  if (tablename != NULL) {
    
  } else {
    
  }
  tablename_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), tablename);
  // @@protoc_insertion_point(field_set_allocated:test.TestPb.TableName)
}

// string Sence = 1;
inline void TestPb::clear_sence() {
  sence_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TestPb::sence() const {
  // @@protoc_insertion_point(field_get:test.TestPb.Sence)
  return sence_.GetNoArena();
}
inline void TestPb::set_sence(const ::std::string& value) {
  
  sence_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:test.TestPb.Sence)
}
#if LANG_CXX11
inline void TestPb::set_sence(::std::string&& value) {
  
  sence_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:test.TestPb.Sence)
}
#endif
inline void TestPb::set_sence(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  sence_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:test.TestPb.Sence)
}
inline void TestPb::set_sence(const char* value, size_t size) {
  
  sence_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:test.TestPb.Sence)
}
inline ::std::string* TestPb::mutable_sence() {
  
  // @@protoc_insertion_point(field_mutable:test.TestPb.Sence)
  return sence_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TestPb::release_sence() {
  // @@protoc_insertion_point(field_release:test.TestPb.Sence)
  
  return sence_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TestPb::set_allocated_sence(::std::string* sence) {
  if (sence != NULL) {
    
  } else {
    
  }
  sence_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), sence);
  // @@protoc_insertion_point(field_set_allocated:test.TestPb.Sence)
}

// int32 Page = 2;
inline void TestPb::clear_page() {
  page_ = 0;
}
inline ::google::protobuf::int32 TestPb::page() const {
  // @@protoc_insertion_point(field_get:test.TestPb.Page)
  return page_;
}
inline void TestPb::set_page(::google::protobuf::int32 value) {
  
  page_ = value;
  // @@protoc_insertion_point(field_set:test.TestPb.Page)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace test

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_test_2eproto
