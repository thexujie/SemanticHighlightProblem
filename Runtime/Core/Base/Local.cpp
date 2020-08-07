#include "PCH.h"
#include "Local.h"

namespace XE
{
	namespace Locals
	{
		_locale_t Ansi()
		{
			return _create_locale(LC_CTYPE, ".ACP");
		}

		_locale_t Local()
		{
			return _get_current_locale();
		}
		
		_locale_t Utf8()
		{
			return _create_locale(LC_CTYPE, ".UTF8");
		}
	}
}
