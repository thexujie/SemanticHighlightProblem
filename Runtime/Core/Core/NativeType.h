#pragma once

#include "CoreInc.h"
#include "Converters.h"

namespace XE
{
	inline namespace Private
	{
		template<typename MemberT>
		struct PropertyMemberPointerHelper
		{
			using InstanceType = RemoveCVT<typename IsMemberPointer<MemberT>::InstanceType>;
			using ValueType = RemoveCVT<typename IsMemberPointer<MemberT>::ValueType>;
		};

		template<>
		struct PropertyMemberPointerHelper<NullPtrT>
		{
			using InstanceType = VoidT<NullPtrT>;
			using ValueType = VoidT<NullPtrT>;
		};
	}
	
	template<typename GetterT, typename SetterT>
	class TProperty : public TRefer<IProperty>
	{
	public:
		using InstanceType = ConditionalT<IsNullPointerV<GetterT>, typename PropertyMemberPointerHelper<SetterT>::InstanceType, typename PropertyMemberPointerHelper<GetterT>::InstanceType>;
		using ValueType = ConditionalT<IsNullPointerV<GetterT>, typename PropertyMemberPointerHelper<SetterT>::ValueType, typename PropertyMemberPointerHelper<GetterT>::ValueType>;
		
	public:
		TProperty(FString Name_, GetterT Getter_, SetterT Setter_) : Name(Name_), Getter(Getter_), Setter(Setter_)
		{

		}

		EPropertyFlags GetPropertyFlags() const override { return PropertyFlags; }
		FType GetPropertyType() const override { return TypeOf<ValueType>(); }

		void SetValue(void * Instance, const FVariant & Variable) const override
		{
			if constexpr (IsMemberFunctionPointerV<SetterT>)
			{
				InstanceType * This = static_cast<InstanceType *>(Instance);
				(This->*Setter)(Variable.Get<ValueType>());
			}
			else if constexpr (IsMemberObjectPointerV<SetterT>)
			{
				InstanceType * This = static_cast<InstanceType *>(Instance);
				This->*Setter = Variable.Get<ValueType>();
			}
			else {}
		}

		EPropertyFlags PropertyFlags = EPropertyFlag::None;
		FString Name;
		GetterT Getter;
		SetterT Setter;
	};

	template<typename T>
	class TNativeType : public INativeType
	{
	public:
		TNativeType() : INativeType(typeid(T))
		{
			TypeFlags.Set(ETypeFlag::Basic, IsIntegralV<T> || IsFloatingPointV<T>);
			TypeFlags.Set(ETypeFlag::Abstract, IsAbstractV<T>);
		}

		FStringView GetName() const override { return FStringView(reinterpret_cast<const Char *>(typeid(T).name())); }
		ETypeFlags GetTypeFlags() const override { return TypeFlags; }
		void Construct(void * Pointer) const override
		{
			if constexpr (!IsAbstractV<T>)
				new (Pointer) T();
		}
		void Destruct(void * Pointer) const override { static_cast<T *>(Pointer)->~T(); }
		uintx_t GetSize() const override { return sizeof(T); }
		uintx_t GetAlignment() const override { return alignof(T); }

		void SetParent(const INativeType * Type) const override { Parent = Type; }
		const INativeType * GetParent() const override { return Parent; }

		void AppendChild(const INativeType * Another) const override
		{
			if (!Child)
				Child = Another;
			else
			{
				const INativeType * Current = Child;
				while (Current->GetNextSibling())
					Current = Current->GetNextSibling();
				Current->SetNextSibling(Another);
			}
		}
		const INativeType * GetFirstChild() const override { return Child; }

		void SetNextSibling(const INativeType * NextSiblingIn) const override { NextSibling = NextSiblingIn; }
		const INativeType * GetNextSibling() const { return NextSibling; }

		ETypeFlags TypeFlags = ETypeFlag::None;
		mutable const INativeType * Parent = nullptr;
		mutable const INativeType * Child = nullptr;
		mutable const INativeType * NextSibling = nullptr;

	public:
		FStringView Format(const FVariant & Variant) const
		{
			if constexpr (IsAbstractV<T>)
				return FStringView::Empty;
			else
				return TConverter<T>().ToString(Variant.Get<T>());
		}
		FVariant Format(FStringView String) const
		{
			if constexpr (IsAbstractV<T>)
				return FVariant();
			else
				return FVariant(TConverter<T>().FromString(String));
		}
		
	public:
		FVariant CreateValue() const override
		{
			FVariant Variable;
			Variable.Create<T>();
			return Variable;
		}

		const IProperty * GetProperty(FStringView Name) const
		{
			auto PropertyIter = Properties.Find(Name);
			if (PropertyIter == Properties.End())
				return Parent ? Parent->GetProperty(Name) : nullptr;
			return PropertyIter->Second.Get();;
		}

		template<typename SetterT, typename GetterT>
		void RegisterProperty(FStringView Name, GetterT && Getter, SetterT && Setter)
		{
			static_assert(IsMemberPointerV<GetterT> || IsNullPointerV<GetterT>);
			static_assert(IsMemberPointerV<SetterT> || IsNullPointerV<SetterT>);
			Properties.InsertOrAssign(Name, MakeRefer<TProperty<GetterT, SetterT>>(Name, Getter, Setter));
		}
		
	private:
		TMap<FString, TReferPtr<IProperty>> Properties;
	};

	template<typename T>
	void Reflection(TNativeType<T> & NativeType)
	{
		
	}
}