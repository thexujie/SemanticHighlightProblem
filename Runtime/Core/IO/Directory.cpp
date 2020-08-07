#include "PCH.h"
#include "Directory.h"

namespace XE
{
	template<typename LambdaT>
	static void EnumPath(FWStringView FilePathW, LambdaT && Lambda)
	{
		WIN32_FIND_DATAW FindData = {};
		HANDLE hFind = ::FindFirstFileExW(FilePathW.Data, FindExInfoBasic, &FindData, FindExSearchNameMatch, nullptr, 0);
		TVector<FString> Paths;
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == 0)
					continue;

				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == L'.' && FindData.cFileName[2] == 0)
					continue;
				
				Lambda(FindData);
				if (FindData.dwFileAttributes & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_DIRECTORY))
					Paths.Emplace(Strings::FromWide(FindData.cFileName));
			}
			while (::FindNextFileW(hFind, &FindData));
			::FindClose(hFind);
		}
	}

	bool FDirectory::Exists(FStringView DirectoryPath)
	{
		DWORD FileAttribute = GetFileAttributesW(Strings::ToWide(DirectoryPath).Data);
		return (FileAttribute != INVALID_FILE_ATTRIBUTES) && ((FileAttribute & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
	}

	bool FDirectory::Create(FStringView DirectoryPath)
	{
		FStringView DirectoryParentPath = FPath::GetDirectory(DirectoryPath);
		if (!Exists(DirectoryParentPath))
			Create(DirectoryParentPath);
		
		FWString DirectoryPathW = Strings::ToWide(DirectoryPath);
		return !!::CreateDirectoryW(DirectoryPathW.Data, NULL);
	}
	
	TVector<FString> FDirectory::GetEntries(FStringView FilePath)
	{
		FWString FilePathW = Strings::ToWide(FPath::Combine(FilePath, Str("*.*")));
		TVector<FString> Paths;
		EnumPath(FilePathW, [&](const WIN32_FIND_DATAW & FindData)
			{
				if (FindData.dwFileAttributes & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_DIRECTORY))
					Paths.Emplace(FPath::Combine(FilePath, Strings::FromWide(FindData.cFileName)));
			});
		return Paths;
	}
	
	TVector<FString> FDirectory::GetFiles(FStringView FilePath)
	{
		FWString FilePathW = Strings::ToWide(FPath::Combine(FilePath, Str("*.*")));
		TVector<FString> Paths;
		EnumPath(FilePathW, [&](const WIN32_FIND_DATAW & FindData)
			{
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
					Paths.Emplace(FPath::Combine(FilePath, Strings::FromWide(FindData.cFileName)));
			});
		return Paths;
	}
	
	TVector<FString> FDirectory::GetDirectories(FStringView FilePath)
	{
		FWString FilePathW = Strings::ToWide(FPath::Combine(FilePath, Str("*.*")));
		TVector<FString> Paths;
		EnumPath(FilePathW, [&](const WIN32_FIND_DATAW & FindData)
			{
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					Paths.Emplace(FPath::Combine(FilePath, Strings::FromWide(FindData.cFileName)));
			});
		return Paths;
	}
}