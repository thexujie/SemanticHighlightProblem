#include "PCH.h"
#include "Guid.h"

#ifdef XE_WINDOWS
#include <guiddef.h>
#pragma comment(lib, "Bcrypt.lib")
#endif

namespace XE
{
	FGuid FGuid::Empty = {};
	
#ifdef XE_WINDOWS
	FGuid::FGuid(const struct _GUID & GUID)
	{
		Data1 = GUID.Data1;
		Data2 = GUID.Data2;
		Data3 = GUID.Data3;
		ArrayCopy(Data4, GUID.Data4);
	}
	
	FGuid FGuid::Generate()
	{
		FGuid Guid;
		BCRYPT_ALG_HANDLE hAlgorithm = NULL;
		NTSTATUS Status =
			BCryptOpenAlgorithmProvider(
				&hAlgorithm,
				BCRYPT_RNG_ALGORITHM,
				NULL,
				0);
		if (Status < 0)
			return Empty;
		
		BCryptGenRandom(hAlgorithm, reinterpret_cast<PUCHAR>(&Guid), sizeof(FGuid), 0);
		BCryptCloseAlgorithmProvider(hAlgorithm, 0);
		return Guid;
	}
#endif
}
