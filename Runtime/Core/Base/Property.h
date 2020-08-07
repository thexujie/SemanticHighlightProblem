#pragma once

#include "Types.h"
#include "Type.h"

namespace XE
{
	enum class EPropertyFlag
	{
		None = 0,
	};
	using EPropertyFlags = TFlag<EPropertyFlag>;
	
	class IProperty : public IRefer
	{
	public:
		virtual ~IProperty() = default;
		virtual EPropertyFlags GetPropertyFlags() const = 0;
		virtual FType GetPropertyType() const = 0;

		virtual void SetValue(void * Instance, const FVariant & Variable) const = 0;
	};
}
