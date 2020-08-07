#pragma once

#include "Types.h"

namespace XE
{
    template <typename T>
	FORCEINLINE T AlignUp(T Value, size_t AlignmentMask)
    {
        return (T)(((size_t)Value + AlignmentMask) & ~AlignmentMask);
    }

    template <typename T>
	FORCEINLINE T AlignDown(T Value, size_t AlignmentMask)
    {
        return (T)((size_t)Value & ~AlignmentMask);
    }

    template<typename T>
    FORCEINLINE T Abs(const T & Value)
    {
        return Value < 0 ? -Value : Value;
    }

    template<typename FromT, typename ToT>
    FORCEINLINE ToT Round(const FromT & Value)
    {
        return static_cast<ToT>(Value + FromT(0.5));
    }

    template<typename T>
    FORCEINLINE int8_t RoundI8(const T & Value)
    {
        return Round<T, int8_t>(Value);
    }

    template<typename T>
    FORCEINLINE uint8_t RoundU8(const T & Value)
    {
        return Round<T, uint8_t>(Value);
    }

    template<typename T>
    FORCEINLINE int16_t RoundI16(const T & Value)
    {
        return Round<T, int16_t>(Value);
    }

    template<typename T>
    FORCEINLINE uint16_t RoundU16(const T & Value)
    {
        return Round<T, uint16_t>(Value);
    }

    template<typename T>
    FORCEINLINE int32_t RoundI32(const T & Value)
    {
        return Round<T, int32_t>(Value);
    }

    template<typename T>
    FORCEINLINE uint32_t RoundU32(const T & Value)
    {
        return Round<T, uint32_t>(Value);
    }

    template<typename T>
    FORCEINLINE int64_t RoundI64(const T & Value)
    {
        return Round<T, int64_t>(Value);
    }

    template<typename T>
    FORCEINLINE uint64_t RoundU64(const T & Value)
    {
        return Round<T, uint64_t>(Value);
    }

    template<typename T>
    FORCEINLINE float32_t RoundF32(const T & Value)
    {
        return Round<T, float32_t>(Value);
    }

    template<typename T>
    FORCEINLINE float64_t RoundF64(const T & Value)
    {
        return Round<T, float64_t>(Value);
    }

    template<Integral T>
    FORCEINLINE T Round(const T & Lhs, const T & Rhs)
    {
        return Lhs / Rhs;
    }

    template<Integral T>
    FORCEINLINE T RoundUp(const T & Lhs, const T & Rhs)
    {
        return (Lhs + Rhs  - 1) / Rhs;
    }

    template<Integral T>
    FORCEINLINE void SetBit(T & Value, size_t Index, bool Bit)
    {
        if (Bit)
            Value |= (T(1) << Index);
        else
            Value &= ~(T(1) << Index);
    }

    template<Integral T>
    FORCEINLINE void SetFlag(T & Value, const T & Flag, bool Set)
    {
        if (Set)
            Value |= Flag;
        else
            Value &= ~Flag;
    }

    template<Integral T>
    FORCEINLINE bool GetFlag(T & Value, const T & Flag)
    {
        return (Value & Flag) == Flag;
    }

    template<Integral T>
    FORCEINLINE bool GetBit(const T & Value, size_t Index)
    {
         return !!(Value & (T(1) << Index));
    }

    FORCEINLINE bool FloatEqual(float32_t FloatA, float32_t FloatB)
    {
        return Abs(FloatA - FloatB) < FLT_EPSILON;
    }

    FORCEINLINE constexpr uint32_t MakeFourCC(const char(&fourcc)[5])
    {
        return (static_cast<uint32_t>(fourcc[0])) |
            (static_cast<uint32_t>(fourcc[1]) << 8) |
            (static_cast<uint32_t>(fourcc[2]) << 16) |
            (static_cast<uint32_t>(fourcc[3]) << 24);
    }

    FORCEINLINE constexpr uint32_t MakeFourCC(char a, char b, char c, char d)
    {
        return (static_cast<uint32_t>(a)) |
            (static_cast<uint32_t>(b) << 8) |
            (static_cast<uint32_t>(c) << 16) |
            (static_cast<uint32_t>(d) << 24);
    }

    template<typename T, typename = EnableIfT<IsIntegralV<T>>>
    FORCEINLINE constexpr T High(T Value)
    {
        if constexpr (sizeof(T) == 8)
        {
            uint64_t UInt64Value = ValueAs<uint64_t, T>(Value);
            return T((UInt64Value >> 32) & 0xFFFFFFFFui64);
        }
        else if constexpr (sizeof(T) == 4)
        {
            uint32_t UInt32Value = ValueAs<uint32_t, T>(Value);
            return T((UInt32Value >> 16) & 0xFFFFu);
        }
        else if constexpr (sizeof(T) == 2)
        {
            uint16_t UInt16Value = ValueAs<uint16_t, T>(Value);
            return T((UInt16Value >> 8) & 0xFF);
        }
        else if constexpr (sizeof(T) == 1)
        {
            uint8_t UInt8Value = ValueAs<uint8_t, T>(Value);
            return T((UInt8Value >> 4) & 0xF);
        }
        else
        {
            static_assert(false, "Unsupported intergal value.");
            return {};
        }
    }

    template<typename T, typename = EnableIfT<IsIntegralV<T>>>
    FORCEINLINE constexpr T Low(T Value)
    {
        if constexpr (sizeof(T) == 8)
        {
            uint64_t UInt64Value = ValueAs<uint64_t, T>(Value);
            return T(UInt64Value & 0xFFFFFFFFui64);
        }
        else if constexpr (sizeof(T) == 4)
        {
            uint32_t UInt32Value = ValueAs<uint32_t, T>(Value);
            return T(UInt32Value & 0xFFFFu);
        }
        else if constexpr (sizeof(T) == 2)
        {
            uint16_t UInt16Value = ValueAs<uint16_t, T>(Value);
            return T(UInt16Value & 0xFF);
        }
        else if constexpr (sizeof(T) == 1)
        {
            uint8_t UInt8Value = ValueAs<uint8_t, T>(Value);
            return T(UInt8Value & 0xF);
        }
        else
        {
            static_assert(false, "Unsupported intergal value.");
            return {};
        }
    }

    template<typename OutT, typename T, typename = EnableIfT<IsNoneBoolIntegralV<T>>>
    FORCEINLINE constexpr OutT HighAs(T Value)
    {
        return OutT(ValueAs<ConditionalT<IsSignedV<OutT>, MakeSignedT<T>, MakeUnSigned<T>>, T>(High(Value)));
    }

    template<typename OutT, typename T, typename = EnableIfT<IsNoneBoolIntegralV<T>>>
    FORCEINLINE constexpr OutT LowAs(T Value)
    {
        return OutT(ValueAs<ConditionalT<IsSignedV<OutT>, MakeSignedT<T>, MakeUnSigned<T>>, T>(Low(Value)));
    }

    template<typename ContainerT>
    constexpr auto Size(const ContainerT & Container) -> decltype(Container.Size()) {
        return Container.Size();
    }

    template <typename ElementT, size_t ContainerSize>
    constexpr size_t Size(const ElementT(&)[ContainerSize]) noexcept {
        return ContainerSize;
    }

    template<typename ValueT>
    constexpr const ValueT & Min(const ValueT & Value) noexcept
    {
        return Value;
    }

    template<typename ValueT, typename ...ArgsT>
    constexpr const ValueT & Min(const ValueT & Left, const ValueT & Right, ArgsT && ...Args) noexcept
    {
        return Min((Left < Right ? Left : Right), Forward<ArgsT>(Args)...);
    }

    template<typename ValueT>
    constexpr const ValueT & Max(const ValueT & Value) noexcept
    {
        return Value;
    }

    template<typename ValueT, typename ...ArgsT>
    constexpr const ValueT & Max(const ValueT & Left, const ValueT & Right, ArgsT && ...Args) noexcept
    {
        return Max((Left < Right ? Right : Left), Forward<ArgsT>(Args)...);
    }

    template<IterAble ContainerT>
    constexpr const auto & Max(const ContainerT & Container)
    {
        auto IterBegin = Container.Begin();
        auto IterEnd = Container.Begin();
        if (IterBegin == IterEnd)
            return {};

        auto Iter = IterBegin;
        ++IterBegin;
        while (IterBegin != IterEnd)
        {
            if (*IterBegin > * Iter)
                Iter = IterBegin;
            ++IterBegin;
        }
        return *Iter;
    }

    template<IterAble ContainerT>
    constexpr const auto & Min(const ContainerT & Container)
    {
        auto IterBegin = Container.Begin();
        auto IterEnd = Container.Begin();
        if (IterBegin == IterEnd)
            return {};

        auto Iter = IterBegin;
        ++IterBegin;
        while (IterBegin != IterEnd)
        {
            if (*IterBegin < *Iter)
                Iter = IterBegin;
            ++IterBegin;
        }
        return *Iter;
    }

    template<typename ValueT>
    constexpr const ValueT & Clamp(const ValueT & Value, const ValueT & ValueMin, const ValueT & ValueMax) noexcept
    {
        if (Value < ValueMin)
            return ValueMin;
        if (ValueMax < Value)
            return ValueMax;
        return Value;
    }

    template<typename ValueT>
    constexpr bool IsBetween(const ValueT & Value, const ValueT & ValueMin, const ValueT & ValueMax) noexcept
    {
        return ValueMin <= Value && Value <= ValueMax;
    }

    template<typename ValueT>
    constexpr ValueT Saturate(const ValueT & Value)
    {
        return Value < ValueT(0) ? ValueT(0) : (ValueT(1) < Value ? ValueT(1) : Value);
    }

    template<typename ValueT, typename InterT>
    constexpr ValueT Lerp(const ValueT & ValueMin, const ValueT & ValueMax, const InterT & Inter)
    {
        return ValueMin + (ValueMax - ValueMin) * Inter;
    }

    template<typename ...ArgsT>
    auto CompareJoin(ArgsT &...Args);

    template<typename LhsT, typename RhsT, typename ...ArgsT>
    auto CompareJoin(const LhsT & Lhs, const RhsT & Rhs, ArgsT &&...Args)
    {
        static_assert(sizeof...(ArgsT) % 2 == 0, "ArgsT must be even");
        if (auto Comp = Lhs <=> Rhs; Comp != 0)
            return Comp;
        return Compare(Forward<ArgsT>(Args)...);
    }

    template<typename T>
    auto Compare(const T & Lhs, const T & Rhs)
    {
        return Lhs <=> Rhs;
    }

    template<Enum EnumT>
    auto Compare(const EnumT & Lhs, const EnumT & Rhs)
    {
        return UnderlyingTypeT<EnumT>(Lhs) <=> UnderlyingTypeT<EnumT>(Rhs);
    }

    template<typename ValueT>
    void Zero(ValueT * First, const ValueT * Last)
    {
        if constexpr (sizeof(ValueT) == 1 && IsTriviallyV<ValueT>)
        {
            std::memset(First, 0, static_cast<size_t>(Last - First));
        }
        else
        {
            while (First != Last)
            {
                *First = 0;
                ++First;
            }
        }
    }

    template<typename ValueT>
    void Fill(ValueT * First, const ValueT * Last, const ValueT & Value)
    {
        if constexpr (sizeof(ValueT) == 1 && IsTriviallyV<ValueT>)
        {
            std::memset(First, static_cast<unsigned char>(Value), static_cast<size_t>(Last - First));
        }
        else
        {
            while (First != Last)
            {
                *First = Value;
                ++First;
            }
        }
    }

    template<IterAble ContainerT, typename ValueT>
    void Fill(ContainerT & Container, const ValueT & Value)
    {
        for (auto Iter = Container.Begin(); Iter != Container.End(); ++Iter)
            *Iter = Value;
    }

    template<typename IterT, typename CondT>
    bool AllOf(IterT IterBegin, IterT IterEnd, CondT && Cond)
    {
        while (IterBegin != IterEnd)
        {
            if (!Cond(*IterBegin))
                return false;
            ++IterBegin;
        }
        return true;
    }

    template<IterAble ContainerT, typename CondT>
    bool AllOf(const ContainerT & Container, CondT && Cond)
    {
        for (auto Iter = Container.Begin(); Iter != Container.End(); ++Iter)
        {
            if (!Cond(*Iter))
                return false;
        }
        return true;
    }

    template <typename T>
    constexpr RemoveReferenceT<T> && Move(T && Arg) noexcept
    {
        return static_cast<RemoveReferenceT<T> &&>(Arg);
    }

    template <typename T, typename = EnableIfT<IsMoveConstructibleV<T> && IsMoveAssignableV<T>, int>>
    void Swap(T & Left, T & Right) noexcept(IsNoThrowMoveConstructibleV<T> && IsNoThrowMoveAssignableV<T>)
    {
        T Temp = Move(Left);
        Left = Move(Right);
        Right = Move(Temp);
    }

    template <typename IterT, typename CompareT>
    IterT FindIf(IterT First, const IterT Last, CompareT && Compare)
    {
        while (First != Last)
        {
            if (Compare(*First))
                break;

            ++First;
        }

        return First;
    }

    template <IterAble ContainerT, typename CompareT>
    auto FindIf(const ContainerT & Container, CompareT && Compare)
    {
        return FindIf(Container.Begin(), Container.End(), Forward<CompareT>(Compare));
    }

    template <typename IterT, typename CompareT>
    IterT FindIfNot(IterT First, const IterT Last, CompareT Compare)
    {
        while (First != Last)
        {
            if (!Compare(*First))
                break;

            ++First;
        }

        return First;
    }
}