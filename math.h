#pragma once
#include "vector.h"

static const constexpr auto PI = 3.14159265358979323846f;
static const constexpr auto RADPI = 57.295779513082f;
static const constexpr auto PIRAD = 0.01745329251f;
static constexpr __forceinline auto DEG2RAD( float x ) { return x * PIRAD; }
static constexpr __forceinline auto RAD2DEG( float x ) { return x * RADPI; }

namespace math
{
	static constexpr float pi = 3.14159265358979323846f;
	static constexpr float radpi = 57.295779513082f;
	static constexpr float pirad = 0.01745329251f;

	__forceinline vec3 vector_angles( vec3& start, vec3& end )
	{
		vec3 direction = end - start;

		float pitch = RAD2DEG( atan2f( -direction.z, direction.length2d( ) ) );
		float yaw = RAD2DEG( atan2f( direction.y, direction.x ) );

		vec3 angle( pitch, yaw, 0.0f );
		angle.sanitize( );
		return angle;
	}

	__forceinline vec3 angle_vectors( vec3 angles )
	{
		float sp, sy, cp, cy;
		sp = sinf( angles.x * PIRAD );
		cp = cosf( angles.x * PIRAD );
		sy = sinf( angles.y * PIRAD );
		cy = cosf( angles.y * PIRAD );

		return vec3( cp * cy, cp * sy, -sp );
	}

	__forceinline void vector_transform( vec3& in1, const matrix3x4& in2, vec3 &out )
	{
		out.x = in1.dot( in2[0] ) + in2[0][3];
		out.y = in1.dot( in2[1] ) + in2[1][3];
		out.z = in1.dot( in2[2] ) + in2[2][3];
	}
};