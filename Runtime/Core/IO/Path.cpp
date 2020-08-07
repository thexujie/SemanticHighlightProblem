#include "PCH.h"
#include "Path.h"
#include <filesystem>

namespace XE
{
	Char FPath::DirectorySeparatorChar = Str('\\');

	template<typename CharT>
	inline bool IsSlash(const CharT & Ch)
	{
		return Ch == '/' || Ch == '\\';
	}

	const char SlashChar = '\\';

	template<typename CharT>
	inline CharT ToUpper(const CharT & Ch)
	{
		return IsBetween<CharT>(Ch, 'a', 'z') ? (Ch - ('a' - 'A')) : Ch;
	}

	template<typename CharT>
	inline bool IsDrivePrefix(const CharT & Drive)
	{
		return IsBetween<CharT>(ToUpper(Drive), 'A', 'Z');
	}

	template<typename CharT>
	inline bool IsDrivePrefix(const CharT & Drive, const CharT & Colon)
	{
		return IsBetween<CharT>(ToUpper(Drive), 'A', 'Z') && Colon == ':';
	}
	
	template<typename CharT>
	diff_t PathBackward(const CharT * Path, const CharT * Begin)
	{
		const CharT * Curr = Path - 1;
		while(Curr > Begin)
		{
			if (IsSlash(*Curr))
				return Curr - Path;
			--Curr;
		}
		return NullIndex;
	}

	template<typename CharT>
	int32_t PathForward(const CharT * Path, const CharT * Begin, const CharT * End)
	{
		while (Path != End)
		{
			if (IsSlash(*Path))
				return Path - Begin;
			++Path;
		}
		return NullIndex;
	}
	
	FStringView FPath::LexicallyNormal(FStringView Path)
	{
		FString & Buffer = GetThreadLocalStringBuffer<Char>();
		if (Path.Size >= Buffer.Capacity)
			Buffer.Resize(0, Path.Size + 1);
		
		const Char * ReadBegin = Path.Begin();
		const Char * Read = Path.Begin();
		const Char * End = Path.End() + 1;
		Char * Write = Buffer.Data;

		uintx_t DotIndex = 0;
		for (; Read != End; ++Read)
		{
			if (Read == End - 1 || IsSlash(Read[0]))
			{
				if (DotIndex == 1 && Read - ReadBegin == 1)
				{
					ReadBegin = Read + 1;
				}
				else if (DotIndex == 2 && Read - ReadBegin == 2)
				{
					if (Write != Buffer.Data)
					{
						if (diff_t BackwardIndex = PathBackward(Write, Buffer.Data); BackwardIndex != NullIndex)
							Write = Write + BackwardIndex;
					}
					ReadBegin = Read + 1;
				}
				else
				{
					if (Write != Buffer.Data)
						*Write++ = SlashChar;
					
					while (ReadBegin < Read)
						*Write++ = *ReadBegin++;
					++ReadBegin;
				}
				
				DotIndex = 0;
				continue;
			}

			if (Read[0] == '.')
				++DotIndex;
			else
				DotIndex = 0;
		}

		Buffer.Resize(Write - Buffer.Data);
		return Buffer;
	}
	
	FStringView FPath::Combine(FStringView BasePath, FStringView Path)
	{
		if (BasePath.IsEmpty())
			return Path;

		if (Path.IsEmpty())
			return BasePath;


		Char LastChar = BasePath[BasePath.Size - 1];
		bool BasePathHasSeparatorChar = LastChar == Str('\\') || LastChar == Str('/');
		size_t Length = (BasePathHasSeparatorChar ? BasePath.Size : BasePath.Size + 1) + Path.Size;
		FString ResultPath(Length);
		TAllocator<Char>::Copy(ResultPath.GetData(), BasePath.GetData(), BasePath.GetSize());
		if (!BasePathHasSeparatorChar)
			TAllocator<Char>::Copy(ResultPath.GetData() + BasePath.Size, XE::FPath::DirectorySeparatorChar, 1);
		TAllocator<Char>::Copy(ResultPath.GetData() + BasePath.Size + 1, Path.Data, Path.Size);
		return LexicallyNormal(ResultPath);
	}

	FStringView FPath::GetAbsolutePath(FStringView Path)
	{
		if (IsAbsolute(Path))
			return Path;
		return LexicallyNormal(Combine(GetWorkingDirectory(), Path));
	}
	
	FStringView FPath::GetAbsolutePath(FStringView BasePath, FStringView Path)
	{
		if (BasePath.IsEmpty())
			return GetAbsolutePath(Path);

		return GetAbsolutePath(Combine(BasePath, Path));
	}

	FStringView FPath::GetWorkingDirectory()
	{
		TString<wchar_t> & Buffer = GetThreadLocalStringBuffer<wchar_t>();
		DWORD Size = ::GetCurrentDirectoryW(DWORD(Buffer.GetSize()), Buffer.GetData());
		if (Size > Buffer.GetSize())
		{
			Buffer.Resize(Size);
			Size = ::GetCurrentDirectoryW(DWORD(Buffer.GetSize()), Buffer.GetData());
		}
		Buffer.Resize(Size);
		return Strings::FromWide(Buffer);
	}

	bool FPath::IsAbsolute(FStringView Path)
	{
		return Path.Size >= 2 && IsDrivePrefix(Path[0]) && IsDrivePrefix(Path[0], Path[1]);
	}
	
	bool FPath::Exists(FStringView Path)
	{
		DWORD FileAttribute = GetFileAttributesW(Strings::ToWide(Path).Data);
		return !(FileAttribute == INVALID_FILE_ATTRIBUTES);
	}

	bool FPath::IsFile(FStringView Path)
	{
		DWORD FileAttribute = GetFileAttributesW(Strings::ToWide(Path).Data);
		return (FileAttribute != INVALID_FILE_ATTRIBUTES) && !!(FileAttribute & FILE_ATTRIBUTE_ARCHIVE);
	}

	bool FPath::IsDirectory(FStringView Path)
	{
		DWORD FileAttribute = GetFileAttributesW(Strings::ToWide(Path).Data);
		return (FileAttribute != INVALID_FILE_ATTRIBUTES) && !!(FileAttribute & FILE_ATTRIBUTE_DIRECTORY);
	}

	template<typename CharT>
	inline const CharT * FindRootEnd(const CharT * Path, const CharT * PathEnd)
	{
		if (PathEnd - Path < 2)
			return Path;

		if (IsDrivePrefix(Path[0]) && Path[1] == ':')
			return Path + 2;

		return Path;
	}
	
	FStringView FPath::GetDirectory(FStringView Path)
	{
		const Char * PathEnd = Path.End();
		const Char * PathBegin = FindRootEnd(Path.Begin(), PathEnd);
		while(PathEnd != PathBegin)
		{
			if (IsSlash(PathEnd[-1]))
				return FStringView(Path.Begin(), PathEnd - Path.Begin() - 1);
			--PathEnd;
		}
		return FStringView::Empty;
	}
}