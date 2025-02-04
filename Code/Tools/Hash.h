#pragma once
#include <utility>
#include <functional>

struct pair_hash
{
	template<typename _Ty1, typename _Ty2>
	std::size_t operator()(const std::pair<_Ty1, _Ty2>& pair) const
	{
		return std::hash<_Ty1>()(pair.first) ^ std::hash<_Ty2>()(pair.second);
	}
};