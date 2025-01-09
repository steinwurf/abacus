// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: abacus/protobuf/kind.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_abacus_2fprotobuf_2fkind_2eproto_2epb_2eh
#define GOOGLE_PROTOBUF_INCLUDED_abacus_2fprotobuf_2fkind_2eproto_2epb_2eh

#include <limits>
#include <string>
#include <type_traits>

#include "google/protobuf/port_def.inc"
#if PROTOBUF_VERSION < 4024000
#error "This file was generated by a newer version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please update"
#error "your headers."
#endif  // PROTOBUF_VERSION

#if 4024003 < PROTOBUF_MIN_PROTOC_VERSION
#error "This file was generated by an older version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please"
#error "regenerate this file with a newer version of protoc."
#endif  // PROTOBUF_MIN_PROTOC_VERSION
#include "google/protobuf/port_undef.inc"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
#include "google/protobuf/generated_enum_reflection.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_abacus_2fprotobuf_2fkind_2eproto

namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_abacus_2fprotobuf_2fkind_2eproto {
  static const ::uint32_t offsets[];
};
extern const ::google::protobuf::internal::DescriptorTable
    descriptor_table_abacus_2fprotobuf_2fkind_2eproto;
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace abacus {
namespace protobuf {
enum Kind : int {
  GAUGE = 0,
  COUNTER = 1,
  CONSTANT = 2,
  Kind_INT_MIN_SENTINEL_DO_NOT_USE_ =
      std::numeric_limits<::int32_t>::min(),
  Kind_INT_MAX_SENTINEL_DO_NOT_USE_ =
      std::numeric_limits<::int32_t>::max(),
};

bool Kind_IsValid(int value);
constexpr Kind Kind_MIN = static_cast<Kind>(0);
constexpr Kind Kind_MAX = static_cast<Kind>(2);
constexpr int Kind_ARRAYSIZE = 2 + 1;
const ::google::protobuf::EnumDescriptor*
Kind_descriptor();
template <typename T>
const std::string& Kind_Name(T value) {
  static_assert(std::is_same<T, Kind>::value ||
                    std::is_integral<T>::value,
                "Incorrect type passed to Kind_Name().");
  return Kind_Name(static_cast<Kind>(value));
}
template <>
inline const std::string& Kind_Name(Kind value) {
  return ::google::protobuf::internal::NameOfDenseEnum<Kind_descriptor,
                                                 0, 2>(
      static_cast<int>(value));
}
inline bool Kind_Parse(absl::string_view name, Kind* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Kind>(
      Kind_descriptor(), name, value);
}

// ===================================================================



// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace protobuf
}  // namespace abacus


namespace google {
namespace protobuf {

template <>
struct is_proto_enum<::abacus::protobuf::Kind> : std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor<::abacus::protobuf::Kind>() {
  return ::abacus::protobuf::Kind_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_INCLUDED_abacus_2fprotobuf_2fkind_2eproto_2epb_2eh
