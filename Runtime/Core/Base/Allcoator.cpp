#include "PCH.h"
#include "Allcoator.h"

namespace XE
{
	class CORE_API FDefaultMalloc : public IMalloc
	{
	public:
		FDefaultMalloc() = default;
		~FDefaultMalloc() = default;

		void * Malloc(size_t Size, size_t Alignment) override
		{
			assert(Size > 0);
			if (Alignment)
				return ::_aligned_malloc(Size, Alignment);
			else
				return ::malloc(Size);
			//auto * ptr =  ::malloc(Size);
			//byte_t * byteptr = (byte_t *)ptr;
			//if (byteptr[0] == 0xc5 && byteptr[1] == 0xb7)
			//	assert(0);
			//return byteptr;
		}
		
		void Free(void * Pointer, size_t Alignment) override
		{
			if (Alignment)
				::_aligned_free(Pointer);
			else
				::free(Pointer);
		}
	};
	
	static FDefaultMalloc GAllocator;

	IMalloc * GetMalloc()
	{
		return &GAllocator;
	}
	
	void * Malloc(size_t Size, size_t Alignment)
	{
		return GAllocator.Malloc(Size, Alignment);
	}

	void Free(void * Pointer, size_t Alignment)
	{
		GAllocator.Free(Pointer, Alignment);
	}
}
