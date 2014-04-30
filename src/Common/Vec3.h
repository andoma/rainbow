// Copyright (c) 2010-14 Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef COMMON_VEC3_H_
#define COMMON_VEC3_H_

#include "Common/Algorithm.h"

namespace Rainbow
{
	/// Structure for storing a three-dimensional vector.
	template<typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3() : x(0), y(0), z(0) { }
		Vec3(const T x, const T y, const T z) : x(x), y(y), z(z) { }

		/// Returns whether the vector is zero.
		bool is_zero() const
		{
			return !this->x && !this->y && !this->z;
		}

		Vec3<T>& operator+=(const Vec3<T> &v)
		{
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			return *this;
		}

		Vec3<T>& operator-=(const Vec3<T> &v)
		{
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			return *this;
		}

		Vec3<T>& operator*=(const T &f)
		{
			this->x *= f;
			this->y *= f;
			this->z *= f;
			return *this;
		}
	};

	template<>
	inline bool Vec3<double>::is_zero() const
	{
		return isequal(0.0, this->x) && isequal(0.0, this->y) && isequal(0.0, this->z);
	}

	template<>
	inline bool Vec3<float>::is_zero() const
	{
		return isequal(0.0f, this->x) && isequal(0.0f, this->y) && isequal(0.0f, this->z);
	}
}

typedef Rainbow::Vec3<double> Vec3d;
typedef Rainbow::Vec3<float> Vec3f;

#endif
