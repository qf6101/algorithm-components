// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: attrpb/attribute_data.proto

#ifndef PROTOBUF_attrpb_2fattribute_5fdata_2eproto__INCLUDED
#define PROTOBUF_attrpb_2fattribute_5fdata_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "attrpb/setcate_value.pb.h"
// @@protoc_insertion_point(includes)

namespace protobuf_attrpb_2fattribute_5fdata_2eproto {
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
void InitDefaultsAttributeDataImpl();
void InitDefaultsAttributeData();
inline void InitDefaults() {
  InitDefaultsAttributeData();
}
}  // namespace protobuf_attrpb_2fattribute_5fdata_2eproto
namespace algocomp {
class AttributeData;
class AttributeDataDefaultTypeInternal;
extern AttributeDataDefaultTypeInternal _AttributeData_default_instance_;
}  // namespace algocomp
namespace algocomp {

enum AttributeData_AttributeType {
  AttributeData_AttributeType_UNKNOWN = 0,
  AttributeData_AttributeType_CATEGORICAL = 1,
  AttributeData_AttributeType_SET_CATEGORICAL = 2,
  AttributeData_AttributeType_REAL_VALUED = 3,
  AttributeData_AttributeType_UNIX_TIMESTAMP = 4,
  AttributeData_AttributeType_AttributeData_AttributeType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  AttributeData_AttributeType_AttributeData_AttributeType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool AttributeData_AttributeType_IsValid(int value);
const AttributeData_AttributeType AttributeData_AttributeType_AttributeType_MIN = AttributeData_AttributeType_UNKNOWN;
const AttributeData_AttributeType AttributeData_AttributeType_AttributeType_MAX = AttributeData_AttributeType_UNIX_TIMESTAMP;
const int AttributeData_AttributeType_AttributeType_ARRAYSIZE = AttributeData_AttributeType_AttributeType_MAX + 1;

const ::google::protobuf::EnumDescriptor* AttributeData_AttributeType_descriptor();
inline const ::std::string& AttributeData_AttributeType_Name(AttributeData_AttributeType value) {
  return ::google::protobuf::internal::NameOfEnum(
    AttributeData_AttributeType_descriptor(), value);
}
inline bool AttributeData_AttributeType_Parse(
    const ::std::string& name, AttributeData_AttributeType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<AttributeData_AttributeType>(
    AttributeData_AttributeType_descriptor(), name, value);
}
// ===================================================================

class AttributeData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:algocomp.AttributeData) */ {
 public:
  AttributeData();
  virtual ~AttributeData();

  AttributeData(const AttributeData& from);

  inline AttributeData& operator=(const AttributeData& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  AttributeData(AttributeData&& from) noexcept
    : AttributeData() {
    *this = ::std::move(from);
  }

  inline AttributeData& operator=(AttributeData&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline ::google::protobuf::Arena* GetArena() const PROTOBUF_FINAL {
    return GetArenaNoVirtual();
  }
  inline void* GetMaybeArenaPointer() const PROTOBUF_FINAL {
    return MaybeArenaPtr();
  }
  static const ::google::protobuf::Descriptor* descriptor();
  static const AttributeData& default_instance();

  enum OneofAttrValCase {
    kStrVal = 2,
    kIntVal = 3,
    kLongVal = 4,
    kFloatVal = 5,
    kScateVal = 6,
    ONEOF_ATTR_VAL_NOT_SET = 0,
  };

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AttributeData* internal_default_instance() {
    return reinterpret_cast<const AttributeData*>(
               &_AttributeData_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void UnsafeArenaSwap(AttributeData* other);
  void Swap(AttributeData* other);
  friend void swap(AttributeData& a, AttributeData& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline AttributeData* New() const PROTOBUF_FINAL { return New(NULL); }

  AttributeData* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const AttributeData& from);
  void MergeFrom(const AttributeData& from);
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
  void InternalSwap(AttributeData* other);
  protected:
  explicit AttributeData(::google::protobuf::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::google::protobuf::Arena* arena);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  typedef AttributeData_AttributeType AttributeType;
  static const AttributeType UNKNOWN =
    AttributeData_AttributeType_UNKNOWN;
  static const AttributeType CATEGORICAL =
    AttributeData_AttributeType_CATEGORICAL;
  static const AttributeType SET_CATEGORICAL =
    AttributeData_AttributeType_SET_CATEGORICAL;
  static const AttributeType REAL_VALUED =
    AttributeData_AttributeType_REAL_VALUED;
  static const AttributeType UNIX_TIMESTAMP =
    AttributeData_AttributeType_UNIX_TIMESTAMP;
  static inline bool AttributeType_IsValid(int value) {
    return AttributeData_AttributeType_IsValid(value);
  }
  static const AttributeType AttributeType_MIN =
    AttributeData_AttributeType_AttributeType_MIN;
  static const AttributeType AttributeType_MAX =
    AttributeData_AttributeType_AttributeType_MAX;
  static const int AttributeType_ARRAYSIZE =
    AttributeData_AttributeType_AttributeType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  AttributeType_descriptor() {
    return AttributeData_AttributeType_descriptor();
  }
  static inline const ::std::string& AttributeType_Name(AttributeType value) {
    return AttributeData_AttributeType_Name(value);
  }
  static inline bool AttributeType_Parse(const ::std::string& name,
      AttributeType* value) {
    return AttributeData_AttributeType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // .algocomp.AttributeData.AttributeType attr_type = 1;
  void clear_attr_type();
  static const int kAttrTypeFieldNumber = 1;
  ::algocomp::AttributeData_AttributeType attr_type() const;
  void set_attr_type(::algocomp::AttributeData_AttributeType value);

  // string str_val = 2;
  private:
  bool has_str_val() const;
  public:
  void clear_str_val();
  static const int kStrValFieldNumber = 2;
  const ::std::string& str_val() const;
  void set_str_val(const ::std::string& value);
  #if LANG_CXX11
  void set_str_val(::std::string&& value);
  #endif
  void set_str_val(const char* value);
  void set_str_val(const char* value, size_t size);
  ::std::string* mutable_str_val();
  ::std::string* release_str_val();
  void set_allocated_str_val(::std::string* str_val);
  PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  ::std::string* unsafe_arena_release_str_val();
  PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_str_val(
      ::std::string* str_val);

  // int32 int_val = 3;
  private:
  bool has_int_val() const;
  public:
  void clear_int_val();
  static const int kIntValFieldNumber = 3;
  ::google::protobuf::int32 int_val() const;
  void set_int_val(::google::protobuf::int32 value);

  // int64 long_val = 4;
  private:
  bool has_long_val() const;
  public:
  void clear_long_val();
  static const int kLongValFieldNumber = 4;
  ::google::protobuf::int64 long_val() const;
  void set_long_val(::google::protobuf::int64 value);

  // float float_val = 5;
  private:
  bool has_float_val() const;
  public:
  void clear_float_val();
  static const int kFloatValFieldNumber = 5;
  float float_val() const;
  void set_float_val(float value);

  // .algocomp.SetCateValue scate_val = 6;
  bool has_scate_val() const;
  void clear_scate_val();
  static const int kScateValFieldNumber = 6;
  private:
  void _slow_mutable_scate_val();
  public:
  const ::algocomp::SetCateValue& scate_val() const;
  ::algocomp::SetCateValue* release_scate_val();
  ::algocomp::SetCateValue* mutable_scate_val();
  void set_allocated_scate_val(::algocomp::SetCateValue* scate_val);
  void unsafe_arena_set_allocated_scate_val(
      ::algocomp::SetCateValue* scate_val);
  ::algocomp::SetCateValue* unsafe_arena_release_scate_val();

  OneofAttrValCase oneof_attr_val_case() const;
  // @@protoc_insertion_point(class_scope:algocomp.AttributeData)
 private:
  void set_has_str_val();
  void set_has_int_val();
  void set_has_long_val();
  void set_has_float_val();
  void set_has_scate_val();

  inline bool has_oneof_attr_val() const;
  void clear_oneof_attr_val();
  inline void clear_has_oneof_attr_val();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  template <typename T> friend class ::google::protobuf::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int attr_type_;
  union OneofAttrValUnion {
    OneofAttrValUnion() {}
    ::google::protobuf::internal::ArenaStringPtr str_val_;
    ::google::protobuf::int32 int_val_;
    ::google::protobuf::int64 long_val_;
    float float_val_;
    ::algocomp::SetCateValue* scate_val_;
  } oneof_attr_val_;
  mutable int _cached_size_;
  ::google::protobuf::uint32 _oneof_case_[1];

  friend struct ::protobuf_attrpb_2fattribute_5fdata_2eproto::TableStruct;
  friend void ::protobuf_attrpb_2fattribute_5fdata_2eproto::InitDefaultsAttributeDataImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AttributeData

// .algocomp.AttributeData.AttributeType attr_type = 1;
inline void AttributeData::clear_attr_type() {
  attr_type_ = 0;
}
inline ::algocomp::AttributeData_AttributeType AttributeData::attr_type() const {
  // @@protoc_insertion_point(field_get:algocomp.AttributeData.attr_type)
  return static_cast< ::algocomp::AttributeData_AttributeType >(attr_type_);
}
inline void AttributeData::set_attr_type(::algocomp::AttributeData_AttributeType value) {
  
  attr_type_ = value;
  // @@protoc_insertion_point(field_set:algocomp.AttributeData.attr_type)
}

// string str_val = 2;
inline bool AttributeData::has_str_val() const {
  return oneof_attr_val_case() == kStrVal;
}
inline void AttributeData::set_has_str_val() {
  _oneof_case_[0] = kStrVal;
}
inline void AttributeData::clear_str_val() {
  if (has_str_val()) {
    oneof_attr_val_.str_val_.Destroy(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
        GetArenaNoVirtual());
    clear_has_oneof_attr_val();
  }
}
inline const ::std::string& AttributeData::str_val() const {
  // @@protoc_insertion_point(field_get:algocomp.AttributeData.str_val)
  if (has_str_val()) {
    return oneof_attr_val_.str_val_.Get();
  }
  return *&::google::protobuf::internal::GetEmptyStringAlreadyInited();
}
inline void AttributeData::set_str_val(const ::std::string& value) {
  if (!has_str_val()) {
    clear_oneof_attr_val();
    set_has_str_val();
    oneof_attr_val_.str_val_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  oneof_attr_val_.str_val_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value,
      GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set:algocomp.AttributeData.str_val)
}
#if LANG_CXX11
inline void AttributeData::set_str_val(::std::string&& value) {
  // @@protoc_insertion_point(field_set:algocomp.AttributeData.str_val)
  if (!has_str_val()) {
    clear_oneof_attr_val();
    set_has_str_val();
    oneof_attr_val_.str_val_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  oneof_attr_val_.str_val_.Set(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_rvalue:algocomp.AttributeData.str_val)
}
#endif
inline void AttributeData::set_str_val(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  if (!has_str_val()) {
    clear_oneof_attr_val();
    set_has_str_val();
    oneof_attr_val_.str_val_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  oneof_attr_val_.str_val_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(value), GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_char:algocomp.AttributeData.str_val)
}
inline void AttributeData::set_str_val(const char* value,
                             size_t size) {
  if (!has_str_val()) {
    clear_oneof_attr_val();
    set_has_str_val();
    oneof_attr_val_.str_val_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  oneof_attr_val_.str_val_.Set(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size),
      GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_pointer:algocomp.AttributeData.str_val)
}
inline ::std::string* AttributeData::mutable_str_val() {
  if (!has_str_val()) {
    clear_oneof_attr_val();
    set_has_str_val();
    oneof_attr_val_.str_val_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  return oneof_attr_val_.str_val_.Mutable(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      GetArenaNoVirtual());
  // @@protoc_insertion_point(field_mutable:algocomp.AttributeData.str_val)
}
inline ::std::string* AttributeData::release_str_val() {
  // @@protoc_insertion_point(field_release:algocomp.AttributeData.str_val)
  if (has_str_val()) {
    clear_has_oneof_attr_val();
    return oneof_attr_val_.str_val_.Release(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
        GetArenaNoVirtual());
  } else {
    return NULL;
  }
}
inline void AttributeData::set_allocated_str_val(::std::string* str_val) {
  if (!has_str_val()) {
    oneof_attr_val_.str_val_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  clear_oneof_attr_val();
  if (str_val != NULL) {
    set_has_str_val();
    oneof_attr_val_.str_val_.SetAllocated(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), str_val,
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_set_allocated:algocomp.AttributeData.str_val)
}
inline ::std::string* AttributeData::unsafe_arena_release_str_val() {
  // @@protoc_insertion_point(field_unsafe_arena_release:algocomp.AttributeData.str_val)
  GOOGLE_DCHECK(GetArenaNoVirtual() != NULL);
  if (has_str_val()) {
    clear_has_oneof_attr_val();
    return oneof_attr_val_.str_val_.UnsafeArenaRelease(
        &::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
  } else {
    return NULL;
  }
}
inline void AttributeData::unsafe_arena_set_allocated_str_val(::std::string* str_val) {
  GOOGLE_DCHECK(GetArenaNoVirtual() != NULL);
  if (!has_str_val()) {
    oneof_attr_val_.str_val_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  clear_oneof_attr_val();
  if (str_val) {
    set_has_str_val();
    oneof_attr_val_.str_val_.UnsafeArenaSetAllocated(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), str_val, GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:algocomp.AttributeData.str_val)
}

// int32 int_val = 3;
inline bool AttributeData::has_int_val() const {
  return oneof_attr_val_case() == kIntVal;
}
inline void AttributeData::set_has_int_val() {
  _oneof_case_[0] = kIntVal;
}
inline void AttributeData::clear_int_val() {
  if (has_int_val()) {
    oneof_attr_val_.int_val_ = 0;
    clear_has_oneof_attr_val();
  }
}
inline ::google::protobuf::int32 AttributeData::int_val() const {
  // @@protoc_insertion_point(field_get:algocomp.AttributeData.int_val)
  if (has_int_val()) {
    return oneof_attr_val_.int_val_;
  }
  return 0;
}
inline void AttributeData::set_int_val(::google::protobuf::int32 value) {
  if (!has_int_val()) {
    clear_oneof_attr_val();
    set_has_int_val();
  }
  oneof_attr_val_.int_val_ = value;
  // @@protoc_insertion_point(field_set:algocomp.AttributeData.int_val)
}

// int64 long_val = 4;
inline bool AttributeData::has_long_val() const {
  return oneof_attr_val_case() == kLongVal;
}
inline void AttributeData::set_has_long_val() {
  _oneof_case_[0] = kLongVal;
}
inline void AttributeData::clear_long_val() {
  if (has_long_val()) {
    oneof_attr_val_.long_val_ = GOOGLE_LONGLONG(0);
    clear_has_oneof_attr_val();
  }
}
inline ::google::protobuf::int64 AttributeData::long_val() const {
  // @@protoc_insertion_point(field_get:algocomp.AttributeData.long_val)
  if (has_long_val()) {
    return oneof_attr_val_.long_val_;
  }
  return GOOGLE_LONGLONG(0);
}
inline void AttributeData::set_long_val(::google::protobuf::int64 value) {
  if (!has_long_val()) {
    clear_oneof_attr_val();
    set_has_long_val();
  }
  oneof_attr_val_.long_val_ = value;
  // @@protoc_insertion_point(field_set:algocomp.AttributeData.long_val)
}

// float float_val = 5;
inline bool AttributeData::has_float_val() const {
  return oneof_attr_val_case() == kFloatVal;
}
inline void AttributeData::set_has_float_val() {
  _oneof_case_[0] = kFloatVal;
}
inline void AttributeData::clear_float_val() {
  if (has_float_val()) {
    oneof_attr_val_.float_val_ = 0;
    clear_has_oneof_attr_val();
  }
}
inline float AttributeData::float_val() const {
  // @@protoc_insertion_point(field_get:algocomp.AttributeData.float_val)
  if (has_float_val()) {
    return oneof_attr_val_.float_val_;
  }
  return 0;
}
inline void AttributeData::set_float_val(float value) {
  if (!has_float_val()) {
    clear_oneof_attr_val();
    set_has_float_val();
  }
  oneof_attr_val_.float_val_ = value;
  // @@protoc_insertion_point(field_set:algocomp.AttributeData.float_val)
}

// .algocomp.SetCateValue scate_val = 6;
inline bool AttributeData::has_scate_val() const {
  return oneof_attr_val_case() == kScateVal;
}
inline void AttributeData::set_has_scate_val() {
  _oneof_case_[0] = kScateVal;
}
inline ::algocomp::SetCateValue* AttributeData::release_scate_val() {
  // @@protoc_insertion_point(field_release:algocomp.AttributeData.scate_val)
  if (has_scate_val()) {
    clear_has_oneof_attr_val();
      ::algocomp::SetCateValue* temp = oneof_attr_val_.scate_val_;
    if (GetArenaNoVirtual() != NULL) {
      temp = ::google::protobuf::internal::DuplicateIfNonNull(temp, NULL);
    }
    oneof_attr_val_.scate_val_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
inline const ::algocomp::SetCateValue& AttributeData::scate_val() const {
  // @@protoc_insertion_point(field_get:algocomp.AttributeData.scate_val)
  return has_scate_val()
      ? *oneof_attr_val_.scate_val_
      : *reinterpret_cast< ::algocomp::SetCateValue*>(&::algocomp::_SetCateValue_default_instance_);
}
inline ::algocomp::SetCateValue* AttributeData::unsafe_arena_release_scate_val() {
  // @@protoc_insertion_point(field_unsafe_arena_release:algocomp.AttributeData.scate_val)
  if (has_scate_val()) {
    clear_has_oneof_attr_val();
    ::algocomp::SetCateValue* temp = oneof_attr_val_.scate_val_;
    oneof_attr_val_.scate_val_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
inline void AttributeData::unsafe_arena_set_allocated_scate_val(::algocomp::SetCateValue* scate_val) {
  clear_oneof_attr_val();
  if (scate_val) {
    set_has_scate_val();
    oneof_attr_val_.scate_val_ = scate_val;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:algocomp.AttributeData.scate_val)
}
inline ::algocomp::SetCateValue* AttributeData::mutable_scate_val() {
  if (!has_scate_val()) {
    clear_oneof_attr_val();
    set_has_scate_val();
    oneof_attr_val_.scate_val_ = 
      ::google::protobuf::Arena::CreateMessage< ::algocomp::SetCateValue >(
      GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:algocomp.AttributeData.scate_val)
  return oneof_attr_val_.scate_val_;
}

inline bool AttributeData::has_oneof_attr_val() const {
  return oneof_attr_val_case() != ONEOF_ATTR_VAL_NOT_SET;
}
inline void AttributeData::clear_has_oneof_attr_val() {
  _oneof_case_[0] = ONEOF_ATTR_VAL_NOT_SET;
}
inline AttributeData::OneofAttrValCase AttributeData::oneof_attr_val_case() const {
  return AttributeData::OneofAttrValCase(_oneof_case_[0]);
}
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace algocomp

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::algocomp::AttributeData_AttributeType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::algocomp::AttributeData_AttributeType>() {
  return ::algocomp::AttributeData_AttributeType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_attrpb_2fattribute_5fdata_2eproto__INCLUDED
