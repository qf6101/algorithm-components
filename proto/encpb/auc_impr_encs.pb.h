// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: encpb/auc_impr_encs.proto

#ifndef PROTOBUF_encpb_2fauc_5fimpr_5fencs_2eproto__INCLUDED
#define PROTOBUF_encpb_2fauc_5fimpr_5fencs_2eproto__INCLUDED

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
#include "encpb/encode_bunch.pb.h"
// @@protoc_insertion_point(includes)

namespace protobuf_encpb_2fauc_5fimpr_5fencs_2eproto {
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
void InitDefaultsAUCImprEncsImpl();
void InitDefaultsAUCImprEncs();
inline void InitDefaults() {
  InitDefaultsAUCImprEncs();
}
}  // namespace protobuf_encpb_2fauc_5fimpr_5fencs_2eproto
namespace algocomp {
class AUCImprEncs;
class AUCImprEncsDefaultTypeInternal;
extern AUCImprEncsDefaultTypeInternal _AUCImprEncs_default_instance_;
}  // namespace algocomp
namespace algocomp {

// ===================================================================

class AUCImprEncs : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:algocomp.AUCImprEncs) */ {
 public:
  AUCImprEncs();
  virtual ~AUCImprEncs();

  AUCImprEncs(const AUCImprEncs& from);

  inline AUCImprEncs& operator=(const AUCImprEncs& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  AUCImprEncs(AUCImprEncs&& from) noexcept
    : AUCImprEncs() {
    *this = ::std::move(from);
  }

  inline AUCImprEncs& operator=(AUCImprEncs&& from) noexcept {
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
  static const AUCImprEncs& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AUCImprEncs* internal_default_instance() {
    return reinterpret_cast<const AUCImprEncs*>(
               &_AUCImprEncs_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void UnsafeArenaSwap(AUCImprEncs* other);
  void Swap(AUCImprEncs* other);
  friend void swap(AUCImprEncs& a, AUCImprEncs& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline AUCImprEncs* New() const PROTOBUF_FINAL { return New(NULL); }

  AUCImprEncs* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const AUCImprEncs& from);
  void MergeFrom(const AUCImprEncs& from);
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
  void InternalSwap(AUCImprEncs* other);
  protected:
  explicit AUCImprEncs(::google::protobuf::Arena* arena);
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

  // repeated float labels = 1;
  int labels_size() const;
  void clear_labels();
  static const int kLabelsFieldNumber = 1;
  float labels(int index) const;
  void set_labels(int index, float value);
  void add_labels(float value);
  const ::google::protobuf::RepeatedField< float >&
      labels() const;
  ::google::protobuf::RepeatedField< float >*
      mutable_labels();

  // repeated float weights = 2;
  int weights_size() const;
  void clear_weights();
  static const int kWeightsFieldNumber = 2;
  float weights(int index) const;
  void set_weights(int index, float value);
  void add_weights(float value);
  const ::google::protobuf::RepeatedField< float >&
      weights() const;
  ::google::protobuf::RepeatedField< float >*
      mutable_weights();

  // repeated float scores = 3;
  int scores_size() const;
  void clear_scores();
  static const int kScoresFieldNumber = 3;
  float scores(int index) const;
  void set_scores(int index, float value);
  void add_scores(float value);
  const ::google::protobuf::RepeatedField< float >&
      scores() const;
  ::google::protobuf::RepeatedField< float >*
      mutable_scores();

  // repeated .algocomp.EncodeBunch encs = 4;
  int encs_size() const;
  void clear_encs();
  static const int kEncsFieldNumber = 4;
  const ::algocomp::EncodeBunch& encs(int index) const;
  ::algocomp::EncodeBunch* mutable_encs(int index);
  ::algocomp::EncodeBunch* add_encs();
  ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeBunch >*
      mutable_encs();
  const ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeBunch >&
      encs() const;

  // int64 feat_size = 5;
  void clear_feat_size();
  static const int kFeatSizeFieldNumber = 5;
  ::google::protobuf::int64 feat_size() const;
  void set_feat_size(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:algocomp.AUCImprEncs)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  template <typename T> friend class ::google::protobuf::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::google::protobuf::RepeatedField< float > labels_;
  mutable int _labels_cached_byte_size_;
  ::google::protobuf::RepeatedField< float > weights_;
  mutable int _weights_cached_byte_size_;
  ::google::protobuf::RepeatedField< float > scores_;
  mutable int _scores_cached_byte_size_;
  ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeBunch > encs_;
  ::google::protobuf::int64 feat_size_;
  mutable int _cached_size_;
  friend struct ::protobuf_encpb_2fauc_5fimpr_5fencs_2eproto::TableStruct;
  friend void ::protobuf_encpb_2fauc_5fimpr_5fencs_2eproto::InitDefaultsAUCImprEncsImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AUCImprEncs

// repeated float labels = 1;
inline int AUCImprEncs::labels_size() const {
  return labels_.size();
}
inline void AUCImprEncs::clear_labels() {
  labels_.Clear();
}
inline float AUCImprEncs::labels(int index) const {
  // @@protoc_insertion_point(field_get:algocomp.AUCImprEncs.labels)
  return labels_.Get(index);
}
inline void AUCImprEncs::set_labels(int index, float value) {
  labels_.Set(index, value);
  // @@protoc_insertion_point(field_set:algocomp.AUCImprEncs.labels)
}
inline void AUCImprEncs::add_labels(float value) {
  labels_.Add(value);
  // @@protoc_insertion_point(field_add:algocomp.AUCImprEncs.labels)
}
inline const ::google::protobuf::RepeatedField< float >&
AUCImprEncs::labels() const {
  // @@protoc_insertion_point(field_list:algocomp.AUCImprEncs.labels)
  return labels_;
}
inline ::google::protobuf::RepeatedField< float >*
AUCImprEncs::mutable_labels() {
  // @@protoc_insertion_point(field_mutable_list:algocomp.AUCImprEncs.labels)
  return &labels_;
}

// repeated float weights = 2;
inline int AUCImprEncs::weights_size() const {
  return weights_.size();
}
inline void AUCImprEncs::clear_weights() {
  weights_.Clear();
}
inline float AUCImprEncs::weights(int index) const {
  // @@protoc_insertion_point(field_get:algocomp.AUCImprEncs.weights)
  return weights_.Get(index);
}
inline void AUCImprEncs::set_weights(int index, float value) {
  weights_.Set(index, value);
  // @@protoc_insertion_point(field_set:algocomp.AUCImprEncs.weights)
}
inline void AUCImprEncs::add_weights(float value) {
  weights_.Add(value);
  // @@protoc_insertion_point(field_add:algocomp.AUCImprEncs.weights)
}
inline const ::google::protobuf::RepeatedField< float >&
AUCImprEncs::weights() const {
  // @@protoc_insertion_point(field_list:algocomp.AUCImprEncs.weights)
  return weights_;
}
inline ::google::protobuf::RepeatedField< float >*
AUCImprEncs::mutable_weights() {
  // @@protoc_insertion_point(field_mutable_list:algocomp.AUCImprEncs.weights)
  return &weights_;
}

// repeated float scores = 3;
inline int AUCImprEncs::scores_size() const {
  return scores_.size();
}
inline void AUCImprEncs::clear_scores() {
  scores_.Clear();
}
inline float AUCImprEncs::scores(int index) const {
  // @@protoc_insertion_point(field_get:algocomp.AUCImprEncs.scores)
  return scores_.Get(index);
}
inline void AUCImprEncs::set_scores(int index, float value) {
  scores_.Set(index, value);
  // @@protoc_insertion_point(field_set:algocomp.AUCImprEncs.scores)
}
inline void AUCImprEncs::add_scores(float value) {
  scores_.Add(value);
  // @@protoc_insertion_point(field_add:algocomp.AUCImprEncs.scores)
}
inline const ::google::protobuf::RepeatedField< float >&
AUCImprEncs::scores() const {
  // @@protoc_insertion_point(field_list:algocomp.AUCImprEncs.scores)
  return scores_;
}
inline ::google::protobuf::RepeatedField< float >*
AUCImprEncs::mutable_scores() {
  // @@protoc_insertion_point(field_mutable_list:algocomp.AUCImprEncs.scores)
  return &scores_;
}

// repeated .algocomp.EncodeBunch encs = 4;
inline int AUCImprEncs::encs_size() const {
  return encs_.size();
}
inline const ::algocomp::EncodeBunch& AUCImprEncs::encs(int index) const {
  // @@protoc_insertion_point(field_get:algocomp.AUCImprEncs.encs)
  return encs_.Get(index);
}
inline ::algocomp::EncodeBunch* AUCImprEncs::mutable_encs(int index) {
  // @@protoc_insertion_point(field_mutable:algocomp.AUCImprEncs.encs)
  return encs_.Mutable(index);
}
inline ::algocomp::EncodeBunch* AUCImprEncs::add_encs() {
  // @@protoc_insertion_point(field_add:algocomp.AUCImprEncs.encs)
  return encs_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeBunch >*
AUCImprEncs::mutable_encs() {
  // @@protoc_insertion_point(field_mutable_list:algocomp.AUCImprEncs.encs)
  return &encs_;
}
inline const ::google::protobuf::RepeatedPtrField< ::algocomp::EncodeBunch >&
AUCImprEncs::encs() const {
  // @@protoc_insertion_point(field_list:algocomp.AUCImprEncs.encs)
  return encs_;
}

// int64 feat_size = 5;
inline void AUCImprEncs::clear_feat_size() {
  feat_size_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 AUCImprEncs::feat_size() const {
  // @@protoc_insertion_point(field_get:algocomp.AUCImprEncs.feat_size)
  return feat_size_;
}
inline void AUCImprEncs::set_feat_size(::google::protobuf::int64 value) {
  
  feat_size_ = value;
  // @@protoc_insertion_point(field_set:algocomp.AUCImprEncs.feat_size)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace algocomp

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_encpb_2fauc_5fimpr_5fencs_2eproto__INCLUDED
