#pragma once

template<typename _Ty>
struct Vector2DBase
{
	_Ty x;
	_Ty y;
};

template<typename _Ty>
inline bool operator==(const Vector2DBase<_Ty>& left, const Vector2DBase<_Ty>& right)
{
	return left.x == right.x && left.y == right.y;
}

template<typename _Ty>
inline bool operator!=(const Vector2DBase<_Ty>& left, const Vector2DBase<_Ty>& right)
{
	return left.x != right.x && left.y != right.y;
}

template<typename _Ty>
inline Vector2DBase<_Ty>& operator++(Vector2DBase<_Ty>& vector2D)
{
	++vector2D.x;
	++vector2D.y;
	return vector2D;
}

template<typename _Ty>
inline Vector2DBase<_Ty> operator++(Vector2DBase<_Ty>& vector2D, int)
{
	Vector2DBase<_Ty> temp(vector2D);
	++vector2D.x;
	++vector2D.y;
	return temp;
}

template<typename _Ty>
inline  Vector2DBase<_Ty>& operator--(Vector2DBase<_Ty>& vector2D)
{
	--vector2D.x;
	--vector2D.y;
	return vector2D;
}

template<typename _Ty>
inline  Vector2DBase<_Ty> operator--(Vector2DBase<_Ty>& vector2D, int)
{
	Vector2DBase<_Ty> temp(vector2D);
	--vector2D.x;
	--vector2D.y;
	return temp;
}

using Vector2DI = Vector2DBase<int>;
using Vector2D = Vector2DBase<size_t>;
using Vector2DD = Vector2DBase<double>;