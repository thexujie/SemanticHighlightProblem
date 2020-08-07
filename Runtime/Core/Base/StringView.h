#pragma once

#include "Hash.h"

namespace std
{
	using u8ios = basic_ios<char8_t, char_traits<char8_t>>;
	using u8streambuf = basic_streambuf<char8_t, char_traits<char8_t>>;
	using u8istream = basic_istream<char8_t, char_traits<char8_t>>;
	using u8ostream = basic_ostream<char8_t, char_traits<char8_t>>;
	using u8iostream = basic_iostream<char8_t, char_traits<char8_t>>;
	using u8stringbuf = basic_stringbuf<char8_t, char_traits<char8_t>, allocator<char8_t>>;
	using u8istringstream = basic_istringstream<char8_t, char_traits<char8_t>, allocator<char8_t>>;
	using u8ostringstream = basic_ostringstream<char8_t, char_traits<char8_t>, allocator<char8_t>>;
	using u8stringstream = basic_stringstream<char8_t, char_traits<char8_t>, allocator<char8_t>>;
	using u8filebuf = basic_filebuf<char8_t, char_traits<char8_t>>;
	using u8ifstream = basic_ifstream<char8_t, char_traits<char8_t>>;
	using u8ofstream = basic_ofstream<char8_t, char_traits<char8_t>>;
	using u8fstream = basic_fstream<char8_t, char_traits<char8_t>>;

	using xstring = basic_string<XE::Char, char_traits<XE::Char>>;
	using xstring_view = basic_string_view<XE::Char, char_traits<XE::Char>>;
	using xios = basic_ios<XE::Char, char_traits<XE::Char>>;
	using xstreambuf = basic_streambuf<XE::Char, char_traits<XE::Char>>;
	using xistream = basic_istream<XE::Char, char_traits<XE::Char>>;
	using xostream = basic_ostream<XE::Char, char_traits<XE::Char>>;
	using xiostream = basic_iostream<XE::Char, char_traits<XE::Char>>;
	using xstringbuf = basic_stringbuf<XE::Char, char_traits<XE::Char>, allocator<XE::Char>>;
	using xistringstream = basic_istringstream<XE::Char, char_traits<XE::Char>, allocator<XE::Char>>;
	using xostringstream = basic_ostringstream<XE::Char, char_traits<XE::Char>, allocator<XE::Char>>;
	using xstringstream = basic_stringstream<XE::Char, char_traits<XE::Char>, allocator<XE::Char>>;
	using xfilebuf = basic_filebuf<XE::Char, char_traits<XE::Char>>;
	using xifstream = basic_ifstream<XE::Char, char_traits<XE::Char>>;
	using xofstream = basic_ofstream<XE::Char, char_traits<XE::Char>>;
	using xfstream = basic_fstream<XE::Char, char_traits<XE::Char>>;
}

namespace XE
{
	namespace Strings
	{
		template<typename CharT>
		int32_t Compare(const CharT * StringLeft, size_t LengthLeft, const CharT * StringRight, size_t LengthRight)
		{
			if (LengthLeft == NullIndex) LengthLeft = Length<CharT>(StringLeft);
			if (LengthRight == NullIndex) LengthRight = Length<CharT>(StringRight);

			if (!LengthLeft && !LengthRight)
				return 0;

			if (!LengthLeft) return LengthRight ? 1 : -1;
			if (!LengthRight) return LengthLeft ? 1 : -1;


			if constexpr (IsAnyOfV<CharT, char, char8_t>)
				return strncmp((const char *)StringLeft, (const char *)StringRight, Max(LengthLeft, LengthRight));
			else if constexpr (IsAnyOfV<CharT, wchar_t, char16_t>)
				return wcsncmp((const wchar_t *)StringLeft, (const wchar_t *)StringRight, Max(LengthLeft, LengthRight));
			else
				throw EError::NotImplemented;
		}

		template<typename CharT>
		int32_t ICCompare(const CharT * StringLeft, size_t LengthLeft, const CharT * StringRight, size_t LengthRight)
		{
			if (LengthLeft == NullIndex) LengthLeft = Length<CharT>(StringLeft);
			if (LengthRight == NullIndex) LengthRight = Length<CharT>(StringRight);

			if (!LengthLeft && !LengthRight)
				return 0;

			if (!LengthLeft) return LengthRight ? 1 : -1;
			if (!LengthRight) return LengthLeft ? 1 : -1;


			if constexpr (IsAnyOfV<CharT, char, char8_t>)
				return _strnicmp((const char *)StringLeft, (const char *)StringRight, Max(LengthLeft, LengthRight));
			else if constexpr (IsAnyOfV<CharT, wchar_t, char16_t>)
				return _wcsnicmp((const wchar_t *)StringLeft, (const wchar_t *)StringRight, Max(LengthLeft, LengthRight));
			else
				throw EError::NotImplemented;
		}
		
		template<typename CharT>
		size_t FindFirst(const CharT * String, size_t Length, const CharT & Ch)
		{
			for (size_t Index = 0; Index < Length; ++Index)
			{
				if (Ch == String[Index])
					return Index;
			}
			return NullIndex;
		}

		template<typename CharT>
		size_t FindFirst(const CharT * String, size_t Length, const CharT * Pattern, size_t PatternLength)
		{
			if (PatternLength > Length)
				return NullIndex;

			for (size_t Index = 0; Index < Length - PatternLength; ++Index)
			{
				bool Match = true;
				for (size_t PatternIndex = 0; PatternIndex < PatternLength; ++PatternIndex)
				{
					if (String[Index + PatternIndex] != Pattern[PatternIndex])
					{
						Match = false;
						break;
					}
				}

				if (Match)
					return Index;
			}

			return NullIndex;
		}

		template<typename CharT>
		size_t FindFirstOf(const CharT * String, size_t Length, const CharT * Pattern, size_t PatternLength)
		{
			for (size_t PatternIndex = 0; PatternIndex < PatternLength; ++PatternIndex)
			{
				for (size_t Index = 0; Index < Length; --Index)
				{
					if (String[Index] == Pattern[PatternIndex])
						return Index;
				}
			}
			return NullIndex;
		}
		
		template<typename CharT>
		size_t FindLast(const CharT * String, size_t Length, const CharT & Ch)
		{
			for (size_t Index = Length; Index > 0; --Index)
			{
				if (Ch == String[Index - 1])
					return Index;
			}
			return NullIndex;
		}

		template<typename CharT>
		size_t FindLast(const CharT * String, size_t Length, const CharT * Pattern, size_t PatternLength)
		{
			if (PatternLength > Length)
				return NullIndex;

			for (size_t Index = Length - PatternLength; Index > 0; --Index)
			{
				bool Match = true;
				for (size_t PatternIndex = 0; PatternIndex < PatternLength; ++PatternIndex)
				{
					if (String[Index - 1] == Pattern[PatternIndex])
					{
						Match = false;
						break;
					}
				}

				if (Match)
					return Index;
			}
			return NullIndex;
		}

		template<typename CharT>
		size_t FindLastOf(const CharT * String, size_t Length, const CharT * Pattern, size_t PatternLength)
		{
			for (size_t PatternIndex = 0; PatternIndex < PatternLength; ++PatternIndex)
			{
				for (size_t Index = Length; Index > 0; --Index)
				{
					if (String[Index - 1] == Pattern[PatternIndex])
						return Index - 1;
				}
			}
			return NullIndex;
		}
		
		template<typename CharT>
		bool StartsWith(const CharT * String, size_t Length, const CharT * Pattern, size_t PatternLength)
		{
			if (PatternLength > Length)
				return false;

			for (size_t Index = 0; Index < PatternLength; ++Index)
			{
				if (String[Index] != Pattern[Index])
					return false;
			}
			return true;
		}

		template<typename CharT>
		bool EndsWith(const CharT * String, size_t Length, const CharT * Pattern, size_t PatternLength)
		{
			if (PatternLength > Length)
				return false;

			for (size_t Index = 0; Index < PatternLength; ++Index)
			{
				if (String[Length - PatternLength + Index] != Pattern[Index])
					return false;
			}
			return true;
		}
	}
	
	template<typename CharT>
	class TStringView
	{
	public:
		using CharType = CharT;
		
	public:
		union
		{
			const CharT * Data = nullptr;
			void * CharData;
		};
		size_t Size = 0;

	public:
		constexpr TStringView() = default;
		constexpr TStringView(const TStringView &) = default;
		constexpr TStringView(TStringView &&) noexcept = default;
		TStringView & operator =(const TStringView &) = default;
		TStringView & operator =(TStringView &&) noexcept = default;
		auto operator<=>(const TStringView & Another) const 
		{
			return Strings::Compare<CharT>(Data, Size, Another.Data, Another.Size) <=> 0;
		}
		auto operator==(const TStringView & Another) const
		{
			return Strings::Compare<CharT>(Data, Size, Another.Data, Another.Size) == 0;
		}
		
		explicit constexpr TStringView(std::basic_string_view<CharT> String) : Data(String.data()), Size(String.size()) {}
		constexpr TStringView(const CharT * String) : Data(String), Size(Length(String)) {}
		constexpr TStringView(const CharT * String, size_t SizeIn) : Data(String), Size(SizeIn == NullIndex ? strlen(reinterpret_cast<const char *>(String)) : SizeIn) {}

		template<size_t Size_>
		constexpr TStringView(const CharT(&String)[Size_]) : Data(String), Size(Size_) {}

		size_t GetSize() const { return Size; }
		const CharT * GetData() const { return Data; }
		bool IsEmpty() const { return Size == 0; }
		const CharT & operator[](size_t Index) const { return Data[Index]; }

		size_t FindFirst(const CharT & Ch) const
		{
			return Strings::FindFirst<CharT>(Data, Size, Ch);
		}
		
		size_t FindFirst(TStringView Pattern) const
		{
			return Strings::FindFirst<CharT>(Data, Size, Pattern.Data, Pattern.Size);
		}

		size_t FindFirstOf(TStringView Chs) const
		{
			return Strings::FindFirstOf<CharT>(Data, Size, Chs.Data, Chs.Size);
		}

		size_t FindLast(const CharT & Ch) const
		{
			return Strings::FindLast<CharT>(Data, Size, Ch);
		}

		size_t FindLast(TStringView Pattern) const
		{
			return Strings::FindLast<CharT>(Data, Size, Pattern.Data, Pattern.Size);
		}
		
		size_t FindLastOf(TStringView Chs) const
		{
			return Strings::FindLastOf<CharT>(Data, Size, Chs.Data, Chs.Size);
		}
		
		bool StartsWith(TStringView Pattern) const
		{
			return Strings::StartsWith(Data, Size, Pattern.Data, Pattern.Size);
		}

		bool EndsWith(TStringView Pattern) const
		{
			return Strings::EndsWith(Data, Size, Pattern.Data, Pattern.Size);
		}

		TStringView Slice(size_t Index, size_t Count = NullIndex) const
		{
			if (Count == NullIndex)
			{
				if (Index >= Size)
					return TStringView();
				Count = Size - Index;
			}
			else if (Index + Count > Size)
				return TStringView();
			else {}
			
			return TStringView(Data + Index, Count);
		}
		
		const CharT * Begin() const { return Data; }
		const CharT * End() const { return Data + Size; }

	public:
		static TStringView Empty;
	};

	namespace Strings
	{
		template<typename CharT>
		int32_t Compare(TStringView<CharT> Left, TStringView<CharT> Right)
		{
			return Compare<CharT>(Left.Data, Left.Size, Right.Data, Right.Size);
		}

		template<typename CharT>
		int32_t ICCompare(TStringView<CharT> Left, TStringView<CharT> Right)
		{
			return ICCompare<CharT>(Left.Data, Left.Size, Right.Data, Right.Size);
		}
	}

	template <typename T>
	TStringView<T> TStringView<T>::Empty = {};

	template class CORE_API TStringView<char>;
	using FAStringView = CORE_API TStringView<char>;

	template class CORE_API TStringView<wchar_t>;
	using FWStringView = CORE_API TStringView<wchar_t>;

	template class CORE_API TStringView<char8_t>;
	using FU8StringView = CORE_API TStringView<char8_t>;

	template class CORE_API TStringView<char16_t>;
	using FU16StringView = CORE_API TStringView<char16_t>;

	template class CORE_API TStringView<char32_t>;
	using FU32StringView = CORE_API TStringView<char32_t>;

	using FStringView = TStringView<Char>;

	template<typename CharT>
	struct THash<TStringView<CharT>> { uintx_t operator()(const TStringView<CharT> & Value) const { return HashString(Value.GetData(), Value.GetSize()); } };

	template<typename T>
	class TConverter
	{
	public:
		FStringView ToString(const T &) const { return FStringView::Empty; }
		NullPtrT FromString(FStringView String) const { return nullptr; }
	};

	template<>
	class TConverter<FStringView>
	{
	public:
		FStringView ToString(const FStringView & Value) const
		{
			return Value;
		}

		FStringView FromString(FStringView String) const
		{
			return String;
		}
	};
	
	inline XE::TStringView<char> operator ""_sv(const char * str, size_t size)
	{
		return XE::TStringView<char>(str, size);
	}

	inline XE::TStringView<char8_t> operator ""_sv(const char8_t * str, size_t size)
	{
		return XE::TStringView<char8_t>(str, size);
	}

	namespace Strings
	{
		CORE_API int32_t Stricmp(FAStringView StringLeft, FAStringView StringRight);
		CORE_API int32_t Stricmp(FU8StringView StringLeft, FU8StringView StringRight);
		CORE_API int32_t Stricmp(FU16StringView StringLeft, FU16StringView StringRight);
		CORE_API int32_t Stricmp(FU32StringView StringLeft, FU32StringView StringRight);
		CORE_API int32_t Stricmp(FWStringView StringLeft, FWStringView StringRight);
	}
}

#define StrView(XXX) FStringView(Str(XXX))
