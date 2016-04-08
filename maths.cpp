#include <iostream>
#include <math.h>
using namespace std;

#include "maths.h"

//-----------------------------------------------------------------------------
// Vector Functions
//-----------------------------------------------------------------------------
float Vector3::GetLength()
{
	return sqrt( x*x + y*y + z*z );
}

void Vector3::Normalise()
{
	float length = GetLength();

	if (length > 0.f)
	{
		x /= length;
		y /= length;
		z /= length;
	}
}

D3DXVECTOR3	Vector3::DXVector3()
{
	D3DXVECTOR3 result;

	result.x = x;
	result.y = y;
	result.z = z;

	return result;
}

Vector3 operator +( const Vector3 &a, const Vector3 &b )
{
	Vector3 result(0, 0, 0);

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

Vector3 operator -( const Vector3 &a, const Vector3 &b )
{
	Vector3 result(0, 0, 0);

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}

float DotProduct( const Vector3 &a, const Vector3 &b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 CrossProduct( const Vector3 &a, const Vector3 &b )
{
	Vector3 result(0, 0, 0);

	result.x = ( a.y * b.z ) - ( a.z * b.y );
	result.y = -( ( a.x * b.z ) - ( a.z * b.x ) );
	result.z = ( a.x * b.y ) - ( a.y * b.x );

	return result;
}

float GetAngle( Vector3 &a, Vector3 &b )
{
	float lengthA = a.GetLength();
	float lengthB = b.GetLength();
	float dot = DotProduct( a, b );

	return acos( dot / (lengthA * lengthB) );
}


//-----------------------------------------------------------------------------
// Matrix4x4 Functions [4x4]
//-----------------------------------------------------------------------------
void Matrix4x4::Identity()
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if ( x == y )
			{
				m[x][y] = 1;
			}
			else
			{
				m[x][y] = 0;
			}
		}
	}
}

D3DXMATRIXA16 Matrix4x4::DXMatrix()
{
	D3DXMATRIXA16 result;

	result._11 = m[0][0];	result._12 = m[1][0];	result._13 = m[2][0];	result._14 = m[3][0];
	result._21 = m[0][1];	result._22 = m[1][1];	result._23 = m[2][1];	result._24 = m[3][1];
	result._31 = m[0][2];	result._32 = m[1][2];	result._33 = m[2][2];	result._34 = m[3][2];
	result._41 = m[0][3];	result._42 = m[1][3];	result._43 = m[2][3];	result._44 = m[3][3];
	
	return result;
}

Matrix4x4 Multiply( Matrix4x4 &a, Matrix4x4 &b )
{
	Matrix4x4 result;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			result.m[y][x] = 
				a.m[y][0] * b.m[0][x] + 
				a.m[y][1] * b.m[1][x] + 
				a.m[y][2] * b.m[2][x] + 
				a.m[y][3] * b.m[3][x];
		}
	}

	return result;
}

Matrix4x4 Transpose( Matrix4x4 &a )
{
	Matrix4x4 result;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			result.m[x][y] = NULL;
		}
	}

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if (result.m[x][y] == NULL)
				result.m[x][y] = a.m[y][x];
		}
	}

	return result;
}

Matrix4x4 Scale( Matrix4x4 &a, float scaleValue )
{
	Matrix4x4 result;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			result.m[x][y] = scaleValue * a.m[x][y];
		}
	}

	return result;
}

//-----------------------------------------------------------------------------
// Note: Determinant for 3 by 3
//-----------------------------------------------------------------------------
float Sarrus( Matrix4x4 &a, int x, int y )
{
	switch (y)
	{
	case 0:
		switch (x)
		{
		case 0:
			return ( a.m[1][1] * a.m[2][2] * a.m[3][3] +
				a.m[2][1] * a.m[3][2] * a.m[1][3] +
				a.m[3][1] * a.m[1][2] * a.m[2][3] -
				a.m[1][1] * a.m[3][2] * a.m[2][3] -
				a.m[2][1] * a.m[1][2] * a.m[3][3] -
				a.m[3][1] * a.m[2][2] * a.m[1][3] );
		case 1:
			return ( a.m[0][1] * a.m[2][2] * a.m[3][3] +
				a.m[2][1] * a.m[3][2] * a.m[0][3] +
				a.m[3][1] * a.m[0][2] * a.m[2][3] -
				a.m[0][1] * a.m[3][2] * a.m[2][3] -
				a.m[2][1] * a.m[0][2] * a.m[3][3] -
				a.m[3][1] * a.m[2][2] * a.m[0][3] );
		case 2:
			return ( a.m[0][1] * a.m[1][2] * a.m[3][3] +
				a.m[1][1] * a.m[3][2] * a.m[0][3] +
				a.m[3][1] * a.m[0][2] * a.m[1][3] -
				a.m[0][1] * a.m[3][2] * a.m[1][3] -
				a.m[1][1] * a.m[0][2] * a.m[3][3] -
				a.m[3][1] * a.m[1][2] * a.m[0][3] );
		case 3:
			return ( a.m[0][1] * a.m[1][2] * a.m[2][3] +
				a.m[1][1] * a.m[2][2] * a.m[0][3] +
				a.m[2][1] * a.m[0][2] * a.m[1][3] -
				a.m[0][1] * a.m[2][2] * a.m[1][3] -
				a.m[1][1] * a.m[0][2] * a.m[2][3] -
				a.m[2][1] * a.m[1][2] * a.m[0][3] );
		default:
			break;
		}

	case 1:
		switch (x)
		{
		case 0:
			return ( a.m[1][0] * a.m[2][2] * a.m[3][3] +
				a.m[2][0] * a.m[3][2] * a.m[1][3] +
				a.m[3][0] * a.m[1][2] * a.m[2][3] -
				a.m[1][0] * a.m[3][2] * a.m[2][3] -
				a.m[2][0] * a.m[1][2] * a.m[3][3] -
				a.m[3][0] * a.m[2][2] * a.m[1][3] );
		case 1:
			return ( a.m[0][0] * a.m[2][2] * a.m[3][3] +
				a.m[2][0] * a.m[3][2] * a.m[0][3] +
				a.m[3][0] * a.m[0][2] * a.m[2][3] -
				a.m[0][0] * a.m[3][2] * a.m[2][3] -
				a.m[2][0] * a.m[0][2] * a.m[3][3] -
				a.m[3][0] * a.m[2][2] * a.m[0][3] );
		case 2:
			return ( a.m[0][0] * a.m[1][2] * a.m[3][3] +
				a.m[1][0] * a.m[3][2] * a.m[0][3] +
				a.m[3][0] * a.m[0][2] * a.m[1][3] -
				a.m[0][0] * a.m[3][2] * a.m[1][3] -
				a.m[1][0] * a.m[0][2] * a.m[3][3] -
				a.m[3][0] * a.m[1][2] * a.m[0][3] );
		case 3:
			return ( a.m[0][0] * a.m[1][2] * a.m[2][3] +
				a.m[1][0] * a.m[2][2] * a.m[0][3] +
				a.m[2][0] * a.m[0][2] * a.m[1][3] -
				a.m[0][0] * a.m[2][2] * a.m[1][3] -
				a.m[1][0] * a.m[0][2] * a.m[2][3] -
				a.m[2][0] * a.m[1][2] * a.m[0][3] );
		default:
			break;
		}

	case 2:
		switch (x)
		{
		case 0:
			return ( a.m[1][0] * a.m[2][1] * a.m[3][3] +
				a.m[2][0] * a.m[3][1] * a.m[1][3] +
				a.m[3][0] * a.m[1][1] * a.m[2][3] -
				a.m[1][0] * a.m[3][1] * a.m[2][3] -
				a.m[2][0] * a.m[1][1] * a.m[3][3] -
				a.m[3][0] * a.m[2][1] * a.m[1][3] );
		case 1:
			return ( a.m[0][0] * a.m[2][1] * a.m[3][3] +
				a.m[2][0] * a.m[3][1] * a.m[0][3] +
				a.m[3][0] * a.m[0][1] * a.m[2][3] -
				a.m[0][0] * a.m[3][1] * a.m[2][3] -
				a.m[2][0] * a.m[0][1] * a.m[3][3] -
				a.m[3][0] * a.m[2][1] * a.m[0][3] );
		case 2:
			return ( a.m[0][0] * a.m[1][1] * a.m[3][3] +
				a.m[1][0] * a.m[3][1] * a.m[0][3] +
				a.m[3][0] * a.m[0][1] * a.m[1][3] -
				a.m[0][0] * a.m[3][1] * a.m[1][3] -
				a.m[1][0] * a.m[0][1] * a.m[3][3] -
				a.m[3][0] * a.m[1][1] * a.m[0][3] );
		case 3:
			return ( a.m[0][0] * a.m[1][1] * a.m[2][3] +
				a.m[1][0] * a.m[2][1] * a.m[0][3] +
				a.m[2][0] * a.m[0][1] * a.m[1][3] -
				a.m[0][0] * a.m[2][1] * a.m[1][3] -
				a.m[1][0] * a.m[0][1] * a.m[2][3] -
				a.m[2][0] * a.m[1][1] * a.m[0][3] );
		default:
			break;
		}

	case 3:
		switch (x)
		{
		case 0:
			return ( a.m[1][0] * a.m[2][1] * a.m[3][2] +
				a.m[2][0] * a.m[3][1] * a.m[1][2] +
				a.m[3][0] * a.m[1][1] * a.m[2][2] -
				a.m[1][0] * a.m[3][1] * a.m[2][2] -
				a.m[2][0] * a.m[1][1] * a.m[3][2] -
				a.m[3][0] * a.m[2][1] * a.m[1][2] );
		case 1:
			return ( a.m[0][0] * a.m[2][1] * a.m[3][2] +
				a.m[2][0] * a.m[3][1] * a.m[0][2] +
				a.m[3][0] * a.m[0][1] * a.m[2][2] -
				a.m[0][0] * a.m[3][1] * a.m[2][2] -
				a.m[2][0] * a.m[0][1] * a.m[3][2] -
				a.m[3][0] * a.m[2][1] * a.m[0][2] );
		case 2:
			return ( a.m[0][0] * a.m[1][1] * a.m[3][2] +
				a.m[1][0] * a.m[3][1] * a.m[0][2] +
				a.m[3][0] * a.m[0][1] * a.m[1][2] -
				a.m[0][0] * a.m[3][1] * a.m[1][2] -
				a.m[1][0] * a.m[0][1] * a.m[3][2] -
				a.m[3][0] * a.m[1][1] * a.m[0][2] );
		case 3:
			return ( a.m[0][0] * a.m[1][1] * a.m[2][2] +
				a.m[1][0] * a.m[2][1] * a.m[0][2] +
				a.m[2][0] * a.m[0][1] * a.m[1][2] -
				a.m[0][0] * a.m[2][1] * a.m[1][2] -
				a.m[1][0] * a.m[0][1] * a.m[2][2] -
				a.m[2][0] * a.m[1][1] * a.m[0][2] );
		default:
			break;
		}
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// Note: Determinant for 4 by 4
//-----------------------------------------------------------------------------
float Determinant( Matrix4x4 &a )
{
	return ( a.m[0][0] * Sarrus(a, 0, 0) - 
		a.m[1][0] * Sarrus(a, 1, 0) + 
		a.m[2][0] * Sarrus(a, 2, 0) - 
		a.m[3][0] * Sarrus(a, 3, 0) );
}

float Minor( Matrix4x4 &a, int x, int y )
{
	return Sarrus(a, x, y);
}

Matrix4x4 Cofactor( Matrix4x4 &a )
{
	Matrix4x4 result;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if ( (x % 2 == 0 && y % 2 == 1) ||
				(x % 2 == 1 && y % 2 == 0) )
			{
				result.m[x][y] = -Sarrus(a, x, y);
			}
			else
			{
				result.m[x][y] = Sarrus(a, x, y);
			}
		}
	}

	return result;
}

Matrix4x4 Inverse( Matrix4x4 &a )
{
	float scaleValue = 1 / Determinant(a);
	
	return ( Scale( Transpose( Cofactor(a) ), scaleValue ) );
}

//			      |fgh| 	        |egh|			  |efh|		        |efg|
//	|A| = a x ( + |jkl| ) + b x ( - |ikl| ) + c x ( + |ijl| ) + d x ( - |ijk| )
//	              |nop|	            |mop|			  |mnp|		        |mno|
//	
//	
//	|fgh|
//	|jkl| = fkp + gln + hjo - flo - gjp - hkn
//	|nop|

Matrix4x4 Pitch( float pitch )
{
	Matrix4x4 result;

	result.Identity();
	result.m[1][1] = result.m[2][2] = cosf( pitch );
	result.m[1][2] = -sinf( pitch );
	result.m[2][1] = sinf( pitch );
	result.m[0][0] = result.m[3][3] = 1;

	return result;
}

Matrix4x4 Yaw( float yaw )
{
	Matrix4x4 result;

	result.Identity();
	result.m[0][0] = result.m[2][2] = cosf( yaw );
	result.m[2][0] = -sinf( yaw );
	result.m[0][2] = sinf( yaw );
	result.m[1][1] = result.m[3][3] = 1;

	return result;
}

Matrix4x4 Roll( float roll )
{
	Matrix4x4 result;

	result.Identity();
	result.m[0][0] = result.m[1][1] = cosf( roll );
	result.m[1][0] = -sinf( roll );
	result.m[0][1] = sinf( roll );
	result.m[2][2] = result.m[3][3] = 1;

	return result;
}

Matrix4x4 Position( float x, float y, float z )
{
	Matrix4x4 result;

	result.Identity();
	result.m[0][3] = x;
	result.m[1][3] = y;
	result.m[2][3] = z;

	return result;
}

Matrix4x4 View( Vector3 viewerPosition, Vector3 viewerDirection, Vector3 upVector )
{
	Matrix4x4 result;

	Vector3 zAxis( viewerDirection - viewerPosition );
	zAxis.Normalise();
	Vector3 xAxis( CrossProduct( upVector, zAxis ) );
	xAxis.Normalise();
	Vector3 yAxis( CrossProduct( zAxis, xAxis ) );

	result.Identity();
	result.m[0][0] = xAxis.x;
	result.m[1][0] = yAxis.x;
	result.m[2][0] = zAxis.x;
	result.m[0][1] = xAxis.y;
	result.m[1][1] = yAxis.y;
	result.m[2][1] = zAxis.y;
	result.m[0][2] = xAxis.z;
	result.m[1][2] = yAxis.z;
	result.m[2][2] = zAxis.z;
	result.m[0][3] = -DotProduct( xAxis, viewerPosition );
	result.m[1][3] = -DotProduct( yAxis, viewerPosition );
	result.m[2][3] = -DotProduct( zAxis, viewerPosition );
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 Perspective( float fov, float aspectRatio, float fNear, float fFar )
{
	Matrix4x4 result;
	
	float yScale = ( 1 / tan( fov / 2 ) );		// cot( fov / 2 )
	float xScale = yScale / aspectRatio;
	
	result.Identity();
	result.m[0][0] = xScale;
	result.m[1][1] = yScale;
	result.m[2][2] = fFar / ( fFar - fNear );
	result.m[2][3] = ( -fNear * fFar) / ( fFar - fNear );
	result.m[3][2] = 1;
	result.m[3][3] = 0;

	return result;
}

Matrix4x4 operator *( Matrix4x4 &a, Matrix4x4 &b )
{
	Matrix4x4 result;

	//result.m[0][0] = ( a.m[0][0] * b.m[0][0] ) + ( a.m[1][0] * b.m[0][1] ) + ( a.m[2][0] * b.m[0][2] ) + ( a.m[3][0] * b.m[0][3] );
	//result.m[1][0] = ( a.m[0][0] * b.m[1][0] ) + ( a.m[1][0] * b.m[1][1] ) + ( a.m[2][0] * b.m[1][2] ) + ( a.m[3][0] * b.m[1][3] );
	//result.m[2][0] = ( a.m[0][0] * b.m[2][0] ) + ( a.m[1][0] * b.m[2][1] ) + ( a.m[2][0] * b.m[2][2] ) + ( a.m[3][0] * b.m[2][3] );
	//result.m[3][0] = ( a.m[0][0] * b.m[3][0] ) + ( a.m[1][0] * b.m[3][1] ) + ( a.m[2][0] * b.m[3][2] ) + ( a.m[3][0] * b.m[3][3] );
	
	//result.m[0][1] = ( a.m[0][1] * b.m[0][0] ) + ( a.m[1][1] * b.m[0][1] ) + ( a.m[2][1] * b.m[0][2] ) + ( a.m[3][1] * b.m[0][3] );
	//result.m[1][1] = ( a.m[0][1] * b.m[1][0] ) + ( a.m[1][1] * b.m[1][1] ) + ( a.m[2][1] * b.m[1][2] ) + ( a.m[3][1] * b.m[1][3] );
	//result.m[2][1] = ( a.m[0][1] * b.m[2][0] ) + ( a.m[1][1] * b.m[2][1] ) + ( a.m[2][1] * b.m[2][2] ) + ( a.m[3][1] * b.m[2][3] );
	//result.m[3][1] = ( a.m[0][1] * b.m[3][0] ) + ( a.m[1][1] * b.m[3][1] ) + ( a.m[2][1] * b.m[3][2] ) + ( a.m[3][1] * b.m[3][3] );
	
	//result.m[0][2] = ( a.m[0][2] * b.m[0][0] ) + ( a.m[1][2] * b.m[0][1] ) + ( a.m[2][2] * b.m[0][2] ) + ( a.m[3][2] * b.m[0][3] );
	//result.m[1][2] = ( a.m[0][2] * b.m[1][0] ) + ( a.m[1][2] * b.m[1][1] ) + ( a.m[2][2] * b.m[1][2] ) + ( a.m[3][2] * b.m[1][3] );
	//result.m[2][2] = ( a.m[0][2] * b.m[2][0] ) + ( a.m[1][2] * b.m[2][1] ) + ( a.m[2][2] * b.m[2][2] ) + ( a.m[3][2] * b.m[2][3] );
	//result.m[3][2] = ( a.m[0][2] * b.m[3][0] ) + ( a.m[1][2] * b.m[3][1] ) + ( a.m[2][2] * b.m[3][2] ) + ( a.m[3][2] * b.m[3][3] );
	
	//result.m[0][3] = ( a.m[0][3] * b.m[0][0] ) + ( a.m[1][3] * b.m[0][1] ) + ( a.m[2][3] * b.m[0][2] ) + ( a.m[3][3] * b.m[0][3] );
	//result.m[1][3] = ( a.m[0][3] * b.m[1][0] ) + ( a.m[1][3] * b.m[1][1] ) + ( a.m[2][3] * b.m[1][2] ) + ( a.m[3][3] * b.m[1][3] );
	//result.m[2][3] = ( a.m[0][3] * b.m[2][0] ) + ( a.m[1][3] * b.m[2][1] ) + ( a.m[2][3] * b.m[2][2] ) + ( a.m[3][3] * b.m[2][3] );
	//result.m[3][3] = ( a.m[0][3] * b.m[3][0] ) + ( a.m[1][3] * b.m[3][1] ) + ( a.m[2][3] * b.m[3][2] ) + ( a.m[3][3] * b.m[3][3] );

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			result.m[y][x] = ( a.m[0][x] * b.m[y][0] ) + 
				( a.m[1][x] * b.m[y][1] ) + 
				( a.m[2][x] * b.m[y][2] ) + 
				( a.m[3][x] * b.m[y][3] );
		}
	}

	return result;
}


//-----------------------------------------------------------------------------
// Quaternion Functions
//-----------------------------------------------------------------------------
float Quaternion::GetLength()
{
	return sqrt( w * w + x * x + y * y + z * z );
}

Quaternion Quaternion::Conjugate()
{
	Quaternion result(0, 0, 0, 0);

	result.w = w;
	result.x = -x;
	result.y = -y;
	result.z = -z;

	return result;
}

Quaternion Quaternion::Inverse()
{
	Quaternion result(0, 0, 0, 0);

	result.w = Conjugate().w / GetLength();
	result.x = Conjugate().x / GetLength();
	result.y = Conjugate().y / GetLength();
	result.z = Conjugate().z / GetLength();

	return result;
}

Quaternion FromAxisAngle( Vector3 axis, float angle )
{
	Quaternion result(0, 0, 0, 0);

	result.w = cosf( angle / 2 );
	result.x = sinf( angle / 2 ) * axis.x;
	result.y = sinf( angle / 2 ) * axis.y;
	result.z = sinf( angle / 2 ) * axis.z;

	return result;
}

float DotProduct( Quaternion &a, Quaternion &b )
{
	return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}

Quaternion operator +( Quaternion &a, Quaternion &b )
{
	Quaternion result(0, 0, 0, 0);

	result.w = a.w + b.w;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

Quaternion operator *( Quaternion &a, Quaternion &b )
{
	Quaternion result(0, 0, 0, 0);

	result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	result.x = a.w * b.x + a.x * b.w + a.z * b.y - a.y * b.z;
	result.y = a.w * b.y + a.y * b.w + a.x * b.z - a.z * b.x;
	result.z = a.w * b.z + a.z * b.w + a.y * b.x - a.x * b.y;

	return result;
}

Quaternion operator *( float f, Quaternion &a )
{
	Quaternion result(0, 0, 0, 0);

	result.w = f * a.w;
	result.x = f * a.x;
	result.y = f * a.y;
	result.z = f * a.z;

	return result;
}

Quaternion Slerp( Quaternion &a, Quaternion &b, float alpha )
{
	float theta = acos( DotProduct( a, b ) );
	
	// Slerp optimisation:
	return 
		( sinf( ( 1 - alpha ) * theta ) / sinf( theta ) ) * a  +
		( sinf( alpha * theta ) / sinf( theta ) ) * b;
}


//-----------------------------------------------------------------------------
// Conversion Functions
//-----------------------------------------------------------------------------
Quaternion ConvertToQuaternion( Matrix4x4 &a )
{
	Quaternion result(0, 0, 0, 0);

	//

	return result;
}

Matrix4x4 ConvertToMatrix4x4( Quaternion &a )
{
	Matrix4x4 result;

	result.Identity();
	result.m[0][0] = 1 - 2 * a.y * a.y - 2 * a.z * a.z;
	result.m[1][0] = 2 * a.x * a.y + 2 * a.z * a.w;
	result.m[2][0] = 2 * a.x * a.z - 2 * a.y * a.w;
	//result.m[3][0] = 0;
	result.m[0][1] = 2 * a.x * a.y - 2 * a.z * a.w;
	result.m[1][1] = 1 - 2 * a.x * a.x - 2 * a.z * a.z;
	result.m[2][1] = 2 * a.y * a.z + 2 * a.x * a.w;
	//result.m[3][1] = 0;
	result.m[0][2] = 2 * a.x * a.z + 2 * a.y * a.w;
	result.m[1][2] = 2 * a.y * a.z - 2 * a.x * a.w;
	result.m[2][2] = 1 - 2 * a.x * a.x - 2 * a.y * a.y;
	//result.m[3][2] = 0;
	//result.m[0][3] = 0;
	//result.m[1][3] = 0;
	//result.m[2][3] = 0;
	//result.m[3][3] = 0;

	return result;
}


//-----------------------------------------------------------------------------
// Overload ostream
//-----------------------------------------------------------------------------
ostream &operator <<( ostream &o, Vector3 &v )
{
	o << v.x << "," << v.y << "," << v.z;

	return o;
}

ostream &operator <<( ostream &o, Matrix4x4 &mat )
{
	o << mat.m[0][0] << "," << mat.m[1][0] << "," << mat.m[2][0] << "," << mat.m[3][0] << endl;
	o << mat.m[0][1] << "," << mat.m[1][1] << "," << mat.m[2][1] << "," << mat.m[3][1] << endl;
	o << mat.m[0][2] << "," << mat.m[1][2] << "," << mat.m[2][2] << "," << mat.m[3][2] << endl;
	o << mat.m[0][3] << "," << mat.m[1][3] << "," << mat.m[2][3] << "," << mat.m[3][3] << endl;

	return o;
}