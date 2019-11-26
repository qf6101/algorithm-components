// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: featpb/disc_feat_data.proto

#include "featpb/disc_feat_data.pb.h"

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
class DiscFeatDataDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<DiscFeatData>
      _instance;
} _DiscFeatData_default_instance_;
}  // namespace algocomp
namespace protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto {
void InitDefaultsDiscFeatDataImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::algocomp::_DiscFeatData_default_instance_;
    new (ptr) ::algocomp::DiscFeatData();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::algocomp::DiscFeatData::InitAsDefaultInstance();
}

void InitDefaultsDiscFeatData() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsDiscFeatDataImpl);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::DiscFeatData, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::DiscFeatData, gname_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::algocomp::DiscFeatData, fname_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::algocomp::DiscFeatData)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::algocomp::_DiscFeatData_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "featpb/disc_feat_data.proto", schemas, file_default_instances, TableStruct::offsets, factory,
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
      "\n\033featpb/disc_feat_data.proto\022\010algocomp\""
      ",\n\014DiscFeatData\022\r\n\005gname\030\001 \001(\t\022\r\n\005fname\030"
      "\002 \001(\tB\037\n\010algocompB\016DiscFeatDataPbP\001\370\001\001b\006"
      "proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 126);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "featpb/disc_feat_data.proto", &protobuf_RegisterTypes);
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
}  // namespace protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto
namespace algocomp {

// ===================================================================

void DiscFeatData::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int DiscFeatData::kGnameFieldNumber;
const int DiscFeatData::kFnameFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

DiscFeatData::DiscFeatData()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto::InitDefaultsDiscFeatData();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:algocomp.DiscFeatData)
}
DiscFeatData::DiscFeatData(::google::protobuf::Arena* arena)
  : ::google::protobuf::Message(),
  _internal_metadata_(arena) {
  ::protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto::InitDefaultsDiscFeatData();
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:algocomp.DiscFeatData)
}
DiscFeatData::DiscFeatData(const DiscFeatData& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  gname_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.gname().size() > 0) {
    gname_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.gname(),
      GetArenaNoVirtual());
  }
  fname_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.fname().size() > 0) {
    fname_.Set(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.fname(),
      GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(copy_constructor:algocomp.DiscFeatData)
}

void DiscFeatData::SharedCtor() {
  gname_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  fname_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _cached_size_ = 0;
}

DiscFeatData::~DiscFeatData() {
  // @@protoc_insertion_point(destructor:algocomp.DiscFeatData)
  SharedDtor();
}

void DiscFeatData::SharedDtor() {
  GOOGLE_DCHECK(GetArenaNoVirtual() == NULL);
  gname_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  fname_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void DiscFeatData::ArenaDtor(void* object) {
  DiscFeatData* _this = reinterpret_cast< DiscFeatData* >(object);
  (void)_this;
}
void DiscFeatData::RegisterArenaDtor(::google::protobuf::Arena* arena) {
}
void DiscFeatData::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* DiscFeatData::descriptor() {
  ::protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const DiscFeatData& DiscFeatData::default_instance() {
  ::protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto::InitDefaultsDiscFeatData();
  return *internal_default_instance();
}

DiscFeatData* DiscFeatData::New(::google::protobuf::Arena* arena) const {
  return ::google::protobuf::Arena::CreateMessage<DiscFeatData>(arena);
}

void DiscFeatData::Clear() {
// @@protoc_insertion_point(message_clear_start:algocomp.DiscFeatData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  gname_.ClearToEmpty(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
  fname_.ClearToEmpty(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), GetArenaNoVirtual());
  _internal_metadata_.Clear();
}

bool DiscFeatData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:algocomp.DiscFeatData)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // string gname = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_gname()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->gname().data(), static_cast<int>(this->gname().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "algocomp.DiscFeatData.gname"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string fname = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_fname()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->fname().data(), static_cast<int>(this->fname().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "algocomp.DiscFeatData.fname"));
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
  // @@protoc_insertion_point(parse_success:algocomp.DiscFeatData)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:algocomp.DiscFeatData)
  return false;
#undef DO_
}

void DiscFeatData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:algocomp.DiscFeatData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string gname = 1;
  if (this->gname().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->gname().data(), static_cast<int>(this->gname().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "algocomp.DiscFeatData.gname");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->gname(), output);
  }

  // string fname = 2;
  if (this->fname().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->fname().data(), static_cast<int>(this->fname().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "algocomp.DiscFeatData.fname");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->fname(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:algocomp.DiscFeatData)
}

::google::protobuf::uint8* DiscFeatData::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:algocomp.DiscFeatData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string gname = 1;
  if (this->gname().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->gname().data(), static_cast<int>(this->gname().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "algocomp.DiscFeatData.gname");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->gname(), target);
  }

  // string fname = 2;
  if (this->fname().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->fname().data(), static_cast<int>(this->fname().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "algocomp.DiscFeatData.fname");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->fname(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:algocomp.DiscFeatData)
  return target;
}

size_t DiscFeatData::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:algocomp.DiscFeatData)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // string gname = 1;
  if (this->gname().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->gname());
  }

  // string fname = 2;
  if (this->fname().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->fname());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void DiscFeatData::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:algocomp.DiscFeatData)
  GOOGLE_DCHECK_NE(&from, this);
  const DiscFeatData* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const DiscFeatData>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:algocomp.DiscFeatData)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:algocomp.DiscFeatData)
    MergeFrom(*source);
  }
}

void DiscFeatData::MergeFrom(const DiscFeatData& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:algocomp.DiscFeatData)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.gname().size() > 0) {
    set_gname(from.gname());
  }
  if (from.fname().size() > 0) {
    set_fname(from.fname());
  }
}

void DiscFeatData::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:algocomp.DiscFeatData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void DiscFeatData::CopyFrom(const DiscFeatData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:algocomp.DiscFeatData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DiscFeatData::IsInitialized() const {
  return true;
}

void DiscFeatData::Swap(DiscFeatData* other) {
  if (other == this) return;
  if (GetArenaNoVirtual() == other->GetArenaNoVirtual()) {
    InternalSwap(other);
  } else {
    DiscFeatData* temp = New(GetArenaNoVirtual());
    temp->MergeFrom(*other);
    other->CopyFrom(*this);
    InternalSwap(temp);
    if (GetArenaNoVirtual() == NULL) {
      delete temp;
    }
  }
}
void DiscFeatData::UnsafeArenaSwap(DiscFeatData* other) {
  if (other == this) return;
  GOOGLE_DCHECK(GetArenaNoVirtual() == other->GetArenaNoVirtual());
  InternalSwap(other);
}
void DiscFeatData::InternalSwap(DiscFeatData* other) {
  using std::swap;
  gname_.Swap(&other->gname_);
  fname_.Swap(&other->fname_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata DiscFeatData::GetMetadata() const {
  protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_featpb_2fdisc_5ffeat_5fdata_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace algocomp

// @@protoc_insertion_point(global_scope)
