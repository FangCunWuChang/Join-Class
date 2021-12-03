// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: class.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_class_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_class_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_class_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_class_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_class_2eproto;
namespace JC {
class Class;
struct ClassDefaultTypeInternal;
extern ClassDefaultTypeInternal _Class_default_instance_;
}  // namespace JC
PROTOBUF_NAMESPACE_OPEN
template<> ::JC::Class* Arena::CreateMaybeMessage<::JC::Class>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace JC {

enum Class_Type : int {
  Class_Type_PUBLIC = 0,
  Class_Type_REQUIRED = 1,
  Class_Type_OPTIONAL = 2,
  Class_Type_Class_Type_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  Class_Type_Class_Type_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool Class_Type_IsValid(int value);
constexpr Class_Type Class_Type_Type_MIN = Class_Type_PUBLIC;
constexpr Class_Type Class_Type_Type_MAX = Class_Type_OPTIONAL;
constexpr int Class_Type_Type_ARRAYSIZE = Class_Type_Type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Class_Type_descriptor();
template<typename T>
inline const std::string& Class_Type_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Class_Type>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Class_Type_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Class_Type_descriptor(), enum_t_value);
}
inline bool Class_Type_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, Class_Type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Class_Type>(
    Class_Type_descriptor(), name, value);
}
// ===================================================================

class Class final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:JC.Class) */ {
 public:
  inline Class() : Class(nullptr) {}
  ~Class() override;
  explicit constexpr Class(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Class(const Class& from);
  Class(Class&& from) noexcept
    : Class() {
    *this = ::std::move(from);
  }

  inline Class& operator=(const Class& from) {
    CopyFrom(from);
    return *this;
  }
  inline Class& operator=(Class&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Class& default_instance() {
    return *internal_default_instance();
  }
  static inline const Class* internal_default_instance() {
    return reinterpret_cast<const Class*>(
               &_Class_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Class& a, Class& b) {
    a.Swap(&b);
  }
  inline void Swap(Class* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Class* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Class* New() const final {
    return new Class();
  }

  Class* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Class>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Class& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Class& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Class* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "JC.Class";
  }
  protected:
  explicit Class(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef Class_Type Type;
  static constexpr Type PUBLIC =
    Class_Type_PUBLIC;
  static constexpr Type REQUIRED =
    Class_Type_REQUIRED;
  static constexpr Type OPTIONAL =
    Class_Type_OPTIONAL;
  static inline bool Type_IsValid(int value) {
    return Class_Type_IsValid(value);
  }
  static constexpr Type Type_MIN =
    Class_Type_Type_MIN;
  static constexpr Type Type_MAX =
    Class_Type_Type_MAX;
  static constexpr int Type_ARRAYSIZE =
    Class_Type_Type_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Type_descriptor() {
    return Class_Type_descriptor();
  }
  template<typename T>
  static inline const std::string& Type_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Type>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Type_Name.");
    return Class_Type_Name(enum_t_value);
  }
  static inline bool Type_Parse(::PROTOBUF_NAMESPACE_ID::ConstStringParam name,
      Type* value) {
    return Class_Type_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kIdFieldNumber = 1,
    kNameFieldNumber = 2,
    kTermFieldNumber = 7,
    kTypeFieldNumber = 3,
    kTimeClassFieldNumber = 4,
    kTimeExperFieldNumber = 5,
    kCreditFieldNumber = 6,
  };
  // string id = 1;
  void clear_id();
  const std::string& id() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_id(ArgT0&& arg0, ArgT... args);
  std::string* mutable_id();
  PROTOBUF_MUST_USE_RESULT std::string* release_id();
  void set_allocated_id(std::string* id);
  private:
  const std::string& _internal_id() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_id(const std::string& value);
  std::string* _internal_mutable_id();
  public:

  // string name = 2;
  void clear_name();
  const std::string& name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_name();
  PROTOBUF_MUST_USE_RESULT std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // string term = 7;
  void clear_term();
  const std::string& term() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_term(ArgT0&& arg0, ArgT... args);
  std::string* mutable_term();
  PROTOBUF_MUST_USE_RESULT std::string* release_term();
  void set_allocated_term(std::string* term);
  private:
  const std::string& _internal_term() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_term(const std::string& value);
  std::string* _internal_mutable_term();
  public:

  // .JC.Class.Type type = 3;
  void clear_type();
  ::JC::Class_Type type() const;
  void set_type(::JC::Class_Type value);
  private:
  ::JC::Class_Type _internal_type() const;
  void _internal_set_type(::JC::Class_Type value);
  public:

  // uint32 timeClass = 4;
  void clear_timeclass();
  ::PROTOBUF_NAMESPACE_ID::uint32 timeclass() const;
  void set_timeclass(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_timeclass() const;
  void _internal_set_timeclass(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 timeExper = 5;
  void clear_timeexper();
  ::PROTOBUF_NAMESPACE_ID::uint32 timeexper() const;
  void set_timeexper(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_timeexper() const;
  void _internal_set_timeexper(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 credit = 6;
  void clear_credit();
  ::PROTOBUF_NAMESPACE_ID::uint32 credit() const;
  void set_credit(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_credit() const;
  void _internal_set_credit(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:JC.Class)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr id_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr term_;
  int type_;
  ::PROTOBUF_NAMESPACE_ID::uint32 timeclass_;
  ::PROTOBUF_NAMESPACE_ID::uint32 timeexper_;
  ::PROTOBUF_NAMESPACE_ID::uint32 credit_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_class_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Class

// string id = 1;
inline void Class::clear_id() {
  id_.ClearToEmpty();
}
inline const std::string& Class::id() const {
  // @@protoc_insertion_point(field_get:JC.Class.id)
  return _internal_id();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Class::set_id(ArgT0&& arg0, ArgT... args) {
 
 id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:JC.Class.id)
}
inline std::string* Class::mutable_id() {
  std::string* _s = _internal_mutable_id();
  // @@protoc_insertion_point(field_mutable:JC.Class.id)
  return _s;
}
inline const std::string& Class::_internal_id() const {
  return id_.Get();
}
inline void Class::_internal_set_id(const std::string& value) {
  
  id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* Class::_internal_mutable_id() {
  
  return id_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* Class::release_id() {
  // @@protoc_insertion_point(field_release:JC.Class.id)
  return id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void Class::set_allocated_id(std::string* id) {
  if (id != nullptr) {
    
  } else {
    
  }
  id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), id,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:JC.Class.id)
}

// string name = 2;
inline void Class::clear_name() {
  name_.ClearToEmpty();
}
inline const std::string& Class::name() const {
  // @@protoc_insertion_point(field_get:JC.Class.name)
  return _internal_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Class::set_name(ArgT0&& arg0, ArgT... args) {
 
 name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:JC.Class.name)
}
inline std::string* Class::mutable_name() {
  std::string* _s = _internal_mutable_name();
  // @@protoc_insertion_point(field_mutable:JC.Class.name)
  return _s;
}
inline const std::string& Class::_internal_name() const {
  return name_.Get();
}
inline void Class::_internal_set_name(const std::string& value) {
  
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* Class::_internal_mutable_name() {
  
  return name_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* Class::release_name() {
  // @@protoc_insertion_point(field_release:JC.Class.name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void Class::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:JC.Class.name)
}

// .JC.Class.Type type = 3;
inline void Class::clear_type() {
  type_ = 0;
}
inline ::JC::Class_Type Class::_internal_type() const {
  return static_cast< ::JC::Class_Type >(type_);
}
inline ::JC::Class_Type Class::type() const {
  // @@protoc_insertion_point(field_get:JC.Class.type)
  return _internal_type();
}
inline void Class::_internal_set_type(::JC::Class_Type value) {
  
  type_ = value;
}
inline void Class::set_type(::JC::Class_Type value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:JC.Class.type)
}

// uint32 timeClass = 4;
inline void Class::clear_timeclass() {
  timeclass_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Class::_internal_timeclass() const {
  return timeclass_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Class::timeclass() const {
  // @@protoc_insertion_point(field_get:JC.Class.timeClass)
  return _internal_timeclass();
}
inline void Class::_internal_set_timeclass(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  timeclass_ = value;
}
inline void Class::set_timeclass(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_timeclass(value);
  // @@protoc_insertion_point(field_set:JC.Class.timeClass)
}

// uint32 timeExper = 5;
inline void Class::clear_timeexper() {
  timeexper_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Class::_internal_timeexper() const {
  return timeexper_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Class::timeexper() const {
  // @@protoc_insertion_point(field_get:JC.Class.timeExper)
  return _internal_timeexper();
}
inline void Class::_internal_set_timeexper(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  timeexper_ = value;
}
inline void Class::set_timeexper(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_timeexper(value);
  // @@protoc_insertion_point(field_set:JC.Class.timeExper)
}

// uint32 credit = 6;
inline void Class::clear_credit() {
  credit_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Class::_internal_credit() const {
  return credit_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Class::credit() const {
  // @@protoc_insertion_point(field_get:JC.Class.credit)
  return _internal_credit();
}
inline void Class::_internal_set_credit(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  credit_ = value;
}
inline void Class::set_credit(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_credit(value);
  // @@protoc_insertion_point(field_set:JC.Class.credit)
}

// string term = 7;
inline void Class::clear_term() {
  term_.ClearToEmpty();
}
inline const std::string& Class::term() const {
  // @@protoc_insertion_point(field_get:JC.Class.term)
  return _internal_term();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Class::set_term(ArgT0&& arg0, ArgT... args) {
 
 term_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:JC.Class.term)
}
inline std::string* Class::mutable_term() {
  std::string* _s = _internal_mutable_term();
  // @@protoc_insertion_point(field_mutable:JC.Class.term)
  return _s;
}
inline const std::string& Class::_internal_term() const {
  return term_.Get();
}
inline void Class::_internal_set_term(const std::string& value) {
  
  term_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* Class::_internal_mutable_term() {
  
  return term_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* Class::release_term() {
  // @@protoc_insertion_point(field_release:JC.Class.term)
  return term_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void Class::set_allocated_term(std::string* term) {
  if (term != nullptr) {
    
  } else {
    
  }
  term_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), term,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:JC.Class.term)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace JC

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::JC::Class_Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::JC::Class_Type>() {
  return ::JC::Class_Type_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_class_2eproto
