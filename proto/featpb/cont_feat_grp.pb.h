// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: featpb/cont_feat_grp.proto

#ifndef PROTOBUF_featpb_2fcont_5ffeat_5fgrp_2eproto__INCLUDED
#define PROTOBUF_featpb_2fcont_5ffeat_5fgrp_2eproto__INCLUDED

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
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
#include "featpb/cont_feat_data.pb.h"
// @@protoc_insertion_point(includes)

namespace protobuf_featpb_2fcont_5ffeat_5fgrp_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsContFeatGrp_FeatsEntry_DoNotUseImpl();
void InitDefaultsContFeatGrp_FeatsEntry_DoNotUse();
void InitDefaultsContFeatGrpImpl();
void InitDefaultsContFeatGrp();
inline void InitDefaults() {
  InitDefaultsContFeatGrp_FeatsEntry_DoNotUse();
  InitDefaultsContFeatGrp();
}
}  // namespace protobuf_featpb_2fcont_5ffeat_5fgrp_2eproto
namespace algocomp {
class ContFeatGrp;
class ContFeatGrpDefaultTypeInternal;
extern ContFeatGrpDefaultTypeInternal _ContFeatGrp_default_instance_;
class ContFeatGrp_FeatsEntry_DoNotUse;
class ContFeatGrp_FeatsEntry_DoNotUseDefaultTypeInternal;
extern ContFeatGrp_FeatsEntry_DoNotUseDefaultTypeInternal _ContFeatGrp_FeatsEntry_DoNotUse_default_instance_;
}  // namespace algocomp
namespace algocomp {

// ===================================================================

class ContFeatGrp_FeatsEntry_DoNotUse : public ::google::protobuf::internal::MapEntry<ContFeatGrp_FeatsEntry_DoNotUse, 
    ::std::string, ::algocomp::ContFeatData,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
    0 > {
public:
  typedef ::google::protobuf::internal::MapEntry<ContFeatGrp_FeatsEntry_DoNotUse, 
    ::std::string, ::algocomp::ContFeatData,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
    0 > SuperType;
  ContFeatGrp_FeatsEntry_DoNotUse();
  ContFeatGrp_FeatsEntry_DoNotUse(::google::protobuf::Arena* arena);
  void MergeFrom(const ContFeatGrp_FeatsEntry_DoNotUse& other);
  static const ContFeatGrp_FeatsEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const ContFeatGrp_FeatsEntry_DoNotUse*>(&_ContFeatGrp_FeatsEntry_DoNotUse_default_instance_); }
  void MergeFrom(const ::google::protobuf::Message& other) PROTOBUF_FINAL;
  ::google::protobuf::Metadata GetMetadata() const;
};

// -------------------------------------------------------------------

class ContFeatGrp : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:algocomp.ContFeatGrp) */ {
 public:
  ContFeatGrp();
  virtual ~ContFeatGrp();

  ContFeatGrp(const ContFeatGrp& from);

  inline ContFeatGrp& operator=(const ContFeatGrp& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ContFeatGrp(ContFeatGrp&& from) noexcept
    : ContFeatGrp() {
    *this = ::std::move(from);
  }

  inline ContFeatGrp& operator=(ContFeatGrp&& from) noexcept {
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
  static const ContFeatGrp& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ContFeatGrp* internal_default_instance() {
    return reinterpret_cast<const ContFeatGrp*>(
               &_ContFeatGrp_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void UnsafeArenaSwap(ContFeatGrp* other);
  void Swap(ContFeatGrp* other);
  friend void swap(ContFeatGrp& a, ContFeatGrp& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ContFeatGrp* New() const PROTOBUF_FINAL { return New(NULL); }

  ContFeatGrp* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const ContFeatGrp& from);
  void MergeFrom(const ContFeatGrp& from);
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
  void InternalSwap(ContFeatGrp* other);
  protected:
  explicit ContFeatGrp(::google::protobuf::Arena* arena);
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

  // map<string, .algocomp.ContFeatData> feats = 1;
  int feats_size() const;
  void clear_feats();
  static const int kFeatsFieldNumber = 1;
  const ::google::protobuf::Map< ::std::string, ::algocomp::ContFeatData >&
      feats() const;
  ::google::protobuf::Map< ::std::string, ::algocomp::ContFeatData >*
      mutable_feats();

  // @@protoc_insertion_point(class_scope:algocomp.ContFeatGrp)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  template <typename T> friend class ::google::protobuf::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::google::protobuf::internal::MapField<
      ContFeatGrp_FeatsEntry_DoNotUse,
      ::std::string, ::algocomp::ContFeatData,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
      0 > feats_;
  mutable int _cached_size_;
  friend struct ::protobuf_featpb_2fcont_5ffeat_5fgrp_2eproto::TableStruct;
  friend void ::protobuf_featpb_2fcont_5ffeat_5fgrp_2eproto::InitDefaultsContFeatGrpImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// ContFeatGrp

// map<string, .algocomp.ContFeatData> feats = 1;
inline int ContFeatGrp::feats_size() const {
  return feats_.size();
}
inline const ::google::protobuf::Map< ::std::string, ::algocomp::ContFeatData >&
ContFeatGrp::feats() const {
  // @@protoc_insertion_point(field_map:algocomp.ContFeatGrp.feats)
  return feats_.GetMap();
}
inline ::google::protobuf::Map< ::std::string, ::algocomp::ContFeatData >*
ContFeatGrp::mutable_feats() {
  // @@protoc_insertion_point(field_mutable_map:algocomp.ContFeatGrp.feats)
  return feats_.MutableMap();
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace algocomp

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_featpb_2fcont_5ffeat_5fgrp_2eproto__INCLUDED