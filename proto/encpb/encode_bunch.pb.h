// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: encpb/encode_bunch.proto

#ifndef PROTOBUF_encpb_2fencode_5fbunch_2eproto__INCLUDED
#define PROTOBUF_encpb_2fencode_5fbunch_2eproto__INCLUDED

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
#include "encpb/encode_data.pb.h"
// @@protoc_insertion_point(includes)

namespace protobuf_encpb_2fencode_5fbunch_2eproto {
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
void InitDefaultsEncodeBunchImpl();
void InitDefaultsEncodeBunch();
inline void InitDefaults() {
  InitDefaultsEncodeBunch();
}
}  // namespace protobuf_encpb_2fencode_5fbunch_2eproto
namespace algocomp {
class EncodeBunch;
class EncodeBunchDefaultTypeInternal;
extern EncodeBunchDefaultTypeInternal _EncodeBunch_default_instance_;
}  // namespace algocomp
namespace algocomp {

// ===================================================================

class EncodeBunch : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:algocomp.EncodeBunch) */ {
 public:
  EncodeBunch();
  virtual ~EncodeBunch();

  EncodeBunch(const EncodeBunch& from);

  inline EncodeBunch& operator=(const EncodeBunch& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  EncodeBunch(EncodeBunch&& from) noexcept
    : EncodeBunch() {
    *this = ::std::move(from);
  }

  inline EncodeBunch& operator=(EncodeBunch&& from) noexcept {
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
  static const EncodeBunch& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const EncodeBunch* internal_default_instance() {
    return reinterpret_cast<const EncodeBunch*>(
               &_EncodeBunch_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void UnsafeArenaSwap(EncodeBunch* other);
  void Swap(EncodeBunch* other);
  friend void swap(EncodeBunch& a, EncodeBunch& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline EncodeBunch* New() const PROTOBUF_FINAL { return New(NULL); }

  EncodeBunch* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const EncodeBunch& from);
  void MergeFrom(const EncodeBunch& from);
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
  void InternalSwap(EncodeBunch* other);
  protected:
  explicit EncodeBunch(::google::protobuf::Arena* arena);
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

  // repeated .algocomp.EncodeData encs = 1;
  int encs_size() const;
  void clear_encs();
  static const int kEncsFieldNumber = 1;
  const ::algocomp::EncodeData& encs(int index) const;
  ::algocomp::EncodeData* mutable_encs(int index);
  ::algocomp::EncodeData* add_encs();
  ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeData >*
      mutable_encs();
  const ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeData >&
      encs() const;

  // @@protoc_insertion_point(class_scope:algocomp.EncodeBunch)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  template <typename T> friend class ::google::protobuf::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeData > encs_;
  mutable int _cached_size_;
  friend struct ::protobuf_encpb_2fencode_5fbunch_2eproto::TableStruct;
  friend void ::protobuf_encpb_2fencode_5fbunch_2eproto::InitDefaultsEncodeBunchImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// EncodeBunch

// repeated .algocomp.EncodeData encs = 1;
inline int EncodeBunch::encs_size() const {
  return encs_.size();
}
inline const ::algocomp::EncodeData& EncodeBunch::encs(int index) const {
  // @@protoc_insertion_point(field_get:algocomp.EncodeBunch.encs)
  return encs_.Get(index);
}
inline ::algocomp::EncodeData* EncodeBunch::mutable_encs(int index) {
  // @@protoc_insertion_point(field_mutable:algocomp.EncodeBunch.encs)
  return encs_.Mutable(index);
}
inline ::algocomp::EncodeData* EncodeBunch::add_encs() {
  // @@protoc_insertion_point(field_add:algocomp.EncodeBunch.encs)
  return encs_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeData >*
EncodeBunch::mutable_encs() {
  // @@protoc_insertion_point(field_mutable_list:algocomp.EncodeBunch.encs)
  return &encs_;
}
inline const ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeData >&
EncodeBunch::encs() const {
  // @@protoc_insertion_point(field_list:algocomp.EncodeBunch.encs)
  return encs_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace algocomp

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_encpb_2fencode_5fbunch_2eproto__INCLUDED