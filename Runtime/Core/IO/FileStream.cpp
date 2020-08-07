#include "PCH.h"
#include "FileStream.h"

#include "Core/Logger.h"
#include "Platform/Win32/Win32.h"

namespace XE
{
	FFileStream::FFileStream(FStringView FilePath, EFileModes FileModes)
	{
		Open(FilePath, FileModes);
	}

	FFileStream::~FFileStream()
	{
		if (Handle)
		{
			CloseHandle(Handle);
			Handle = nullptr;
		}
	}

	EError FFileStream::Open(FStringView FilePath, EFileModes FileModes)
	{
		FWStringView FilePathW = Strings::ToWide(FilePath);

		TFlag<uint32_t> DesiredAccess;
		uint32_t CreationDisposition = FileModes.Any(EFileMode::Out) ? OPEN_ALWAYS : OPEN_EXISTING;
		TFlag<uint32_t> ShareMode = 0;

		DesiredAccess.Set(GENERIC_READ, FileModes.Any(EFileMode::In));
		DesiredAccess.Set(GENERIC_WRITE, FileModes.Any(EFileMode::Out));
		
		if (FileModes.Any(EFileMode::CreateNew))
			CreationDisposition = CREATE_NEW;
		else if (FileModes.Any(EFileMode::CreateAlways))
			CreationDisposition = CREATE_ALWAYS;
		else if (FileModes.Any(EFileMode::Truncate))
			CreationDisposition = TRUNCATE_EXISTING;

		ShareMode.Set(FILE_SHARE_READ, !FileModes.Any(EFileMode::DontShareRead));
		
		uint32_t FlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
		HANDLE hFile = CreateFileW(FilePathW.Data, DesiredAccess, ShareMode, NULL, CreationDisposition, FlagsAndAttributes, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return EError::Inner;

		Handle = hFile;
		return EError::OK;
	}

	void FFileStream::Close()
	{
		if (Handle)
		{
			CloseHandle(Handle);
			Handle = nullptr;
		}
	}

	void FFileStream::Flush()
	{
		if (Handle)
		{
			FlushFileBuffers(Handle);
		}
	}

	size_t FFileStream::GetSize() const
	{
		if (!Handle)
			return 0;

		LARGE_INTEGER FileSize;
		BOOL bGetFileSizeEx = GetFileSizeEx(Handle, &FileSize);
		assert(bGetFileSizeEx);
		return size_t(FileSize.QuadPart);
	}

	void FFileStream::Seek(EFilePosition FilePosition, intx_t Position)
	{
		if (!Handle)
			return;

		DWORD SeekResult = 0;
		switch(FilePosition)
		{
		case EFilePosition::Begin:
			SeekResult = ::SetFilePointer(Handle, long(Position), 0, FILE_BEGIN);
			break;
		case EFilePosition::End:
			SeekResult = ::SetFilePointer(Handle, long(Position), 0, FILE_END);
			break;
		default:
			break;
		}
		if (SeekResult == INVALID_SET_FILE_POINTER) 
		{
			LogWarning(Str(__FUNCTION__ " Terminal failure: Unable to set file pointer to end-of-file: {}"), Win32::ErrorStr(GetLastError()));
		}
	}
	
	uintx_t FFileStream::Read(void * Data, uintx_t Size)
	{
		if (!Handle)
			throw EError::IO;

		DWORD NumberOfBytesRead = 0;
		BOOL bReadFile = ReadFile(Handle, Data, DWORD(Size), &NumberOfBytesRead, NULL);
		if (!bReadFile)
			return 0;
		assert(NumberOfBytesRead == Size);
		return NumberOfBytesRead;
	}

	uintx_t FFileStream::Write(const void * Data, uintx_t Size)
	{
		if (!Handle)
			throw EError::IO;

		DWORD NumberOfBytesWriten = 0;
		BOOL bReadFile = WriteFile(Handle, Data, DWORD(Size), &NumberOfBytesWriten, NULL);
		if (!bReadFile)
			return 0;
		assert(NumberOfBytesWriten == Size);
		return NumberOfBytesWriten;
	}
}
