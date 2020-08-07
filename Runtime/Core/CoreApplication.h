#pragma once

#include "Base/Base.h"
#include "Core/Object.h"
#include "Core/NativeType.h"

namespace XE
{
	class CORE_API IService : public FObject
	{
	public:
	};

	class CORE_API FCoreApplication
	{
	public:
		FCoreApplication();

		IService * GetService(const FGuid & Guid);

		template<typename T>
		T * GetService()
		{
			return static_cast<T *>(GetService(GuidOf<T>()));
		}

		void RegisterService(const FGuid & Guid, FType Type);

		template<typename T>
		void RegisterService()
		{
			RegisterService(GuidOf<T>(), TypeOf<T>());
		}

		void RegisterDataConverter(FType Type, const IDataConverter * DataConverter);

		template<typename T, typename DataConverterT>
		void RegisterDataConverter()
		{
			static DataConverterT DataConverter;
			RegisterDataConverter(TypeOf<T>(), &DataConverter);
		}
		
		const IDataConverter * GetDataConverter(FType Type);

		const TMap<FStringView, FType> & GetTypes() { return Types; }

		FType GetType(FStringView Name) const;
		
		void RegisterType(FType Type, TVectorView<FType> ParentTypes);

		template<typename T, typename ...ParentsT>
		void RegisterType()
		{
			static_assert(ConjunctionV<IsBaseOf<ParentsT, T>...>);
			static TNativeType<T> NativeType;
			Reflection(NativeType);
			RegisterType(FType(&NativeType), { ((TypeOf<ParentsT>()))... });
		}

	private:
		TMap<FGuid, TVector<FType>> ServiceTypes;
		TMap<FGuid, TSharedPtr<IService>> Services;
		TMap<FType, TPointer<const IDataConverter>> DataConverters;

		// static reflection, FStringView as Key by name pointer is OK
		TMap<FStringView, FType> Types;
		
	public:
		static FCoreApplication & Instance();
	};
}
