// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msg.proto

#ifndef PROTOBUF_msg_2eproto__INCLUDED
#define PROTOBUF_msg_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3004000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace Protocol {
class GetName;
class GetNameDefaultTypeInternal;
extern GetNameDefaultTypeInternal _GetName_default_instance_;
class Hello;
class HelloDefaultTypeInternal;
extern HelloDefaultTypeInternal _Hello_default_instance_;
}  // namespace Protocol

namespace Protocol {

namespace protobuf_msg_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static void InitDefaultsImpl();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_msg_2eproto

// ===================================================================

class Hello : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Protocol.Hello) */ {
 public:
  Hello();
  virtual ~Hello();

  Hello(const Hello& from);

  inline Hello& operator=(const Hello& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Hello(Hello&& from) noexcept
    : Hello() {
    *this = ::std::move(from);
  }

  inline Hello& operator=(Hello&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Hello& default_instance();

  static inline const Hello* internal_default_instance() {
    return reinterpret_cast<const Hello*>(
               &_Hello_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(Hello* other);
  friend void swap(Hello& a, Hello& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Hello* New() const PROTOBUF_FINAL { return New(NULL); }

  Hello* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Hello& from);
  void MergeFrom(const Hello& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Hello* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string content = 1;
  void clear_content();
  static const int kContentFieldNumber = 1;
  const ::std::string& content() const;
  void set_content(const ::std::string& value);
  #if LANG_CXX11
  void set_content(::std::string&& value);
  #endif
  void set_content(const char* value);
  void set_content(const char* value, size_t size);
  ::std::string* mutable_content();
  ::std::string* release_content();
  void set_allocated_content(::std::string* content);

  // @@protoc_insertion_point(class_scope:Protocol.Hello)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr content_;
  mutable int _cached_size_;
  friend struct protobuf_msg_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class GetName : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Protocol.GetName) */ {
 public:
  GetName();
  virtual ~GetName();

  GetName(const GetName& from);

  inline GetName& operator=(const GetName& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  GetName(GetName&& from) noexcept
    : GetName() {
    *this = ::std::move(from);
  }

  inline GetName& operator=(GetName&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const GetName& default_instance();

  static inline const GetName* internal_default_instance() {
    return reinterpret_cast<const GetName*>(
               &_GetName_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(GetName* other);
  friend void swap(GetName& a, GetName& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline GetName* New() const PROTOBUF_FINAL { return New(NULL); }

  GetName* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const GetName& from);
  void MergeFrom(const GetName& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(GetName* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // int32 userId = 1;
  void clear_userid();
  static const int kUserIdFieldNumber = 1;
  ::google::protobuf::int32 userid() const;
  void set_userid(::google::protobuf::int32 value);

  // int32 sex = 2;
  void clear_sex();
  static const int kSexFieldNumber = 2;
  ::google::protobuf::int32 sex() const;
  void set_sex(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Protocol.GetName)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::int32 userid_;
  ::google::protobuf::int32 sex_;
  mutable int _cached_size_;
  friend struct protobuf_msg_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Hello

// string content = 1;
inline void Hello::clear_content() {
  content_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Hello::content() const {
  // @@protoc_insertion_point(field_get:Protocol.Hello.content)
  return content_.GetNoArena();
}
inline void Hello::set_content(const ::std::string& value) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Protocol.Hello.content)
}
#if LANG_CXX11
inline void Hello::set_content(::std::string&& value) {
  
  content_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Protocol.Hello.content)
}
#endif
inline void Hello::set_content(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Protocol.Hello.content)
}
inline void Hello::set_content(const char* value, size_t size) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Protocol.Hello.content)
}
inline ::std::string* Hello::mutable_content() {
  
  // @@protoc_insertion_point(field_mutable:Protocol.Hello.content)
  return content_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Hello::release_content() {
  // @@protoc_insertion_point(field_release:Protocol.Hello.content)
  
  return content_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Hello::set_allocated_content(::std::string* content) {
  if (content != NULL) {
    
  } else {
    
  }
  content_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), content);
  // @@protoc_insertion_point(field_set_allocated:Protocol.Hello.content)
}

// -------------------------------------------------------------------

// GetName

// int32 userId = 1;
inline void GetName::clear_userid() {
  userid_ = 0;
}
inline ::google::protobuf::int32 GetName::userid() const {
  // @@protoc_insertion_point(field_get:Protocol.GetName.userId)
  return userid_;
}
inline void GetName::set_userid(::google::protobuf::int32 value) {
  
  userid_ = value;
  // @@protoc_insertion_point(field_set:Protocol.GetName.userId)
}

// int32 sex = 2;
inline void GetName::clear_sex() {
  sex_ = 0;
}
inline ::google::protobuf::int32 GetName::sex() const {
  // @@protoc_insertion_point(field_get:Protocol.GetName.sex)
  return sex_;
}
inline void GetName::set_sex(::google::protobuf::int32 value) {
  
  sex_ = value;
  // @@protoc_insertion_point(field_set:Protocol.GetName.sex)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace Protocol

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_msg_2eproto__INCLUDED
