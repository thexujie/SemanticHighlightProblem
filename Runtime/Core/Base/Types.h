#pragma once

#include "Config.h"

namespace XE
{
	using Char = char;
#define Str(xxx) xxx

	//using Char = char8_t;
	//#define Str(xxx) u8##xxx
	//
	using long_t = long;
	using ulong_t = unsigned long;
	using float32_t = float;
	using float64_t = double;
	using ptr_t = void *;
	
#if XE_X64
	using intx_t =  int64_t;
	using uintx_t = uint64_t;
	using intx_h = int32_t;
	using uintx_h = uint32_t;
	using addr_t = uint64_t;
#else
	using intx_t = int32_t;
	using uintx_t = uint32_t;
	using intx_h = int16_t;
	using uintx_h = uint16_t;
	using addr_t = uint32_t;
#endif
	using byte_t = uint8_t;
	using diff_t = intx_t;
	
	constexpr addr_t NullAddress = addr_t(0);
	constexpr size_t NullPos = size_t(-1);
	constexpr size_t NullIndex = size_t(-1);
	constexpr uint16_t NullIndexU16 = uint16_t(-1);
	constexpr uint32_t NullIndexU32 = uint32_t(-1);
	constexpr uint64_t NullIndexU64 = uint64_t(-1);
	constexpr uint32_t InfinityU32 = 0x7f800000;
	const float32_t NanF = std::nanf("0");
	const float64_t NanF64 = std::nan("0");

	const int16_t Int16Min = std::numeric_limits<int16_t>::min();
	const int16_t Int16Max = std::numeric_limits<int16_t>::max();
	const uint16_t UInt16Min = std::numeric_limits<uint16_t>::min();
	const uint16_t UInt16Max = std::numeric_limits<uint16_t>::max();
	const int32_t Int32Min = std::numeric_limits<int32_t>::min();
	const int32_t Int32Max = std::numeric_limits<int32_t>::max();
	const uint32_t UInt32Min = std::numeric_limits<uint32_t>::min();
	const uint32_t UInt32Max = std::numeric_limits<uint32_t>::max();
	const int64_t Int64Min = std::numeric_limits<int64_t>::min();
	const int64_t Int64Max = std::numeric_limits<int64_t>::max();
	const uint64_t UInt64Min = std::numeric_limits<uint64_t>::min();
	const uint64_t UInt64Max = std::numeric_limits<uint64_t>::max();

#if XE_X64
	const uintx_t IntXMin = Int64Min;
	const uintx_t IntXMax = Int64Max;
	const uintx_t UIntXMin = UInt64Min;
	const uintx_t UIntXMax = UInt64Max;
#else
	const uintx_t IntXMin = Int32Min;
	const uintx_t IntXMax = Int32Max;
	const uintx_t UIntXMin = UInt32Min;
	const uintx_t UIntXMax = UInt32Max;
#endif

	const float32_t Float32Min = 1.175494351e-38f;
	const float32_t Float32Max = 3.402823466e+38f;
	const float32_t Float32Epslion = 1.192092896e-07f;
	const float32_t Float32PI = 3.141592654f;

	const float64_t Float64Min = 2.2250738585072014e-308;
	const float64_t Float64Max = 1.7976931348623158e+308;
	const float64_t Float64Epslion = 2.2204460492503131e-016;
	const float64_t Float64PI = 3.14159265358979323846;

	template<typename T> const T FloatEpslion = 0;
	template<> const float32_t FloatEpslion<float32_t> = Float32Epslion;
	template<> const float64_t FloatEpslion<float64_t> = Float64Epslion;
	
	struct FStrongOrdering
	{
		static FStrongOrdering Less;
		static FStrongOrdering Greater;
		static FStrongOrdering Equal;
	};
	
	// ---------------- EnableIf
	template <bool Test, typename T = void>
	struct EnableIf {};

	template <typename T>
	struct EnableIf<true, T>
	{
		using Type = T;
	};

	template <bool Test, typename T = void>
	using EnableIfT = typename EnableIf<Test, T>::Type;
	
	// ---------------- NullPtrT
	using NullPtrT = decltype(__nullptr);

	// ---------------- VoidT
	template <typename... Types>
	using VoidT = void;

	//----------------- Add
	template<typename T>
	struct AddLValueReference { using Type = T &; };

	template<>
	struct AddLValueReference<void> { using Type = void; };

	template<typename T>
	using AddLValueReferenceT = typename AddLValueReference<T>::Type;

	template<typename T>
	struct AddRValueReference { using Type = T &&; };

	template<>
	struct AddRValueReference<void> { using Type = void; };

	template<typename T>
	using AddRValueReferenceT = typename AddRValueReference<T>::Type;

	template<bool Cond, typename TrueT, typename FalseT>
	struct Conditional { typedef TrueT Type; };

	template<typename TrueT, typename FalseT>
	struct Conditional<false, TrueT, FalseT> { typedef FalseT Type; };

	template<bool Cond, typename TrueT, typename FalseT>
	using ConditionalT = typename Conditional<Cond, TrueT, FalseT>::Type;

	template<typename T>
	AddRValueReferenceT<T> DeclVal() noexcept;
	
	// ---------------- IntegralConstant
	template <typename T, T ConstantValue>
	struct IntegralConstant
	{
		using ValueType = T;
		using Type = IntegralConstant;

		static constexpr T Value = ConstantValue;

		constexpr operator ValueType() const noexcept { return Value; }
		constexpr ValueType operator()() const noexcept { return Value; }
	};

	// ---------------- BoolConstant
	template <bool ConstantValue>
	using BoolConstant = IntegralConstant<bool, ConstantValue>;

	using TrueType = BoolConstant<true>;
	using FalseType = BoolConstant<false>;

	template <typename T>
	constexpr bool TrueTypeV = BoolConstant<true>::Value;

	template <typename T>
	constexpr bool FalseTypeV = BoolConstant<false>::Value;

	// ---------------- Types
	template<typename BaseT, typename DerivedT>
	inline constexpr bool IsBaseOfV = __is_base_of(BaseT, DerivedT);

	template<typename BaseT, typename DerivedT>
	using IsBaseOf = BoolConstant<IsBaseOfV<BaseT, DerivedT>>;
	
	// ---------------- Types
	template<typename T> constexpr bool IsUnionV = __is_union(T);
	template<typename T> constexpr bool IsEnumV = __is_enum(T);
	template<typename T> constexpr bool IsClassV = __is_class(T);
	template<typename T> constexpr bool IsEnumClassV = __is_class(T) && __is_enum(T);

	template <typename T>
	inline constexpr bool IsAbstractV = __is_abstract(T);
	template <typename T>
	struct IsAbstract : BoolConstant<IsAbstractV<T>> {};
	template <typename T>
	concept Abstract = IsAbstractV<T>;
	
	template <typename T>
	inline constexpr bool IsFinalV = __is_final(T);
	template <typename T>
	struct IsFinal : BoolConstant<IsFinalV<T>> {};

	template<typename T, bool = IsEnumV<T>>
	struct UnderlyingType
	{
		using Type = __underlying_type(T);
	};
	template<typename T>
	using UnderlyingTypeT = typename UnderlyingType<T>::Type;

	template <typename FromT, typename ToT>
	inline constexpr bool IsConvertibleToV = __is_convertible_to(FromT, ToT);
	
	template <typename FromT, typename ToT>
	concept ConvertibleTo = __is_convertible_to(FromT, ToT) && requires(AddRValueReferenceT<FromT>(&ConvertFun)())
	{
		static_cast<FromT>(ConvertFun());
	};
	
	template<typename T>
	concept EqualAble = requires(const T & Lhs, const T & Rhs) { {Lhs == Rhs}->ConvertibleTo<bool>; };
	

	// ---------------- Disjunction
	template<typename...> struct Disjunction : FalseType { };
	template<typename T> struct Disjunction<T> : T { };
	template<typename T, typename... ArgsT>
	struct Disjunction<T, ArgsT...> : ConditionalT<bool(T::Value), T, Disjunction<ArgsT...>> { };

	template<typename ...ArgsT>
	inline constexpr bool DisjunctionV = Disjunction<ArgsT...>::Value;

	// ---------------- IsSame
	template <typename, typename>
	constexpr bool IsSameV = false;
	template <typename T>
	constexpr bool IsSameV<T, T> = true;

	template <typename T1, typename T2>
	struct IsSame : BoolConstant<IsSameV<T1, T2>> {};

	// ---------------- IsAnyOfV
	template<typename T, typename... ArgsT>
	inline constexpr bool IsAnyOfV = DisjunctionV<IsSame<T, ArgsT>...>;

	// ---------------- IsTrivially

	template <typename T>
	inline constexpr bool IsDefaultConstructibleV = __is_constructible(T);
	template <typename T>
	struct IsDefaultConstructible : BoolConstant<IsDefaultConstructibleV<T>> {};
	template <typename T>
	concept DefaultConstructible = IsDefaultConstructibleV<T>;

	template <typename T>
	inline constexpr bool IsMoveConstructibleV = __is_constructible(T, T);

	template <typename T>
	inline constexpr bool IsMoveAssignableV = __is_assignable(AddLValueReferenceT<T>, T);

	template <typename T>
	inline constexpr bool IsNoThrowMoveConstructibleV = __is_nothrow_constructible(T, T);

	template <typename T>
	inline constexpr bool IsNoThrowMoveAssignableV = __is_nothrow_assignable(AddLValueReferenceT<T>, T);

	// ---------------- IsTrivially
	template <typename T>
	inline constexpr bool IsTriviallyConstructibleV = __is_trivially_constructible(T);

	template <typename T>
	inline constexpr bool IsTriviallyCopyableV = __is_trivially_copyable(T);

	template<typename T>
	concept TriviallyCopyable = IsTriviallyCopyableV<T>;

	template <typename T>
	inline constexpr bool IsTriviallyV = __is_trivially_constructible(T) && __is_trivially_copyable(T);

	template <typename _Ty>
	inline constexpr bool IsTriviallyMoveConstructibleV = __is_trivially_constructible(_Ty, _Ty);

	// ---------------- Negation
	template <typename Trait>
	struct Negation : BoolConstant<!static_cast<bool>(Trait::Value)> {};

	template <typename Trait>
	inline constexpr bool NegationV = Negation<Trait>::Value;


	// ---------------- RemoveReference
	template <typename T>
	struct RemoveReference
	{
		using Type = T;
	};

	template <typename T>
	struct RemoveReference<T &>
	{
		using Type = T;
	};

	template <typename T>
	struct RemoveReference<T &&>
	{
		using Type = T;
	};

	template <class T>
	using RemoveReferenceT = typename RemoveReference<T>::Type;

	// ---------------- RemoveCV
	template <typename T> struct RemoveCV { typedef T Type; };
	template <typename T> struct RemoveCV<const T> { typedef T Type; };
	template <typename T> struct RemoveCV<volatile T> { typedef T Type; };
	template <typename T> struct RemoveCV<const volatile T> { typedef T Type; };

	template <typename T>
	using RemoveCVT = typename RemoveCV<T>::Type;

	template <typename T>
	using RemoveCVRefT = RemoveCVT<RemoveReferenceT<T>>;


	// ---------------- RemoveExtent
	template <typename T> struct RemoveExtent { typedef T Type; };
	template <typename T> struct RemoveExtent<T[]> { typedef T Type; };
	template <typename T, size_t N> struct RemoveExtent<T[N]> { typedef T Type; };
	template <typename T>
	using RemoveExtentT = typename RemoveExtent<T>::Type;

	// ---------------- Add CVRef
	template <typename... TypesT>
	void Swallow(TypesT &&...) {}

	template <typename T>
	struct AddConst { using Type = const T; };

	template <typename T>
	using AddConstT = typename AddConst<T>::Type;

	template <typename T>
	struct AddVolatile { using Type = volatile T; };

	template <typename T>
	using AddVolatileT = typename AddVolatile<T>::Type;

	template <typename T>
	struct AddCV { using Type = const volatile T; };

	template <typename T>
	using AddCVT = typename AddCV<T>::Type;

	// ---------------- AddPointer
	template <typename T, typename = void>
	struct AddPointer
	{
		using Type = T;
	};

	template <typename T>
	struct AddPointer<T, VoidT<RemoveReferenceT<T> *>>
	{
		using Type = RemoveReferenceT<T> *;
	};

	template <typename T>
	using AddPointerT = typename AddPointer<T>::Type;

	template <typename T>
	struct RemovePointer { using Type = T; };

	template <typename T>
	struct RemovePointer<T *> { using Type = T; };

	template <typename T>
	struct RemovePointer<T * const> { using Type = T; };

	template <typename T>
	struct RemovePointer<T * volatile> { using Type = T; };

	template <typename T>
	struct RemovePointer<T * const volatile> { using Type = T; };

	template <typename T>
	using RemovePointerT = typename RemovePointer<T>::Type;
	
	// ---------------- IsPointer
	template <typename>
	inline constexpr bool IsPointerV = false;

	template <typename T>
	inline constexpr bool IsPointerV<T *> = true;

	template <typename T>
	inline constexpr bool IsPointerV<T * const> = true;

	template <typename T>
	inline constexpr bool IsPointerV<T * volatile> = true;

	template <typename T>
	inline constexpr bool IsPointerV<T * const volatile> = true;

	template <typename T>
	struct IsPointer : BoolConstant<IsPointerV<T>> {};
	
	// ---------------- IsNullPointer
	template <typename T>
	inline constexpr bool IsNullPointerV = IsSameV<RemoveCVT<T>, NullPtrT>;
	
	template <typename T>
	using IsNullPointer = BoolConstant<IsNullPointerV<T>>;
	// 
	// ---------------- IsIntegral
	template <typename T>
	inline constexpr bool IsCharacterV = IsAnyOfV<RemoveCVT<T>, char, wchar_t, char8_t, char16_t, char32_t>;
	
	template <typename T>
	inline constexpr bool IsIntegralV = IsAnyOfV<RemoveCVT<T>, bool, char, signed char, unsigned char,
		wchar_t,
#ifdef __cpp_char8_t
		char8_t,
#endif // __cpp_char8_t
		char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;

	template <typename T>
	inline constexpr bool IsFloatingPointV = IsAnyOfV<RemoveCVT<T>, float, double, long double>;

	template <typename T>
	inline constexpr bool IsArithmeticV = IsIntegralV<T> || IsFloatingPointV<T>;

	template <typename T>
	inline constexpr bool IsNoneBoolIntegralV = IsIntegralV<T> && !IsSameV<T, bool>;
	
	// ---------------- IsSigned
	inline namespace Private
	{
		template<typename T, bool = IsIntegralV<T>>
		struct IsSignedPrivate
		{
			using SingedType = BoolConstant<T(-1) < T(0)> ;
			using UnSingedType = BoolConstant<!(T(-1) < T(0))>;
		};
		
		template<typename T>
		struct IsSignedPrivate<T, false>
		{
			using SingedType = BoolConstant<IsFloatingPointV<T>>;
			using UnSingedType = BoolConstant<false>;
		};

		template<>
		struct IsSignedPrivate<bool, true>
		{
			using SingedType = BoolConstant<false>;
			using UnSingedType = BoolConstant<true>;
		};
	}

	template<typename T>
	struct IsSigned : IsSignedPrivate<T>::SingedType {};

	template<typename T>
	inline constexpr bool IsSignedV = IsSigned<T>::Value;

	template<typename T>
	concept Signed = IsSignedV<T>;
	
	template<typename T>
	concept UnSigned = IsSignedV<T>;
	
	inline namespace Private
	{
		template<size_t Size>
		struct MakeSignedPrivate;

		template<>
		struct MakeSignedPrivate<1>
		{
			using SingedType = int8_t;
			using UnSingedType = uint8_t;
		};

		template<>
		struct MakeSignedPrivate<2>
		{
			using SingedType = int16_t;
			using UnSingedType = uint16_t;
		};


		template<>
		struct MakeSignedPrivate<4>
		{
			using SingedType = int32_t;
			using UnSingedType = uint32_t;
		};


		template<>
		struct MakeSignedPrivate<8>
		{
			using SingedType = int64_t;
			using UnSingedType = uint64_t;
		};
	}

	template<typename T, typename = EnableIfT<IsIntegralV<T> && !IsSameV<T, bool>>>
	struct MakeSigned { using Type = typename MakeSignedPrivate<sizeof(T)>::SingedType; };

	template<typename T, typename = EnableIfT<IsIntegralV<T> && !IsSameV<T, bool>>>
	struct MakeUnSigned { using Type = typename MakeSignedPrivate<sizeof(T)>::UnSingedType; };

	template<typename T, typename = EnableIfT<IsIntegralV<T> && !IsSameV<T, bool>>>
	using MakeSignedT = typename MakeSigned<T>::Type;

	template<typename T, typename = EnableIfT<IsIntegralV<T> && !IsSameV<T, bool>>>
	using MakeUnSignedT = typename MakeUnSigned<T>::Type;

	template<typename T>
	concept OppositeAble = requires (T Value) { -Value; };
	
	template<typename T>
	inline constexpr bool IsOppositeAbleV = false;
	template<OppositeAble T>
	inline constexpr bool IsOppositeAbleV<T> = true;
	
	template<typename T>
	struct IsOppositeAble {using Type = decltype(-(DeclVal<T>())); };

	template<OppositeAble T>
	using IsOppositeAbleT = typename IsOppositeAble<T>::Type;
	
	// ---------------- IntegerSequence
	template <class T, T... Values>
	struct IntegerSequence
	{
		static_assert(IsIntegralV<T>, "IntegerSequence<T, I...> requires T to be an integral type.");
		using ValueType = T;

		static constexpr size_t Size() noexcept { return sizeof...(Values); }
	};

	template <class T, T Size>
	using MakeIntegerSequence = __make_integer_seq<IntegerSequence, T, Size>;

	template <size_t... Values>
	using IndexSequence = IntegerSequence<size_t, Values...>;

	template <size_t Size>
	using MakeIndexSequence = MakeIntegerSequence<size_t, Size>;

	template<typename ...TypesT>
	using IndexSequenceFor = MakeIndexSequence<sizeof...(TypesT)>;

	// ---------------- IsFunction
	template <typename>
	struct IsFunction : public FalseType {};

	template <typename ReturnType, typename... ArgsType>
	struct IsFunction<ReturnType(ArgsType...)> : public TrueType {};

	template <typename ReturnType, typename... ArgsType>
	struct IsFunction<ReturnType(ArgsType..., ...)> : public TrueType {};

	template <class T>
	constexpr bool IsFunctionV = IsFunction<T>::Value;

	// ---------------- is_signed
	//template<typename T, typename = void>
	//struct IsSigned : FalseType { };

	//template<typename T, typename = EnableIfT<IsIntegralV<T>>>
	//struct IsSigned : FalseType { };

	// ---------------- IsArray
	template<typename>
	constexpr bool IsArrayV = false;

	template <typename T, size_t Length>
	constexpr bool IsArrayV<T[Length]> = true;

	template <typename T>
	constexpr bool IsArrayV<T[]> = true;

	template <typename T>
	struct IsArray : BoolConstant<IsArrayV<T>> {};

	// ---------------- IsMemberObjectPointer
	template<typename T>
	struct IsMemberFunctionPointer :FalseType { };

	template<typename ReturnT, typename InstanceT, typename ...ArgsT>
	struct IsMemberFunctionPointer<ReturnT(InstanceT:: *)(ArgsT...)> :TrueType
	{
		using InstanceType = InstanceT;
		using ValueType = ReturnT;
	};

	template <typename T>
	inline constexpr bool IsMemberFunctionPointerV = IsMemberFunctionPointer<RemoveCVT<T>>::Value;

	template <typename T, bool = IsMemberFunctionPointerV<T>>
	struct IsMemberObjectPointer : FalseType {};

	template <typename ObjectT, typename InstanceT>
	struct IsMemberObjectPointer<ObjectT InstanceT:: *, false> : TrueType
	{
		using InstanceType = InstanceT;
		using ValueType = ObjectT;
	};

	template <typename T>
	inline constexpr bool IsMemberObjectPointerV = IsMemberObjectPointer<RemoveCVT<T>>::Value;

	// STRUCT TEMPLATE is_member_pointer
	template <typename T>
	inline constexpr bool IsMemberPointerV = IsMemberFunctionPointerV<T> || IsMemberObjectPointerV <T>;

	template <typename T>
	using IsMemberPointer = ConditionalT<IsMemberFunctionPointerV<T>, IsMemberFunctionPointer<T>, IsMemberObjectPointer<T>>;
	

	// ---------------- Decay
	template <typename T>
	struct Decay
	{
		using NoneReferenceType = RemoveReferenceT<T>;
		using TypeInner = ConditionalT<IsFunctionV<NoneReferenceType>, AddPointerT<NoneReferenceType>, RemoveCVT<NoneReferenceType>>;
		using Type = ConditionalT<IsArrayV<NoneReferenceType>, AddPointerT<RemoveExtentT<NoneReferenceType>>, TypeInner>;
	};
	template <typename T>
	using DecayT = typename Decay<T>::Type;

	// STRUCT TEMPLATE is_lvalue_reference
	template <typename>
	constexpr bool IsLValueReferenceV = false;

	template <typename T>
	constexpr bool IsLValueReferenceV<T &> = true;

	template <typename T>
	constexpr T && Forward(RemoveReferenceT<T> & Arg) noexcept
	{
		return static_cast<T &&>(Arg);
	}

	template <typename T>
	constexpr T && Forward(RemoveReferenceT<T> && Arg) noexcept
	{
		static_assert(!IsLValueReferenceV<T>, "bad forward call");
		return static_cast<T &&>(Arg);
	}

	// ---------------- Conjunction
	template <bool FirstValue, typename FirstType, typename... RestTypes>
	struct ConjunctionHelper
	{
		using Type = FirstType;
	};

	template <class TrueT, typename NextT, typename... RestsT>
	struct ConjunctionHelper<true, TrueT, NextT, RestsT...>
	{
		using Type = typename ConjunctionHelper<NextT::Value, NextT, RestsT...>::type;
	};

	template <typename... Traits>
	struct Conjunction : TrueType {};

	template <typename FirstType, typename... RestTypes>
	struct Conjunction<FirstType, RestTypes...> : ConjunctionHelper<FirstType::Value, FirstType, RestTypes...>::Type
	{
	};

	template <typename... Traits>
	constexpr bool ConjunctionV = Conjunction<Traits...>::Value;


	// ---------------- IsComplete
	inline namespace Private
	{
		template <typename T, std::size_t = sizeof(T)>
		TrueType IsCompletePrivate(T *);

		FalseType IsCompletePrivate(...);
	}

	template<typename T>
	using IsComplete = decltype(IsCompletePrivate(std::declval<T *>()));

	template<typename T>
	constexpr bool IsCompleteV = IsComplete<T>::Value;

	template<typename T>
	concept Integral = IsIntegralV<T>;

	template<typename T>
	concept NoneBoolIntegral = IsNoneBoolIntegralV<T>;

	template<typename T>
	concept FloatingPoint = IsFloatingPointV<T>;

	template<typename T>
	concept Arithmetic = IsArithmeticV<T>;

	template<typename T>
	concept Enum = IsEnumV<T>;

	template<typename T>
	concept Class = IsClassV<T>;

	template<typename T>
	concept EnumClass = IsEnumClassV<T>;


	template<typename ContainerT>
	concept IterAble = requires (ContainerT Container)
	{
		Container.Begin();
		Container.End();
	};

	template<typename CharT>
	constexpr size_t Length(const CharT * String)
	{
		if (!String)
			return 0;

		size_t Result = 0;
		while (*String++)
			++Result;
		return Result;
	}

	template<typename CharT, typename IntergerT>
	IntergerT ToInterger(const CharT * String, size_t Size, int32_t Radis = 0)
	{
		if (Size == UIntXMax)
			Size = Length(String);

		while (Size && *String == ' ')
		{
			++String;
			--Size;
		}

		IntergerT sign = 1;
		if (String[0] == L'-')
		{
			sign = (IntergerT)-1;
			++String;
		}

		if (!Size || !String)
			return 0;

		const CharT * curr = String;
		if (Radis <= 0)
		{
			if (curr[0] == '0' && Size >= 2)
			{
				if (curr[1] == 'b' || curr[1] == 'B' || curr[1] == '0')
				{
					Radis = 2;
					curr += 2;
				}
				else if (curr[1] == 'x' || curr[1] == 'X')
				{
					Radis = 16;
					curr += 2;
				}
				else if (curr[1] == 'h' || curr[1] == 'H')
				{
					Radis = 8;
					curr += 2;
				}
				else if (curr[1] == 'd' || curr[1] == 'D')
				{
					Radis = 10;
					curr += 2;
				}
				else
					Radis = 10;
			}
			else
				Radis = 10;
		}

		IntergerT integer = 0;
		IntergerT number = 0;
		CharT ch = 0;

		while (Size--)
		{
			ch = *curr++;
			if (!ch)
				break;

			if (ch >= '0' && ch <= '9')
				number = ch - '0';
			else if (ch >= 'A' && ch <= 'Z')
				number = ch - 'A' + 10;
			else if (ch >= 'a' && ch <= 'z')
				number = ch - 'a' + 10;
			else
				break;

			if (number >= (IntergerT)Radis)
				break;

			integer = integer * (IntergerT)Radis + number;
		}
		return integer * sign;
	}

	template<typename CharT>
	bool TStringToBoolean(const CharT * String, size_t Length)
	{
		return false;
	}

	template<typename CharT, typename FloatingPointT>
	FloatingPointT ToFloatingPoint(const CharT * String, size_t Size = -1)
	{
		if (Size == UIntXMax)
			Size = Length(String);

		if (!Size)
			return FloatingPointT(0);

		const CharT * end = String + Size;
		bool sign_radix = true;
		bool sign_exp = true;
		// reading integer part.
		bool read_integer = true;
		// reading exp part.
		bool read_exp = false;

		// radix = integer + decimal.
		// integer part.
		FloatingPointT integer = 0;
		// decimal part.
		FloatingPointT decimal = 0;
		// exp part.
		int32_t exp = 0;
		// use to caculate decimal.
		FloatingPointT decimal_index = FloatingPointT(1) / FloatingPointT(10);

		if (String[0] == '-')
		{
			sign_radix = false;
			++String;
		}
		else {}

		while (String < end)
		{
			CharT ch = *String++;
			if (ch == '-')
			{
				if (read_exp)
					sign_exp = false;
				else
					break;
			}
			else if (ch == '.')
			{
				read_integer = false;
			}
			else if (ch == 'E' || ch == 'e')
			{
				read_integer = false;
				read_exp = true;
			}
			else if ('0' <= ch && ch <= '9')
			{
				int32_t number = (int32_t)(ch - '0');
				if (read_exp)
				{
					exp *= 10;
					exp += number;
				}
				else if (read_integer)
				{
					integer *= 10;
					integer += number;
				}
				else
				{
					decimal += number * decimal_index;
					decimal_index /= FloatingPointT(10);
				}
			}
			else
				break;
		}

		FloatingPointT result = (FloatingPointT)(integer + decimal);
		if (!sign_radix)
			result = -result;

		if (sign_exp)
		{
			for (int32_t cnt = 0; cnt < exp; ++cnt)
				result *= 10;
		}
		else
		{
			for (int32_t cnt = 0; cnt < exp; ++cnt)
				result /= FloatingPointT(10);
		}

		return result;
	}

	enum class EError : int
	{
		Generic = std::numeric_limits<int>::min(),
		Inner,
		Eof,
		Failed,
		Nullptr,
		Unreachable,
		OutOfMemory,
		OutOfBound,
		Args,
		Exists,
		Access,
		State,
		IO,
		Broken,

		Timeout,
		Cancel,

		BadData,
		BadFormat,

		NotNow,
		NotSupported,
		NotImplemented,
		NotFound,

		InvalidOperation,

		OK = 0,
		True,
		False,
		Pendding,
	};

	CORE_API const char * ErrorStr(EError err);

	CORE_API bool IsNan(float32_t Value);
	CORE_API bool IsNan(float64_t Value);
	
	template<typename T>
	struct TDeleterDefault
	{
		TDeleterDefault() = default;
		TDeleterDefault(const TDeleterDefault &) = default;
		TDeleterDefault & operator=(const TDeleterDefault &) = default;
		~TDeleterDefault() = default;

		FORCEINLINE void operator()(T * Ptr)
		{
			delete Ptr;
		}
	};
	template<typename T>
	struct TDeleterDefault<T[]>
	{
		TDeleterDefault() = default;
		TDeleterDefault(const TDeleterDefault &) = default;
		TDeleterDefault & operator=(const TDeleterDefault &) = default;
		~TDeleterDefault() = default;

		FORCEINLINE void operator()(T * Ptr)
		{
			delete[] Ptr;
		}
	};

	template <typename T>
	struct TEqual
	{
		constexpr bool operator()(const T & Left, const T & Right) const { return Left == Right; }
	};

	template <typename T>
	struct TLess
	{
		constexpr bool operator()(const T & Left, const T & Right) const { return Left < Right; }
	};

	class CORE_API IObject
	{
	public:
		IObject() = default;
		IObject(const IObject &) = default;
		IObject(IObject &&) noexcept = default;
		IObject & operator = (const IObject &) = default;
		virtual ~IObject() = default;
	};

	template<typename T>
	struct IsObject : BoolConstant<IsBaseOfV<IObject, T>> {};

	template<typename T>
	inline constexpr bool IsObjectV = IsObject<T>::Value;

	class CORE_API IRefer : public IObject
	{
	public:
		IRefer() = default;
		IRefer(const IRefer &) = default;
		IRefer(IRefer &&) noexcept = default;
		IRefer & operator = (const IRefer &) = default;
		virtual ~IRefer() = default;

		virtual uintx_t AddRef() = 0;
		virtual uintx_t Release() = 0;
	};
}
