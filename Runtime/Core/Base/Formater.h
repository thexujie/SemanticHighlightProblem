#pragma once

#include "StringView.h"
#include "fmt/format.h"

namespace XE
{
	template<typename T>
	struct TFormatter;
	
	template<typename CharT>
	fmt::basic_memory_buffer<CharT> & GetThreadLocalFormatBuffer();
	
	template CORE_API fmt::basic_memory_buffer<char> & GetThreadLocalFormatBuffer<char>();
	template CORE_API fmt::basic_memory_buffer<char8_t> & GetThreadLocalFormatBuffer<char8_t>();
	template CORE_API fmt::basic_memory_buffer<wchar_t> & GetThreadLocalFormatBuffer<wchar_t>();
	
	template<typename CharT, typename ...ArgsT>
	TStringView<CharT> FormatImpl(TStringView<CharT> Formal, ArgsT && ...Args)
	{
		fmt::basic_memory_buffer<CharT> & FormatBuffer = GetThreadLocalFormatBuffer<CharT>();
		fmt::format_to(FormatBuffer, fmt::basic_string_view<CharT>(Formal.Data, Formal.Size) , Forward<ArgsT>(Args)...);
		return TStringView<CharT>(FormatBuffer.begin(), FormatBuffer.size());
	}

	template<typename ...ArgsT>
	TStringView<char> Format(TStringView<char> Formal, ArgsT && ...Args)
	{
		return FormatImpl(Formal, Forward<ArgsT>(Args)...);
	}

	template<typename ...ArgsT>
	TStringView<char8_t> Format(TStringView<char8_t> Formal, ArgsT && ...Args)
	{
		return FormatImpl(Formal, Forward<ArgsT>(Args)...);
	}

	template<typename ...ArgsT>
	TStringView<wchar_t> Format(TStringView<wchar_t> Formal, ArgsT && ...Args)
	{
		return FormatImpl(Formal, Forward<ArgsT>(Args)...);
	}
	
	template<typename T>
	concept FormatAble = requires(T t)
	{
		t.ToString();
	};

	template<FormatAble T>
	struct TFormatter<T>
	{
		FStringView operator()(const T & Value) const
		{
			return Value.ToString();
		}
	};
}

namespace fmt
{
	template<typename T>
	concept UserDefineFormatAble = requires(const T & Value)
	{
		XE::TFormatter<T>()(Value);
	};
	
	template<>
	struct formatter<XE::FStringView> : formatter<basic_string_view<XE::Char>>
	{
		template <typename FormatContext>
		auto format(XE::FStringView String, FormatContext & Context)
		{
			return formatter<string_view>::format(basic_string_view<XE::Char>(String.Data, String.Size), Context);
		}
	};

	template<UserDefineFormatAble T>
	struct formatter<T> : formatter<basic_string_view<XE::Char>>
	{
		template <typename FormatContext>
		auto format(const T & Value, FormatContext & Context)
		{
			return formatter<XE::FStringView>().format(XE::TFormatter<T>()(Value), Context);
		}
	};
}
