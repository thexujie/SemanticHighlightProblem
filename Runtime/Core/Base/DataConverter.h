#pragma once

#include "Vector.h"

namespace XE
{
	class CORE_API IDataConverter
	{
	public:
		virtual ~IDataConverter() = default;
		virtual FVariant Convert(const IDataProvider * ValueProvider) const = 0;
	};
	
	class CORE_API IDataProvider : public IRefer
	{
	public:
		virtual ~IDataProvider() = default;

		virtual FStringView GetName() const = 0;
		virtual FVariant GetValue(FType Type) const = 0;
		
		virtual const IDataProvider * GetContent() const = 0;
		virtual TVector<const IDataProvider *> GetProperties() const = 0;
	};
}
