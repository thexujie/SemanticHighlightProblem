#pragma once

#include "Types.h"
#include "Flag.h"
#include "StringView.h"
#include "Variant.h"

namespace XE
{
	enum class ETypeFlag
	{
		None = 0,
		Basic = 0x0001,
		System = 0x0002,
		Abstract = 0x0004,
	};
	using ETypeFlags = TFlag<ETypeFlag>;

	class IValue;
	class IProperty;
	class IFormatter;
	class IDataConverter;
	class IDataProvider;
	
	class CORE_API INativeType
	{
	public:
		INativeType(const std::type_info & StdTypeInfoIn) : StdTypeInfo(StdTypeInfoIn) {}
		virtual ~INativeType() = default;
		
		virtual FStringView GetName() const = 0;
		virtual ETypeFlags GetTypeFlags() const = 0;

		virtual uintx_t GetSize() const = 0;
		virtual uintx_t GetAlignment() const = 0;
		
		virtual void Construct(void * Pointer) const = 0;
		virtual void Destruct(void * Pointer) const = 0;

		virtual void SetParent(const INativeType * Type) const = 0;
		virtual const INativeType * GetParent() const = 0;

		virtual void AppendChild(const INativeType * Type) const = 0;
		virtual const INativeType * GetFirstChild() const = 0;
		
		virtual void SetNextSibling(const INativeType * Type) const = 0;
		virtual const INativeType * GetNextSibling() const = 0;

		virtual FVariant CreateValue() const = 0;

		virtual FStringView Format(const FVariant & Variant) const = 0;
		virtual FVariant Format(FStringView String) const = 0;

		virtual const IProperty * GetProperty(FStringView Name) const = 0;
		
	public:
		bool IsParentOf(const INativeType * Another) const;
		bool IsChildOf(const INativeType * Another) const;

	protected:
		const std::type_info & StdTypeInfo;
	};

	class CORE_API FType
	{
	public:
		FType() = default;
		FType(const INativeType * NativeTypeIn) : NativeType(NativeTypeIn) {}
		FType(const FType &) = default;
		FType(FType &&) noexcept = default;
		FType & operator = (const FType &) noexcept = default;
		virtual ~FType() = default;
		
		bool operator==(const FType & Another) const noexcept = default;
		auto operator<=>(const FType & Another) const noexcept = default;

		const INativeType * GetNativeType() const { return NativeType; }
		explicit operator bool() const { return !!NativeType; }
		FStringView GetName() const { return NativeType ? NativeType->GetName() : FStringView::Empty; }
		FStringView GetShortName() const;

		ETypeFlags GetTypeFlags() const { return NativeType ? NativeType->GetTypeFlags() : ETypeFlag::None; }
		void Construct(void * Pointer) const { if (NativeType) NativeType->Construct(Pointer); }
		void Destruct(void * Pointer) const { if (NativeType) NativeType->Destruct(Pointer); }

		FType GetParent() const { return NativeType ? FType(NativeType->GetParent()) : Unkown; }
		uintx_t GetSize() const { return NativeType ? NativeType->GetSize() : 0; }
		uintx_t GetAlignment() const { return NativeType ? NativeType->GetAlignment() : 0; }

	public:
		FStringView Format(const FVariant & Variant) const { return NativeType ? NativeType->Format(Variant) : nullptr; }
		FVariant Format(FStringView String) const { return NativeType ? NativeType->Format(String) : FVariant(); }
		
	public:
		FVariant CreateValue() const { return NativeType ? NativeType->CreateValue() : nullptr; }
		const IProperty * GetProperty(FStringView Name) const { return NativeType ? NativeType ->GetProperty(Name) : nullptr; }
		
	public:
		bool IsParentOf(const FType & Another) const { return NativeType ? NativeType->IsParentOf(Another.GetNativeType()) : false; }
		bool IsChildOf(const FType & Another) const { return NativeType ? NativeType->IsChildOf(Another.GetNativeType()) : false; }
		bool DefaultConvertableTo(const FType & Another) const { if (!NativeType) return false;  return operator==(Another) || IsChildOf(Another); }
		bool DefaultConvertableFrom(const FType & Another) const { if (!NativeType) return false;  return operator==(Another) || IsParentOf(Another); }
		
	public:
		bool IsBasic() const { return GetTypeFlags().Any(ETypeFlag::Basic); }
		bool IsSystem() const { return GetTypeFlags().Any(ETypeFlag::System); }
		bool IsAbstract() const { return GetTypeFlags().Any(ETypeFlag::Abstract); }
	protected:
		const INativeType * NativeType = nullptr;

	public:
		static FType Unkown;
	};
	
	CORE_API FType TypeBy(const Char * Name);
	template<typename T>
	FType TypeOf()
	{
		return TypeBy(reinterpret_cast<const Char*>(typeid(T).name()));
	}
}
