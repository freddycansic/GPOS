#pragma once
#include <functional>
#include <map>

namespace Util
{
	template <typename ReturnType, typename... Args>
	std::function<ReturnType(Args...)> memoize(std::function<ReturnType(Args...)> func)
	{
		std::map<std::tuple<Args...>, ReturnType> cache;

		return [cache, func](Args... args) mutable
		{
			std::tuple<Args...> t(args...);

			if (!cache.contains(t))
			{
				cache[t] = func(args...);
			}

			return cache.at(t);
		};
	}
}
