#include "LetArrayMotion.h"

template<typename... Ts>
inline bool LetArrayMotion::ArrayAppends(LetObject* var, LetObject* va1, Ts... args)
{
	bool result = ArrayAppend(var, va1);

	if (result == false) {
		return result;
	}

	if (sizeof...(args)) {
		result = ArrayAppends(var, args...);
	}
	return result;
}