#pragma once

namespace XE
{
	template<typename T>
	class TReference
	{
	public:
		TReference() = delete;
		TReference & operator = (const TReference & That) = delete;
		
		TReference(T & ValueIn) : Value(ValueIn) {}
		TReference(const TReference &) = default;
		
		T & Value;
	};
}