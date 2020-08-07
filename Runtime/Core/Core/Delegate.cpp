#include "PCH.h"
#include "Delegate.h"

namespace XE
{
	static thread_local EReply Reply = EReply::None;

	void FReply::None()
	{
		Reply = EReply::None;
	}

	void FReply::Break()
	{
		Reply = EReply::Break;
	}

	EReply FReply::State()
	{
		return Reply;
	}
}