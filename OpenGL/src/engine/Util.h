#pragma once
#include <functional>
#include <map>
#include <unordered_map>

namespace Util
{
	// i stole this
	template <typename ReturnType, typename... Args>
	std::function<ReturnType(Args...)> memoize(const std::function<ReturnType(Args...)>& func)
	{
		std::map<std::tuple<Args...>, ReturnType> cache;

		return [cache, func](Args... args) mutable
		{
			const std::tuple<Args...> t(args...);

			if (!cache.contains(t))
			{
				cache[t] = func(args...);
			}

			return cache.at(t);
		};
	}
}
