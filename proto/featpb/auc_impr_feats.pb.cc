// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: featpb/auc_impr_feats.proto

#include "featpb/auc_impr_feats.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace algocomp {
class AUCImprFeatsDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<AUCImprFeats>
      _instance;
} _AUCImprFeats_default_instance_;
}  // namespace algocomp
namespace protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto {
void InitDefaultsAUCImprFeatsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  protobuf_featpb_2ffeature_5fbunch_2eproto::InitDefaultsFeatureBunch();
  {
    void* ptr = &::algocomp::_AUCImprFeats_default_instance_;
    new (ptr) ::algocomp::AUCImprFeats();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::algocomp::AUCImprFeats::InitAsDefaultInstance();
}

void InitDefaultsAUCImprFeats() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsAUCImprFeatsImpl);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::AUCImprFeats, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::AUCImprFeats, labels_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::AUCImprFeats, weights_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::AUCImprFeats, scores_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::AUCImprFeats, user_and_ctx_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::AUCImprFeats, ads_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::algocomp::AUCImprFeats)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::algocomp::_AUCImprFeats_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "featpb/auc_impr_feats.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\033featpb/auc_impr_feats.proto\022\010algocomp\032"
      "\032featpb/feature_bunch.proto\"\222\001\n\014AUCImprF"
      "eats\022\016\n\006labels\030\001 \003(\002\022\017\n\007weights\030\002 \003(\002\022\016\n"
      "\006scores\030\003 \003(\002\022,\n\014user_and_ctx\030\004 \001(\0132\026.al"
      "gocomp.FeatureBunch\022#\n\003ads\030\005 \003(\0132\026.algoc"
      "omp.FeatureBunchB\037\n\010algocompB\016AUCImprFea"
      "tsPbP\001\370\001\001b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 257);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "featpb/auc_impr_feats.proto", &protobuf_RegisterTypes);
  ::protobuf_featpb_2ffeature_5fbunch_2eproto::AddDescriptors();
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto
namespace algocomp {

// ===================================================================

void AUCImprFeats::InitAsDefaultInstance() {
  ::algocomp::_AUCImprFeats_default_instance_._instance.get_mutable()->user_and_ctx_ = const_cast< ::algocomp::FeatureBunch*>(
      ::algocomp::FeatureBunch::internal_default_instance());
}
void AUCImprFeats::_slow_mutable_user_and_ctx() {
  user_and_ctx_ = ::google::protobuf::Arena::CreateMessage< ::algocomp::FeatureBunch >(
      GetArenaNoVirtual());
}
void AUCImprFeats::unsafe_arena_set_allocated_user_and_ctx(
    ::algocomp::FeatureBunch* user_and_ctx) {
  if (GetArenaNoVirtual() == NULL) {
    delete user_and_ctx_;
  }
  user_and_ctx_ = user_and_ctx;
  if (user_and_ctx) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:algocomp.AUCImprFeats.user_and_ctx)
}
void AUCImprFeats::clear_user_and_ctx() {
  if (GetArenaNoVirtual() == NULL && user_and_ctx_ != NULL) {
    delete user_and_ctx_;
  }
  user_and_ctx_ = NULL;
}
void AUCImprFeats::clear_ads() {
  ads_.Clear();
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int AUCImprFeats::kLabelsFieldNumber;
const int AUCImprFeats::kWeightsFieldNumber;
const int AUCImprFeats::kScoresFieldNumber;
const int AUCImprFeats::kUserAndCtxFieldNumber;
const int AUCImprFeats::kAdsFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

AUCImprFeats::AUCImprFeats()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto::InitDefaultsAUCImprFeats();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:algocomp.AUCImprFeats)
}
AUCImprFeats::AUCImprFeats(::google::protobuf::Arena* arena)
  : ::google::protobuf::Message(),
  _internal_metadata_(arena),
  labels_(arena),
  weights_(arena),
  scores_(arena),
  ads_(arena) {
  ::protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto::InitDefaultsAUCImprFeats();
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:algocomp.AUCImprFeats)
}
AUCImprFeats::AUCImprFeats(const AUCImprFeats& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      labels_(from.labels_),
      weights_(from.weights_),
      scores_(from.scores_),
      ads_(from.ads_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.has_user_and_ctx()) {
    user_and_ctx_ = new ::algocomp::FeatureBunch(*from.user_and_ctx_);
  } else {
    user_and_ctx_ = NULL;
  }
  // @@protoc_insertion_point(copy_constructor:algocomp.AUCImprFeats)
}

void AUCImprFeats::SharedCtor() {
  user_and_ctx_ = NULL;
  _cached_size_ = 0;
}

AUCImprFeats::~AUCImprFeats() {
  // @@protoc_insertion_point(destructor:algocomp.AUCImprFeats)
  SharedDtor();
}

void AUCImprFeats::SharedDtor() {
  GOOGLE_DCHECK(GetArenaNoVirtual() == NULL);
  if (this != internal_default_instance()) delete user_and_ctx_;
}

void AUCImprFeats::ArenaDtor(void* object) {
  AUCImprFeats* _this = reinterpret_cast< AUCImprFeats* >(object);
  (void)_this;
}
void AUCImprFeats::RegisterArenaDtor(::google::protobuf::Arena* arena) {
}
void AUCImprFeats::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AUCImprFeats::descriptor() {
  ::protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const AUCImprFeats& AUCImprFeats::default_instance() {
  ::protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto::InitDefaultsAUCImprFeats();
  return *internal_default_instance();
}

AUCImprFeats* AUCImprFeats::New(::google::protobuf::Arena* arena) const {
  return ::google::protobuf::Arena::CreateMessage<AUCImprFeats>(arena);
}

void AUCImprFeats::Clear() {
// @@protoc_insertion_point(message_clear_start:algocomp.AUCImprFeats)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  labels_.Clear();
  weights_.Clear();
  scores_.Clear();
  ads_.Clear();
  if (GetArenaNoVirtual() == NULL && user_and_ctx_ != NULL) {
    delete user_and_ctx_;
  }
  user_and_ctx_ = NULL;
  _internal_metadata_.Clear();
}

bool AUCImprFeats::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:algocomp.AUCImprFeats)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated float labels = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, this->mutable_labels())));
        } else if (
            static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(13u /* 13 & 0xFF */)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitiveNoInline<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 1, 10u, input, this->mutable_labels())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated float weights = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, this->mutable_weights())));
        } else if (
            static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(21u /* 21 & 0xFF */)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitiveNoInline<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 1, 18u, input, this->mutable_weights())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated float scores = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(26u /* 26 & 0xFF */)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, this->mutable_scores())));
        } else if (
            static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(29u /* 29 & 0xFF */)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitiveNoInline<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 1, 26u, input, this->mutable_scores())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .algocomp.FeatureBunch user_and_ctx = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 34 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_user_and_ctx()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .algocomp.FeatureBunch ads = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(42u /* 42 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(input, add_ads()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:algocomp.AUCImprFeats)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:algocomp.AUCImprFeats)
  return false;
#undef DO_
}

void AUCImprFeats::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:algocomp.AUCImprFeats)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated float labels = 1;
  if (this->labels_size() > 0) {
    ::google::protobuf::internal::WireFormatLite::WriteTag(1, ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED, output);
    output->WriteVarint32(static_cast< ::google::protobuf::uint32>(
        _labels_cached_byte_size_));
    ::google::protobuf::internal::WireFormatLite::WriteFloatArray(
      this->labels().data(), this->labels_size(), output);
  }

  // repeated float weights = 2;
  if (this->weights_size() > 0) {
    ::google::protobuf::internal::WireFormatLite::WriteTag(2, ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED, output);
    output->WriteVarint32(static_cast< ::google::protobuf::uint32>(
        _weights_cached_byte_size_));
    ::google::protobuf::internal::WireFormatLite::WriteFloatArray(
      this->weights().data(), this->weights_size(), output);
  }

  // repeated float scores = 3;
  if (this->scores_size() > 0) {
    ::google::protobuf::internal::WireFormatLite::WriteTag(3, ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED, output);
    output->WriteVarint32(static_cast< ::google::protobuf::uint32>(
        _scores_cached_byte_size_));
    ::google::protobuf::internal::WireFormatLite::WriteFloatArray(
      this->scores().data(), this->scores_size(), output);
  }

  // .algocomp.FeatureBunch user_and_ctx = 4;
  if (this->has_user_and_ctx()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      4, *this->user_and_ctx_, output);
  }

  // repeated .algocomp.FeatureBunch ads = 5;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->ads_size()); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      5, this->ads(static_cast<int>(i)), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:algocomp.AUCImprFeats)
}

::google::protobuf::uint8* AUCImprFeats::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:algocomp.AUCImprFeats)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated float labels = 1;
  if (this->labels_size() > 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteTagToArray(
      1,
      ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      target);
    target = ::google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(
        static_cast< ::google::protobuf::int32>(
            _labels_cached_byte_size_), target);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteFloatNoTagToArray(this->labels_, target);
  }

  // repeated float weights = 2;
  if (this->weights_size() > 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteTagToArray(
      2,
      ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      target);
    target = ::google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(
        static_cast< ::google::protobuf::int32>(
            _weights_cached_byte_size_), target);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteFloatNoTagToArray(this->weights_, target);
  }

  // repeated float scores = 3;
  if (this->scores_size() > 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteTagToArray(
      3,
      ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      target);
    target = ::google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(
        static_cast< ::google::protobuf::int32>(
            _scores_cached_byte_size_), target);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteFloatNoTagToArray(this->scores_, target);
  }

  // .algocomp.FeatureBunch user_and_ctx = 4;
  if (this->has_user_and_ctx()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        4, *this->user_and_ctx_, deterministic, target);
  }

  // repeated .algocomp.FeatureBunch ads = 5;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->ads_size()); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        5, this->ads(static_cast<int>(i)), deterministic, target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:algocomp.AUCImprFeats)
  return target;
}

size_t AUCImprFeats::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:algocomp.AUCImprFeats)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // repeated float labels = 1;
  {
    unsigned int count = static_cast<unsigned int>(this->labels_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
            static_cast< ::google::protobuf::int32>(data_size));
    }
    int cached_size = ::google::protobuf::internal::ToCachedSize(data_size);
    GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
    _labels_cached_byte_size_ = cached_size;
    GOOGLE_SAFE_CONCURRENT_WRITES_END();
    total_size += data_size;
  }

  // repeated float weights = 2;
  {
    unsigned int count = static_cast<unsigned int>(this->weights_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
            static_cast< ::google::protobuf::int32>(data_size));
    }
    int cached_size = ::google::protobuf::internal::ToCachedSize(data_size);
    GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
    _weights_cached_byte_size_ = cached_size;
    GOOGLE_SAFE_CONCURRENT_WRITES_END();
    total_size += data_size;
  }

  // repeated float scores = 3;
  {
    unsigned int count = static_cast<unsigned int>(this->scores_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
            static_cast< ::google::protobuf::int32>(data_size));
    }
    int cached_size = ::google::protobuf::internal::ToCachedSize(data_size);
    GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
    _scores_cached_byte_size_ = cached_size;
    GOOGLE_SAFE_CONCURRENT_WRITES_END();
    total_size += data_size;
  }

  // repeated .algocomp.FeatureBunch ads = 5;
  {
    unsigned int count = static_cast<unsigned int>(this->ads_size());
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          this->ads(static_cast<int>(i)));
    }
  }

  // .algocomp.FeatureBunch user_and_ctx = 4;
  if (this->has_user_and_ctx()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *this->user_and_ctx_);
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void AUCImprFeats::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:algocomp.AUCImprFeats)
  GOOGLE_DCHECK_NE(&from, this);
  const AUCImprFeats* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const AUCImprFeats>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:algocomp.AUCImprFeats)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:algocomp.AUCImprFeats)
    MergeFrom(*source);
  }
}

void AUCImprFeats::MergeFrom(const AUCImprFeats& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:algocomp.AUCImprFeats)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  labels_.MergeFrom(from.labels_);
  weights_.MergeFrom(from.weights_);
  scores_.MergeFrom(from.scores_);
  ads_.MergeFrom(from.ads_);
  if (from.has_user_and_ctx()) {
    mutable_user_and_ctx()->::algocomp::FeatureBunch::MergeFrom(from.user_and_ctx());
  }
}

void AUCImprFeats::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:algocomp.AUCImprFeats)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AUCImprFeats::CopyFrom(const AUCImprFeats& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:algocomp.AUCImprFeats)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AUCImprFeats::IsInitialized() const {
  return true;
}

void AUCImprFeats::Swap(AUCImprFeats* other) {
  if (other == this) return;
  if (GetArenaNoVirtual() == other->GetArenaNoVirtual()) {
    InternalSwap(other);
  } else {
    AUCImprFeats* temp = New(GetArenaNoVirtual());
    temp->MergeFrom(*other);
    other->CopyFrom(*this);
    InternalSwap(temp);
    if (GetArenaNoVirtual() == NULL) {
      delete temp;
    }
  }
}
void AUCImprFeats::UnsafeArenaSwap(AUCImprFeats* other) {
  if (other == this) return;
  GOOGLE_DCHECK(GetArenaNoVirtual() == other->GetArenaNoVirtual());
  InternalSwap(other);
}
void AUCImprFeats::InternalSwap(AUCImprFeats* other) {
  using std::swap;
  labels_.InternalSwap(&other->labels_);
  weights_.InternalSwap(&other->weights_);
  scores_.InternalSwap(&other->scores_);
  ads_.InternalSwap(&other->ads_);
  swap(user_and_ctx_, other->user_and_ctx_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata AUCImprFeats::GetMetadata() const {
  protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_featpb_2fauc_5fimpr_5ffeats_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace algocomp

// @@protoc_insertion_point(global_scope)
