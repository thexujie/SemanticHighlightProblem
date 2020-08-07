#pragma once

#include "Types.h"

namespace XE
{
	template<typename T>
	class TVectorView
	{
	public:
		const T * Data = nullptr;
		size_t Size = 0;

	public:
		constexpr TVectorView() = default;
		constexpr TVectorView(const TVectorView & That) : Data(That.Data), Size(That.Size) {}

		TVectorView(TVectorView && That) noexcept : Data(That.Data), Size(That.Size)
		{
			That.Data = nullptr;
			That.Size = 0;
		}

		constexpr TVectorView(TInitializerList<T> InitList) : Data(InitList.begin()), Size(InitList.size()) {}
		constexpr TVectorView(const T * DataIn, size_t SizeIn) : Data(DataIn), Size(SizeIn) {}
		constexpr TVectorView(NullPtrT) : Data(nullptr), Size(0) {}

		template<size_t FixedSize>
		constexpr TVectorView(const T (&DataIn)[FixedSize]) : Data(DataIn), Size(FixedSize) {}

		size_t GetSize() const { return Size; }
		const T * GetData() const { return Data; }
		bool IsEmpty() const { return Size == 0; }
		const T & operator[](size_t Index) const { return Data[Index]; }

		TVectorView & operator=(const TVectorView & That)
		{
			Data = That.Data;
			Size = That.Size;
			return *this;
		}

		const T * begin() const { return Data; }
		const T * end() const { return Data + Size; }
	};
}
