#pragma once

#include "IOInc.h"
#include "Path.h"

namespace XE
{
	class CORE_API FDirectory
	{
	public:


	public:
		static bool Exists(FStringView DirectoryPath);
		static bool Create(FStringView DirectoryPath);
		
		static TVector<FString> GetEntries(FStringView FilePath);
		static TVector<FString> GetFiles(FStringView FilePath);
		static TVector<FString> GetDirectories(FStringView FilePath);
	};
}
