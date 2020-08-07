#include "PCH.h"
#include "CoreApplication.h"

namespace XE
{
	static FCoreApplication * GCoreApplicationPtr = nullptr;
	
	FType TypeBy(const Char * Name)
	{
		return FCoreApplication::Instance().GetType(Name);
	}
	
	FCoreApplication::FCoreApplication()
	{
		GCoreApplicationPtr = this;
		
		RegisterType<char8_t>();
		RegisterType<char16_t>();
		RegisterType<char32_t>();
		
		RegisterType<int8_t>();
		RegisterType<uint8_t>();
		RegisterType<int16_t>();
		RegisterType<uint16_t>();
		RegisterType<int32_t>();
		RegisterType<uint32_t>();
		RegisterType<int64_t>();
		RegisterType<uint64_t>();

		RegisterType<FColor32>();
		RegisterType<FStringView>();
		RegisterType<FString>();

		RegisterType<TVec4<int16_t>>();
		RegisterType<TVec4<uint16_t>>();
		RegisterType<TVec4<int32_t>>();
		RegisterType<TVec4<uint32_t>>();
		RegisterType<TVec4<int64_t>>();
		RegisterType<TVec4<uint64_t>>();
		RegisterType<TVec4<float32_t>>();
		RegisterType<TVec4<float64_t>>();
	}
	
	FCoreApplication & FCoreApplication::Instance()
	{
		return *GCoreApplicationPtr;
	}

	IService * FCoreApplication::GetService(const FGuid & Guid)
	{
		auto ServiceIter = Services.Find(Guid);
		if (ServiceIter != Services.End())
			return ServiceIter->Second.Get();
		
		auto ServiceTypeIter = ServiceTypes.Find(Guid);
		if (ServiceTypeIter == ServiceTypes.End())
			throw EError::NotImplemented;

		FType Type = ServiceTypeIter->Second.Front();
		IService * Pointer = (IService *)Malloc(Type.GetSize(), Type.GetAlignment());
		Type.Construct(Pointer);
		TSharedPtr<IService> Service = TSharedPtr<IService>(Pointer);
		Services[Guid] = Service;
		return Service.Get();
	}


	void FCoreApplication::RegisterType(FType Type, TVectorView<FType> ParentTypes)
	{
		if (Types.Containts(Type.GetName()))
			throw EError::Exists;

		if (!ParentTypes.IsEmpty())
			Type.GetNativeType()->SetParent(ParentTypes[0].GetNativeType());
		
		Types[Type.GetName()] = Type;
		for (FType ParentType : ParentTypes)
			ParentType.GetNativeType()->AppendChild(Type.GetNativeType());
	}

	FType FCoreApplication::GetType(FStringView Name) const
	{
		auto TypeIter = Types.Find(Name);
		if (TypeIter != Types.End())
			return TypeIter->Second;
		return FType::Unkown;
	}
	
	void FCoreApplication::RegisterService(const FGuid & Guid, FType Type)
	{
		ServiceTypes[Guid].Add(Type);
	}

	void FCoreApplication::RegisterDataConverter(FType Type, const IDataConverter * DataConverter)
	{
		if (DataConverters.Containts(Type))
			throw EError::Exists;
		DataConverters[Type] = DataConverter;
	}
	
	const IDataConverter * FCoreApplication::GetDataConverter(FType Type)
	{
		auto Iter = DataConverters.Find(Type);
		if (Iter == DataConverters.End())
		{
			for (auto DataConverterIter = DataConverters.Begin(); DataConverterIter != DataConverters.End(); ++DataConverterIter)
			{
				if (Type.IsChildOf(DataConverterIter->First))
					return DataConverterIter->Second.Get();
			}
			return nullptr;
		}
		return Iter->Second.Get();
	}
}
