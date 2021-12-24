#pragma once
#include <cmath>
#include <algorithm>

using matrix3x4 = float[3][4];
using matrix4x4 = float[4][4];

class vec2
{
public:
	vec2( )
	{
		x = y = 0.0f;
	}

	vec2( float X, float Y )
	{
		x = X;
		y = Y;
	}

	vec2( float* v )
	{
		x = v[0];
		y = v[1];
	}

	vec2( const float* v )
	{
		x = v[0];
		y = v[1];
	}

	vec2( const vec2& v )
	{
		x = v.x;
		y = v.y;
	}

	vec2& operator=( const vec2& v )
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	float& operator[]( int i )
	{
		return reinterpret_cast<float*>( this )[i];
	}

	float operator[]( int i ) const
	{
		return ( (float*)this )[i];
	}

	vec2& operator+=( const vec2& v )
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	vec2& operator-=( const vec2& v )
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec2& operator*=( const vec2& v )
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	vec2& operator/=( const vec2& v )
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	vec2& operator+=( float v )
	{
		x += v;
		y += v;
		return *this;
	}

	vec2& operator-=( float v )
	{
		x -= v;
		y -= v;
		return *this;
	}

	vec2& operator*=( float v )
	{
		x *= v;
		y *= v;
		return *this;
	}

	vec2& operator/=( float v )
	{
		x /= v;
		y /= v;
		return *this;
	}

	vec2 operator+( const vec2& v ) const
	{
		return vec2( x + v.x, y + v.y );
	}

	vec2 operator-( const vec2& v ) const
	{
		return vec2( x - v.x, y - v.y );
	}

	vec2 operator*( const vec2& v ) const
	{
		return vec2( x * v.x, y * v.y );
	}

	vec2 operator/( const vec2& v ) const
	{
		return vec2( x / v.x, y / v.y );
	}

	vec2 operator+( float v ) const
	{
		return vec2( x + v, y + v );
	}

	vec2 operator-( float v ) const
	{
		return vec2( x - v, y - v );
	}

	vec2 operator*( float v ) const
	{
		return vec2( x * v, y * v );
	}

	vec2 operator/( float v ) const
	{
		return vec2( x / v, y / v );
	}

	void set( float X = 0.0f, float Y = 0.0f )
	{
		x = X;
		y = Y;
	}

	float length( void ) const
	{
		return sqrtf( x * x + y * y );
	}

	float length_sqr( void ) const
	{
		return ( x * x + y * y );
	}

	float dist( const vec2& v ) const
	{
		return ( *this - v ).length( );
	}

	float dist_sqr( const vec2& v ) const
	{
		return ( *this - v ).length_sqr( );
	}

	float dot( const vec2& v ) const
	{
		return ( x * v.x + y * v.y );
	}

	bool is_zero( void ) const
	{
		return ( x > -0.01f && x < 0.01f &&
				 y > -0.01f && y < 0.01f );
	}

public:
	float x, y;
};

class vec3
{
public:
	vec3( )
	{
		x = y = z = 0.0f;
	}

	vec3( float X, float Y, float Z )
	{
		x = X;
		y = Y;
		z = Z;
	}

	vec3( float* v )
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}

	vec3( const float* v )
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}

	vec3( const vec3& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	vec3( const vec2& v )
	{
		x = v.x;
		y = v.y;
		z = 0.0f;
	}

	vec3& operator=( const vec3& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	vec3& operator=( const vec2& v )
	{
		x = v.x;
		y = v.y;
		z = 0.0f;
		return *this;
	}

	float& operator[]( int i )
	{
		return ( (float*)this )[i];
	}

	float operator[]( int i ) const
	{
		return ( (float*)this )[i];
	}

	vec3& operator+=( const vec3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3& operator-=( const vec3& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	vec3& operator*=( const vec3& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	vec3& operator/=( const vec3& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	vec3& operator+=( float v )
	{
		x += v;
		y += v;
		z += v;
		return *this;
	}

	vec3& operator-=( float v )
	{
		x -= v;
		y -= v;
		z -= v;
		return *this;
	}

	vec3& operator*=( float v )
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}

	vec3& operator/=( float v )
	{
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}

	vec3 operator+( const vec3& v ) const
	{
		return vec3( x + v.x, y + v.y, z + v.z );
	}

	vec3 operator-( const vec3& v ) const
	{
		return vec3( x - v.x, y - v.y, z - v.z );
	}

	vec3 operator*( const vec3& v ) const
	{
		return vec3( x * v.x, y * v.y, z * v.z );
	}

	vec3 operator/( const vec3& v ) const
	{
		return vec3( x / v.x, y / v.y, z / v.z );
	}

	vec3 operator+( float v ) const
	{
		return vec3( x + v, y + v, z + v );
	}

	vec3 operator-( float v ) const
	{
		return vec3( x - v, y - v, z - v );
	}

	vec3 operator*( float v ) const
	{
		return vec3( x * v, y * v, z * v );
	}

	vec3 operator/( float v ) const
	{
		return vec3( x / v, y / v, z / v );
	}

	bool operator== ( const vec3& other ) const
	{
		return x == other.x && y == other.y && other.z == other.z;
	}

	bool operator!= ( const vec3& other ) const
	{
		return !( *this == other );
	}

	void set( float X = 0.0f, float Y = 0.0f, float Z = 0.0f )
	{
		x = X;
		y = Y;
		z = Z;
	}

	float length( ) const
	{
		return sqrtf( x * x + y * y + z * z );
	}

	float length_sqr( ) const
	{
		return ( x * x + y * y + z * z );
	}

	float length2d( ) const
	{
		return sqrtf( x * x + y * y );
	}

	float length2d_sqr( ) const
	{
		return ( x * x + y * y );
	}

	float dist( const vec3& v ) const
	{
		return ( *this - v ).length( );
	}

	float dist_sqr( const vec3& v ) const
	{
		return ( *this - v ).length_sqr( );
	}

	float dot( const vec3& v ) const
	{
		return ( x * v.x + y * v.y + z * v.z );
	}

	vec3 Cross( const vec3& v ) const
	{
		return vec3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	bool is_zero( ) const
	{
		return ( x > -0.01f && x < 0.01f &&
				 y > -0.01f && y < 0.01f &&
				 z > -0.01f && z < 0.01f );
	}

	bool is_nan( )
	{
		return isnan( x ) || isnan( y ) || isnan( z );
	}

	void normalize( )
	{
		while ( x > 180.f )
			x -= 360.f;

		while ( x < -180.f )
			x += 360.f;

		while ( y > 180.f )
			y -= 360.f;

		while ( y < -180.f )
			y += 360.f;
	}

	void clamp( )
	{
		x = std::clamp<float>( x, -89.f, 89.f );
		y = std::clamp<float>( y, -180.0f, 180.0f );
		z = 0.0f;
	}

	void sanitize( )
	{
		//safety
		if ( !isfinite( x ) || isnan( x ) || isinf( x ) )
			x = 0.0f;

		if ( !isfinite( y ) || isnan( y ) || isinf( y ) )
			y = 0.0f;

		//normalize
		normalize( );

		//clamp
		clamp( );
	}

	void normalize_in_place( )
	{
		vec3& v = *this;

		float iradius = 1.f / ( this->length( ) + FLT_EPSILON );

		v.x *= iradius;
		v.y *= iradius;
		v.z *= iradius;
	}

	vec3 euler_angle( )
	{
		float pitch, yaw, length;

		length = length2d( );

		if ( length > 0.f )
		{
			pitch = ( atan2( -z, length ) * 180.f / 3.14159265358979323846f );

			if ( pitch < 0 )
			{
				pitch += 360;
			}

			yaw = ( atan2( y, x ) * 180.f / 3.14159265358979323846f );

			if ( yaw < 0.f )
			{
				yaw += 360;
			}
		}
		else
		{
			pitch = ( z > 0.0f ) ? 270.f : 90.f;
			yaw = 0;
		}

		return vec3( pitch, yaw, 0 );
	}

	bool IsZero( )
	{
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}
public:
	float x, y, z;
};