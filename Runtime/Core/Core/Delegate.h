#pragma once

#include "CoreInc.h"

namespace XE
{
	template<typename T>
	class FDelegate;

	template<typename ReturnT, typename ...ParametersT>
	class FDelegate<ReturnT(ParametersT...)>
	{
	public:
		FDelegate() = default;
		~FDelegate() = default;

		template<typename CallableT, typename ...CallableParametersT>
		void Bind(CallableT && Callable, CallableParametersT && ...CallableParameters)
		{
			Function = TFunction<ReturnT(ParametersT...)>(Callable, Forward<CallableParametersT>(CallableParameters)...);
		}

		bool IsBound() const { return !!Function; }

		template<typename ...ArgsT>
		ReturnT Excute(ArgsT && ...Args)
		{
			if (!Function)
				return {};
			return Function(Forward<ArgsT>(Args)...);
		}

		TFunction< ReturnT(ParametersT...)> Function;
	};

	enum class EReply
	{
		None = 0,
		Break,
	};
	
	using FEventHandle = ptr_t;

	struct CORE_API FReply
	{
		static void None();
		
		static void Break();

		static EReply State();
	};

	template<typename ...ParametersT>
	class FEvent
	{
	public:
		FEvent() = default;
		~FEvent() = default;

		template<typename CallableT, typename ...CallableParametersT>
		FEventHandle AddBind(CallableT && Callable, CallableParametersT && ...CallableParameters)
		{
			auto & Function = Functions.Emplace(TFunction<EReply(ParametersT...)>(Callable, Forward<CallableParametersT>(CallableParameters)...));
			return FEventHandle(Function.Storage);
		}

		void UnBind(FEventHandle EventHandle)
		{
			auto iter = FindIf(Functions, [&](auto & Function) { return FEventHandle(Function.Storage) == EventHandle; });
			Functions.Erase(iter);
		}
		
		bool IsBound() const { return !Functions.IsEmpty(); }
		
		template<typename ...ArgsT>
		void Excute(ArgsT && ...Args)
		{
			FReply::None();
			for (auto & Function : Functions)
			{
				assert(!!Function);
				EReply Reply = Function(Forward<ArgsT>(Args)...);
				if (Reply == EReply::Break)
					break;
			}
		}

		TVector<TFunction<EReply(ParametersT...)>> Functions;
	};
}
