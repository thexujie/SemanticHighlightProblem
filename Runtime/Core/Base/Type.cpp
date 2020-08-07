#include "PCH.h"
#include "Type.h"

namespace XE
{
	FType FType::Unkown;

	FStringView FType::GetShortName() const
	{
		FStringView Name = GetName();
		if (size_t Index = Name.FindLastOf(Str(" :")); Index != NullIndex)
			return Name.Slice(Index + 1);
		return Name;
	}

	bool INativeType::IsParentOf(const INativeType * Another) const
	{
		if (!Another)
			return false;

		const INativeType * NativeType = GetFirstChild();
		while (NativeType)
		{
			if (NativeType == Another || Another->IsParentOf(NativeType))
				return true;
			NativeType = NativeType->GetNextSibling();
		}
		return false;
	}

	bool INativeType::IsChildOf(const INativeType * Another) const
	{
		if (!Another)
			return false;
		
		const INativeType * NativeType = Another->GetFirstChild();
		while (NativeType)
		{
			if (NativeType == this || IsChildOf(NativeType))
				return true;
			NativeType = NativeType->GetNextSibling();
		}
		return false;
	}
}
