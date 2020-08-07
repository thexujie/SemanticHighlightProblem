#pragma once

#include "Types.h"

namespace XE
{
    template<typename T>
    class TVec2
    {
    public:
        TVec2() = default;
        TVec2(const TVec2 &) = default;
        TVec2 & operator=(const TVec2 &) = default;
        ~TVec2() = default;
        auto operator <=>(const TVec2 & Another) const = default; // { return Compare(X, Another.X, Y, Another.Y); }

        constexpr TVec2(T InX, T InY) : X(InX), Y(InY) { }

        TVec2 Add(const TVec2 & Another) const
        {
            return TVec2(X + Another.X, Y + Another.Y);
        }

        void Clear()
        {
            Width = 0;
            Height = 0;
        }

        bool IsEmpty() const
        {
            if constexpr (IsFloatingPointV<T>)
                return IsNan(Width) || std::isnan(Height) || std::abs(Width) <= std::numeric_limits<T>::epsilon() || std::abs(Height) <= std::numeric_limits<T>::epsilon();
            else
                return Width == 0 || Height == 0;
        }

        //bool HasNan() const
        //{
        //    if constexpr (!std::is_arithmetic_v<T> || !std::is_floating_point_v<T>)
        //        return false;
        //    return std::isnan(X) || std::isnan(Y);
        //}

        //bool IsAllNan() const
        //{
        //    if constexpr (!std::is_arithmetic_v<T> || !std::is_floating_point_v<T>)
        //        return false;
        //    return std::isnan(X) && std::isnan(Y);
        //}

        //------------------------------------------------------- 四则运算
        template<typename = decltype(std::declval<T>() + std::declval<T>())>
        TVec2 operator +(const TVec2 & Another) const
        {
            return TVec2(X + Another.X, Y + Another.Y);
        }

        template<typename = decltype(std::declval<T>() - std::declval<T>())>
        TVec2 operator -(const TVec2 & Another) const
        {
            return TVec2(X - Another.X, Y - Another.Y);
        }

        template<typename = decltype(std::declval<T>() * std::declval<T>())>
        TVec2 operator *(const TVec2 & Another) const
        {
            return TVec2(X * Another.X, Y * Another.Y);
        }

        template<typename = decltype(std::declval<T>() / std::declval<T>())>
        TVec2 operator /(const TVec2 & Another) const
        {
            return TVec2(X / Another.X, Y / Another.Y);
        }

        template<typename = decltype(std::declval<T &>() += std::declval<T>())>
        TVec2 & operator +=(const TVec2 & Another)
        {
            X += Another.X;
            Y += Another.Y;
            return *this;
        }

        template<typename = decltype(std::declval<T &>() -= std::declval<T>())>
        TVec2 & operator -=(const TVec2 & Another)
        {
            X -= Another.X;
            Y -= Another.Y;
            return *this;
        }

        template<typename = decltype(std::declval<T &>() *= std::declval<T>())>
        TVec2 & operator *=(const TVec2 & Another)
        {
            X *= Another.X;
            Y *= Another.Y;
            return *this;
        }

        template<typename = decltype(std::declval<T &>() /= std::declval<T>())>
        TVec2 & operator /=(const TVec2 & Another)
        {
            X /= Another.X;
            Y /= Another.Y;
            return *this;
        }

        template<typename = decltype(std::declval<T>() * std::declval<T>())>
        TVec2 operator * (const T & Rate) const
        {
            return TVec2(X * Rate, Y * Rate);
        }

        template<typename = EnableIfT<!IsSameV<T, float32_t>>>
        TVec2 operator * (float32_t Rate) const
        {
            return TVec2(static_cast<T>(X * Rate), static_cast<T>(Y * Rate));
        }

        TVec2 operator / (const T & Rate)
        {
            return TVec2(X / Rate, Y / Rate);
        }

        template<typename = EnableIfT<!IsSameV<T, float32_t>>>
        TVec2 operator / (float32_t Rate) const
        {
            return TVec2(static_cast<T>(X / Rate), static_cast<T>(Y / Rate));
        }

        TVec2 & operator *=(const T & Rate)
        {
            X *= Rate;
            Y *= Rate;
            return *this;
        }

        template<typename = EnableIfT<!IsSameV<T, float32_t>>>
        TVec2 & operator *= (float32_t Rate)
        {
            X = static_cast<T>(X * Rate);
            Y = static_cast<T>(Y * Rate);
            return *this;
        }

        TVec2 & operator /=(const T & Rate)
        {
            X /= Rate;
            Y /= Rate;
            return *this;
        }

        template<typename = EnableIfT<!IsSameV<T, float32_t>>>
        TVec2 & operator /=(float32_t Rate)
        {
            X = static_cast<T>(X / Rate);
            Y = static_cast<T>(Y / Rate);
            return *this;
        }

        TVec2 operator -() const requires IsSignedV<T>
        {
            return TVec2(-X, -Y);
        }

        TVec2 operator +() const
        {
            return *this;
        }

        //--------------------------------------------------比较运算
        bool operator ==(const TVec2 & Another) const
        {
            if constexpr (IsFloatingPointV<T>)
                return FloatEqual(X, Another.X) && FloatEqual(Y, Another.Y);
            else
                return X == Another.X && Y == Another.Y;
        }

        bool operator !=(const TVec2 & Another) const
        {
            return !operator ==(Another);
        }

        template<typename T2>
        TVec2<T2> To() const
        {
            return { static_cast<T2>(X), static_cast<T2>(Y) };
        }

        template<class ConvertT>
        auto Convert(ConvertT convert) const
        {
            return TVec2(convert(X), convert(Y));
        }
    	
        friend FArchive & operator >>(FArchive & Archive, TVec2 & Vec2)
        {
            Archive >> Vec2.X;
            Archive >> Vec2.Y;
            return Archive;
        }
    public:
    	union
    	{
            struct
            {
                T X;
                T Y;
            };

            struct
            {
                T Width;
                T Height;
    		};

            T Values[2];
    	};
    };

    template CORE_API class TVec2<int32_t>;
    template CORE_API class TVec2<uint32_t>;
    template CORE_API class TVec2<float32_t>;

    using Int2 = TVec2<int32_t>;
    using UInt2 = TVec2<uint32_t>;

    using Vec2I = TVec2<int32_t>;
    using Vec2U = TVec2<uint32_t>;
    using Vec2F = TVec2<float32_t>;

    using SizeI = TVec2<int32_t>;
    using SizeU = TVec2<uint32_t>;
    using SizeF = TVec2<float32_t>;

    using PointI = TVec2<int32_t>;
    using PointU = TVec2<uint32_t>;
    using PointF = TVec2<float32_t>;
	
}
