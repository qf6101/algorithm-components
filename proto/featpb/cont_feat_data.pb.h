// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: featpb/cont_feat_data.proto

#ifndef PROTOBUF_featpb_2fcont_5ffeat_5fdata_2eproto__INCLUDED
#define PROTOBUF_featpb_2fcont_5ffeat_5fdata_2eproto__INCLUDED

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_featpb_2fcont_5ffeat_5fdata_2eproto {
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
void InitDefaultsContFeatDataImpl();
void InitDefaultsContFeatData();
inline void InitDefaults() {
  InitDefaultsContFeatData();
}
}  // namespace protobuf_featpb_2fcont_5ffeat_5fdata_2eproto
namespace algocomp {
class ContFeatData;
class ContFeatDataDefaultTypeInternal;
extern ContFeatDataDefaultTypeInternal _ContFeatData_default_instance_;
}  // namespace algocomp
namespace algocomp {

// ===================================================================

class ContFeatData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:algocomp.ContFeatData) */ {
 public:
  ContFeatData();
  virtual ~ContFeatData();

  ContFeatData(const ContFeatData& from);

  inline ContFeatData& operator=(const ContFeatData& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ContFeatData(ContFeatData&& from) noexcept
    : ContFeatData() {
    *this = ::std::move(from);
  }

  inline ContFeatData& operator=(ContFeatData&& from) noexcept {
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
  static const ContFeatData& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ContFeatData* internal_default_instance() {
    return reinterpret_cast<const ContFeatData*>(
               &_ContFeatData_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void UnsafeArenaSwap(ContFeatData* other);
  void Swap(ContFeatData* other);
  friend void swap(ContFeatData& a, ContFeatData& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ContFeatData* New() const PROTOBUF_FINAL { return New(NULL); }

  ContFeatData* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const ContFeatData& from);
  void MergeFrom(const ContFeatData& from);
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
  void InternalSwap(ContFeatData* other);
  protected:
  explicit ContFeatData(::google::protobuf::Arena* arena);
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

  // accessors -------------------------------------------------------

  // string gname = 1;
  void clear_gname();
  static const int kGnameFieldNumber = 1;
  const ::std::string& gname() const;
  void set_gname(const ::std::string& value);
  #if LANG_CXX11
  void set_gname(::std::string&& value);
  #endif
  void set_gname(const char* value);
  void set_gname(const char* value, size_t size);
  ::std::string* mutable_gname();
  ::std::string* release_gname();
  void set_allocated_gname(::std::string* gname);
  PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  ::std::string* unsafe_arena_release_gname();
  PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_gname(
      ::std::string* gname);

  // string fname = 2;
  void clear_fname();
  static const int kFnameFieldNumber = 2;
  const ::std::string& fname() const;
  void set_fname(const ::std::string& value);
  #if LANG_CXX11
  void set_fname(::std::string&& value);
  #endif
  void set_fname(const char* value);
  void set_fname(const char* value, size_t size);
  ::std::string* mutable_fname();
  ::std::string* release_fname();
  void set_allocated_fname(::std::string* fname);
  PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  ::std::string* unsafe_arena_release_fname();
  PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_fname(
      ::std::string* fname);

  // float fval = 3;
  void clear_fval();
  static const int kFvalFieldNumber = 3;
  float fval() const;
  void set_fval(float value);

  // @@protoc_insertion_point(class_scope:algocomp.ContFeatData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  template <typename T> friend class ::google::protobuf::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::google::protobuf::internal::ArenaStringPtr gname_;
  ::google::protobuf::internal::ArenaStringPtr fname_;
  float fval_;
  mutable int _cached_size_;
  friend struct ::protobuf_featpb_2fcont_5ffeat_5fdata_2eproto::TableStruct;
  friend void ::protobuf_featpb_2fcont_5ffeat_5fdata_2eproto::InitDefaultsContFeatDataImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ContFeatData

// string gname = 1;
inline void ContFeatData::clear_gname() {
  gname_.ClearToEmpty(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
}
inline const ::std::string& ContFeatData::gname() const {
  // @@protoc_insertion_point(field_get:algocomp.ContFeatData.gname)
  return gname_.Get();
}
inline void ContFeatData::set_gname(const ::std::string& value) {
  
  gname_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value, GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set:algocomp.ContFeatData.gname)
}
#if LANG_CXX11
inline void ContFeatData::set_gname(::std::string&& value) {
  
  gname_.Set(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_rvalue:algocomp.ContFeatData.gname)
}
#endif
inline void ContFeatData::set_gname(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  gname_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_char:algocomp.ContFeatData.gname)
}
inline void ContFeatData::set_gname(const char* value,
    size_t size) {
  
  gname_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_pointer:algocomp.ContFeatData.gname)
}
inline ::std::string* ContFeatData::mutable_gname() {
  
  // @@protoc_insertion_point(field_mutable:algocomp.ContFeatData.gname)
  return gname_.Mutable(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
}
inline ::std::string* ContFeatData::release_gname() {
  // @@protoc_insertion_point(field_release:algocomp.ContFeatData.gname)
  
  return gname_.Release(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
}
inline void ContFeatData::set_allocated_gname(::std::string* gname) {
  if (gname != NULL) {
    
  } else {
    
  }
  gname_.SetAllocated(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), gname,
      GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_allocated:algocomp.ContFeatData.gname)
}
inline ::std::string* ContFeatData::unsafe_arena_release_gname() {
  // @@protoc_insertion_point(field_unsafe_arena_release:algocomp.ContFeatData.gname)
  GOOGLE_DCHECK(GetArenaNoVirtual() != NULL);
  
  return gname_.UnsafeArenaRelease(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      GetArenaNoVirtual());
}
inline void ContFeatData::unsafe_arena_set_allocated_gname(
    ::std::string* gname) {
  GOOGLE_DCHECK(GetArenaNoVirtual() != NULL);
  if (gname != NULL) {
    
  } else {
    
  }
  gname_.UnsafeArenaSetAllocated(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      gname, GetArenaNoVirtual());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:algocomp.ContFeatData.gname)
}

// string fname = 2;
inline void ContFeatData::clear_fname() {
  fname_.ClearToEmpty(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
}
inline const ::std::string& ContFeatData::fname() const {
  // @@protoc_insertion_point(field_get:algocomp.ContFeatData.fname)
  return fname_.Get();
}
inline void ContFeatData::set_fname(const ::std::string& value) {
  
  fname_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value, GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set:algocomp.ContFeatData.fname)
}
#if LANG_CXX11
inline void ContFeatData::set_fname(::std::string&& value) {
  
  fname_.Set(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_rvalue:algocomp.ContFeatData.fname)
}
#endif
inline void ContFeatData::set_fname(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  fname_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_char:algocomp.ContFeatData.fname)
}
inline void ContFeatData::set_fname(const char* value,
    size_t size) {
  
  fname_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_pointer:algocomp.ContFeatData.fname)
}
inline ::std::string* ContFeatData::mutable_fname() {
  
  // @@protoc_insertion_point(field_mutable:algocomp.ContFeatData.fname)
  return fname_.Mutable(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
}
inline ::std::string* ContFeatData::release_fname() {
  // @@protoc_insertion_point(field_release:algocomp.ContFeatData.fname)
  
  return fname_.Release(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
}
inline void ContFeatData::set_allocated_fname(::std::string* fname) {
  if (fname != NULL) {
    
  } else {
    
  }
  fname_.SetAllocated(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), fname,
      GetArenaNoVirtual());
  // @@protoc_insertion_point(field_set_allocated:algocomp.ContFeatData.fname)
}
inline ::std::string* ContFeatData::unsafe_arena_release_fname() {
  // @@protoc_insertion_point(field_unsafe_arena_release:algocomp.ContFeatData.fname)
  GOOGLE_DCHECK(GetArenaNoVirtual() != NULL);
  
  return fname_.UnsafeArenaRelease(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      GetArenaNoVirtual());
}
inline void ContFeatData::unsafe_arena_set_allocated_fname(
    ::std::string* fname) {
  GOOGLE_DCHECK(GetArenaNoVirtual() != NULL);
  if (fname != NULL) {
    
  } else {
    
  }
  fname_.UnsafeArenaSetAllocated(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      fname, GetArenaNoVirtual());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:algocomp.ContFeatData.fname)
}

// float fval = 3;
inline void ContFeatData::clear_fval() {
  fval_ = 0;
}
inline float ContFeatData::fval() const {
  // @@protoc_insertion_point(field_get:algocomp.ContFeatData.fval)
  return fval_;
}
inline void ContFeatData::set_fval(float value) {
  
  fval_ = value;
  // @@protoc_insertion_point(field_set:algocomp.ContFeatData.fval)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace algocomp

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_featpb_2fcont_5ffeat_5fdata_2eproto__INCLUDED