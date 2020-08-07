#include "PCH.h"
#include "String.h"
#include "Atomic.h"

namespace XE
{
	static const size_t StringBufferCount = 32;
	static const int STRING_BUFFER_SIZE = 512;
	enum EFormatBuffer
	{
		EFormatBuffer_Ansi = 0,
		EFormatBuffer_Utf8,
		EFormatBuffer_Wide,
		EFormatBuffer_Count,
	};

	template<typename CharT>
	static int32_t GStringBufferIndex;

	template<typename CharT>
	static TString<CharT> GStringBuffers[StringBufferCount];

	template<typename CharT>
	TString<CharT> & GetThreadLocalStringBuffer()
	{
		int32_t Index = Atomics::IncFetch(GStringBufferIndex<CharT>) - 1;
		auto & ThreadLocalStringBuffer = GStringBuffers<CharT>[Index % StringBufferCount];
		if (ThreadLocalStringBuffer.Capacity < STRING_BUFFER_SIZE)
			ThreadLocalStringBuffer.Resize(0, STRING_BUFFER_SIZE);
		return ThreadLocalStringBuffer;
	}

	template TString<char> & GetThreadLocalStringBuffer<char>();
	template TString<char8_t> & GetThreadLocalStringBuffer<char8_t>();
	template TString<wchar_t> & GetThreadLocalStringBuffer<wchar_t>();
	
	namespace Strings
	{
		FU8StringView AnsiToUtf8(const char * Source, size_t StringLength)
		{
			if (StringLength == NullPos)
				StringLength = Length(Source);

			if (!StringLength)
				return FU8StringView();

			TString<wchar_t> & ConvertBufferWide = GetThreadLocalStringBuffer<wchar_t>();
			
			size_t NumWideChars = 0, NumUtf8Chars = 0;
			errno_t err = _mbstowcs_s_l(&NumWideChars, ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), Source, StringLength, Locals::Ansi());
			if (err)
				return {};

			if (NumWideChars > ConvertBufferWide.GetCapacityWithoutNull())
			{
				ConvertBufferWide.Resize(0, NumWideChars + 1);
				err = _mbstowcs_s_l(&NumWideChars, ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), Source, StringLength, Locals::Ansi());
			}

			TString<char8_t> & ConvertBufferUtf8 = GetThreadLocalStringBuffer<char8_t>();
			err = _wcstombs_s_l(&NumUtf8Chars, (char *)ConvertBufferUtf8.GetData(), ConvertBufferUtf8.GetCapacityWithoutNull(), ConvertBufferWide.GetData(), NumWideChars * 2, Locals::Utf8());
			if (err)
				return {};

			if (NumUtf8Chars > ConvertBufferUtf8.GetCapacityWithoutNull())
			{
				ConvertBufferUtf8.Resize(0, NumUtf8Chars + 1);
				size_t NumUtf8CharsExpect = NumUtf8Chars;
				err = _wcstombs_s_l(&NumUtf8Chars, (char *)ConvertBufferUtf8.GetData(), ConvertBufferUtf8.GetCapacityWithoutNull(), ConvertBufferWide.GetData(), NumWideChars * 2, Locals::Utf8());
				assert(NumUtf8Chars == NumUtf8CharsExpect);
			}
			assert(NumUtf8Chars > 0);
			return FU8StringView(ConvertBufferUtf8.GetData(), NumUtf8Chars - 1);
		}

		FWStringView AnsiToWide(const char * Source, size_t StringLength)
		{
			if (StringLength == NullPos)
				StringLength = Length(Source);

			if (!StringLength)
				return FWStringView();

			TString<wchar_t> & ConvertBufferWide = GetThreadLocalStringBuffer<wchar_t>();
			
			size_t NumWideChars = 0;
			errno_t err = _mbstowcs_s_l(&NumWideChars, ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), (const char *)Source, StringLength, Locals::Ansi());
			if (err)
				return {};

			if (NumWideChars > ConvertBufferWide.GetCapacityWithoutNull())
			{
				ConvertBufferWide.Resize(0, NumWideChars + 1);
				size_t NumWideCharsExpect = NumWideChars;
				err = _mbstowcs_s_l(&NumWideChars, ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), (const char *)Source, StringLength, Locals::Ansi());
				assert(NumWideCharsExpect == NumWideChars);
			}
			assert(NumWideChars > 0);
			return FWStringView(ConvertBufferWide.GetData(), NumWideChars - 1);
		}

		FAStringView Utf8ToAnsi(const char8_t * Source, size_t StringLength)
		{
			if (StringLength == NullPos)
				StringLength = Length(Source);

			if (!StringLength)
				return FAStringView();

			TString<wchar_t> & ConvertBufferWide = GetThreadLocalStringBuffer<wchar_t>();

			size_t NumWideChars = 0, NumAnsiChars = 0;
			errno_t err = _mbstowcs_s_l(&NumWideChars, ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), (const char *)Source, StringLength, Locals::Utf8());
			if (err)
				return {};

			if (NumWideChars > ConvertBufferWide.GetCapacityWithoutNull())
			{
				ConvertBufferWide.Resize(0, NumWideChars + 1);
				err = _mbstowcs_s_l(&NumWideChars, ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), (const char *)Source, StringLength, Locals::Utf8());
			}

			TString<char> & GConvertBufferAnsi = GetThreadLocalStringBuffer<char>();
			err = _wcstombs_s_l(&NumAnsiChars, GConvertBufferAnsi.GetData(), GConvertBufferAnsi.GetCapacityWithoutNull(), ConvertBufferWide.GetData(), NumWideChars * 2, Locals::Ansi());
			if (err)
				return {};

			if (NumAnsiChars > GConvertBufferAnsi.GetCapacityWithoutNull())
			{
				GConvertBufferAnsi.Resize(0, NumAnsiChars + 1);
				size_t NumAnsiCharsExpect = NumAnsiChars;
				err = _wcstombs_s_l(&NumAnsiChars, (char *)GConvertBufferAnsi.GetData(), GConvertBufferAnsi.GetCapacityWithoutNull(), ConvertBufferWide.GetData(), NumWideChars * 2, Locals::Ansi());
				assert(NumAnsiChars == NumAnsiCharsExpect);
			}
			assert(NumAnsiChars > 0);
			return FAStringView(GConvertBufferAnsi.GetData(), NumAnsiChars - 1);
		}

		FWStringView Utf8ToWide(const char8_t * Source, size_t StringLength)
		{
			if (StringLength == NullPos)
				StringLength = Length(Source);

			if (!StringLength)
				return FWStringView();

			size_t BufferSize = AlignUp(StringLength * 2 + 1, STRING_BUFFER_SIZE);
			TString<wchar_t> & ConvertBufferWide = GetThreadLocalStringBuffer<wchar_t>();

			size_t NumWideChars = 0;
			errno_t err = _mbstowcs_s_l(&NumWideChars, ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), (const char *)Source, StringLength, Locals::Utf8());
			if (err)
				return {};

			if (NumWideChars > ConvertBufferWide.GetCapacityWithoutNull())
			{
				ConvertBufferWide.Resize(0, NumWideChars + 1);
				size_t NumWideCharsExpect = NumWideChars;
				err = _mbstowcs_s_l(&NumWideChars, ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), (const char *)Source, StringLength, Locals::Utf8());
				assert(NumWideCharsExpect == NumWideChars);
			}
			assert(NumWideChars > 0);
			return FWStringView(ConvertBufferWide.GetData(), NumWideChars - 1);
		}

		FAStringView WideToAnsi(const wchar_t * String, size_t StringLength)
		{
			if (StringLength == NullPos)
				StringLength = Length(String);

			if (!StringLength)
				return FAStringView();

			TString<char> & GConvertBufferAnsi = GetThreadLocalStringBuffer<char>();

			size_t NumAnsiChars = 0;
			errno_t err = _wcstombs_s_l(&NumAnsiChars, GConvertBufferAnsi.GetData(), GConvertBufferAnsi.GetCapacityWithoutNull(), String, StringLength * 2, Locals::Ansi());
			if (err)
				return {};

			if (NumAnsiChars > GConvertBufferAnsi.GetCapacityWithoutNull())
			{
				GConvertBufferAnsi.Resize(0, NumAnsiChars + 1);
				size_t NumAnsiCharsExpect = NumAnsiChars;
				err = _wcstombs_s_l(&NumAnsiChars, (char *)GConvertBufferAnsi.GetData(), GConvertBufferAnsi.GetCapacityWithoutNull(), String, StringLength * 2, Locals::Ansi());
				assert(NumAnsiChars == NumAnsiCharsExpect);
			}
			assert(NumAnsiChars > 0);
			return FAStringView(GConvertBufferAnsi.GetData(), NumAnsiChars - 1);
		}

		FU8StringView WideToUtf8(const wchar_t * Source, size_t StringLength)
		{
			if (StringLength == NullPos)
				StringLength = Length(Source);

			if (!StringLength)
				return FU8StringView();

			TString<char8_t> & ConvertBufferUtf8 = GetThreadLocalStringBuffer<char8_t>();

			size_t NumUtf8Chars = 0;
			errno_t err = _wcstombs_s_l(&NumUtf8Chars, (char *)ConvertBufferUtf8.GetData(), ConvertBufferUtf8.GetCapacityWithoutNull(), Source, StringLength * 2, Locals::Utf8());
			if (err)
				return {};

			if (NumUtf8Chars > ConvertBufferUtf8.GetCapacityWithoutNull())
			{
				ConvertBufferUtf8.Resize(0, NumUtf8Chars + 1);
				size_t NumUtf8CharsExpect = NumUtf8Chars;
				err = _wcstombs_s_l(&NumUtf8Chars, (char *)ConvertBufferUtf8.GetData(), ConvertBufferUtf8.GetCapacityWithoutNull(), Source, StringLength * 2, Locals::Utf8());
				assert(NumUtf8Chars == NumUtf8CharsExpect);
			}
			assert(NumUtf8Chars > 0);
			return FU8StringView(ConvertBufferUtf8.GetData(), NumUtf8Chars - 1);
		}

		FAStringView Printf(const char * Format, ...)
		{
			TString<char> & GConvertBufferAnsi = GetThreadLocalStringBuffer<char>();

			va_list ArgList;
			va_start(ArgList, Format);
			int32_t NumAnsiChars = std::vsnprintf(GConvertBufferAnsi.GetData(), GConvertBufferAnsi.GetCapacityWithoutNull(), Format, ArgList);
			// error happend
			if (NumAnsiChars == -1)
				return FAStringView();

			if (NumAnsiChars > GConvertBufferAnsi.GetCapacityWithoutNull())
			{
				GConvertBufferAnsi.Resize(0, NumAnsiChars + 1);
				size_t NumAnsiCharsExpect = NumAnsiChars;
				NumAnsiChars = std::vsnprintf(GConvertBufferAnsi.GetData(), GConvertBufferAnsi.GetCapacityWithoutNull(), Format, ArgList);
				assert(NumAnsiCharsExpect == NumAnsiChars);
			}

			return FAStringView(GConvertBufferAnsi.GetData(), NumAnsiChars);
		}

		FU8StringView Printf(const char8_t * Format, ...)
		{
			TString<char8_t> & ConvertBufferUtf8 = GetThreadLocalStringBuffer<char8_t>();

			va_list ArgList;
			va_start(ArgList, Format);
			int32_t NumUtf8Chars = std::vsnprintf((char *)ConvertBufferUtf8.GetData(), ConvertBufferUtf8.GetCapacityWithoutNull(), (const char *)Format, ArgList);
			// error happend
			if (NumUtf8Chars == -1)
				return FU8StringView();

			if (NumUtf8Chars > ConvertBufferUtf8.GetCapacityWithoutNull())
			{
				ConvertBufferUtf8.Resize(0, NumUtf8Chars + 1);
				size_t NumUtf8CharsExpect = NumUtf8Chars;
				NumUtf8Chars = std::vsnprintf((char *)ConvertBufferUtf8.GetData(), ConvertBufferUtf8.GetCapacityWithoutNull(), (const char *)Format, ArgList);
				assert(NumUtf8CharsExpect == NumUtf8Chars);
			}

			return FU8StringView(ConvertBufferUtf8.GetData(), NumUtf8Chars);
		}

		FWStringView Printf(const wchar_t * Format, ...)
		{
			TString<wchar_t> & ConvertBufferWide = GetThreadLocalStringBuffer<wchar_t>();

			va_list ArgList;
			va_start(ArgList, Format);
			int32_t NumWideChars = std::vswprintf(ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), Format, ArgList);
			// error happend
			if (NumWideChars == -1)
				return FWStringView();

			if (NumWideChars > ConvertBufferWide.GetCapacityWithoutNull())
			{
				ConvertBufferWide.Resize(0, NumWideChars + 1);
				size_t NumWideCharsExpect = NumWideChars;
				NumWideChars = std::vswprintf(ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), Format, ArgList);
				assert(NumWideCharsExpect == NumWideChars);
			}

			return FWStringView(ConvertBufferWide.GetData(), NumWideChars);
		}

		int32_t ToBool(FStringView String)
		{
			return TStringToBoolean(String.Data, String.Size);
		}

		int32_t ToInt32(FStringView String, uint32_t Radix)
		{
			return ToInterger<Char, int32_t>(String.Data, String.Size);
		}

		uint32_t ToUInt32(FStringView String, uint32_t Radix)
		{
			return ToInterger<Char, uint32_t>(String.Data, String.Size);
		}

		int64_t ToInt64(FStringView String, uint32_t Radix)
		{
			return ToInterger<Char, int32_t>(String.Data, String.Size);
		}

		uint64_t ToUInt64(FStringView String, uint32_t Radix)
		{
			return ToInterger<Char, uint64_t>(String.Data, String.Size);
		}

		intx_t ToIntX(FStringView String, uint32_t Radix)
		{
			return ToInterger<Char, intx_t>(String.Data, String.Size);
		}

		uintx_t ToUIntX(FStringView String, uint32_t Radix)
		{
			return ToInterger<Char, uintx_t>(String.Data, String.Size);
		}

		float32_t ToFloat32(FStringView String)
		{
			return ToFloatingPoint<Char, float32_t>(String.Data, String.Size);
		}

		float64_t ToFloat64(FStringView String)
		{
			return ToFloatingPoint<Char, float64_t>(String.Data, String.Size);
		}
	}
}


template<typename CharT>
XE::TStringView<CharT> operator +(XE::TStringView<CharT> Lhs, XE::TStringView<CharT> Rhs)
{
	XE::TString<CharT> & Buffer = XE::GetThreadLocalStringBuffer<CharT>();
	Buffer.Resize(Lhs.Size + Rhs.Size);
	XE::Memcpy(Buffer.Data, Lhs.Data, sizeof(CharT) * Lhs.Size);
	XE::Memcpy(Buffer.Data + Lhs.Size, Rhs.Data, sizeof(CharT) * Rhs.Size);
	return Buffer;
}

template XE::TStringView<char> operator +(XE::TStringView<char> Lhs, XE::TStringView<char> Rhs);
template XE::TStringView<wchar_t> operator +(XE::TStringView<wchar_t> Lhs, XE::TStringView<wchar_t> Rhs);
template XE::TStringView<char8_t> operator +(XE::TStringView<char8_t> Lhs, XE::TStringView<char8_t> Rhs);
template XE::TStringView<char16_t> operator +(XE::TStringView<char16_t> Lhs, XE::TStringView<char16_t> Rhs);
