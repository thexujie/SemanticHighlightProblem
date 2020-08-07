#pragma once

#include "IOInc.h"

namespace XE
{
	enum class EFileBOM
	{
		None = 0,
		UTF8,
		UTF16BE,
		UTF16LE,
	};
	
	enum class EFileMode
	{
		None = 0,
		In = 0x0001,
		Out = 0x0002,

		Truncate = 0x0100,
		CreateNew = 0x0200,
		CreateAlways = 0x0400,

		DontShareRead = 0x00010000,
		InOut = In | Out,
	};
	using EFileModes = TFlag<EFileMode>;

	enum class EFilePosition
	{
		Begin = 0,
		End,
	};

	class CORE_API FFile
	{
	public:
		FFile() = delete;
		~FFile();
		FFile(const FFile &) = delete;
		FFile(FFile && That) noexcept : Handle(That.Handle) { That.Handle = nullptr; }
		FFile(FStringView FileName);

		bool IsValid() const { return !!Handle; }
		size_t GetSize() const;
		
	private:
		ptr_t Handle = nullptr;

	public:
		static bool Exists(FStringView FilePath);
		static FString ReadToString(FStringView FileName, EFileBOM * FileBOM = nullptr);
		static bool SaveToFile(FStringView FileName, FStringView String);
		static TVector<byte_t> ReadToBytes(FStringView FileName);
		
	};
}