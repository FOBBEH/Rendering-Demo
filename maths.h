// [maths.h]
#ifndef MATHS_H
#define MATHS_H

#include <d3dx9.h>

//-----------------------------------------------------------------------------
// Vector [3D]
//-----------------------------------------------------------------------------
struct Vector3
{
	Vector3( float _x, float _y, float _z )
		: x(_x), y(_y), z(_z) {}				// x = _x, y = _y, z = _z, SO FAST!
	
	float			x, y, z;

	float			GetLength();
	void			Normalise();
	D3DXVECTOR3		DXVector3();
};

//-----------------------------------------------------------------------------
// Vector Functions
//-----------------------------------------------------------------------------
Vector3				operator +( const Vector3 &a, const Vector3 &b );
Vector3				operator -( const Vector3 &a, const Vector3 &b );
float				DotProduct( const Vector3 &a, const Vector3 &b );
Vector3				CrossProduct( const Vector3 &a, const Vector3 &b );
float				GetAngle( const Vector3 &a, const Vector3 &b );


//-----------------------------------------------------------------------------
// Matrix4x4 [4x4]
//-----------------------------------------------------------------------------
struct Matrix4x4
{
	float			m[4][4];

	void			Identity();
	D3DXMATRIXA16	DXMatrix();
};

//-----------------------------------------------------------------------------
// Matrix4x4 Functions [4x4]
//-----------------------------------------------------------------------------
Matrix4x4			Multiply( Matrix4x4 &a, Matrix4x4 &b );
Matrix4x4			Transpose( Matrix4x4 &a );
Matrix4x4			Scale( Matrix4x4 &a, float scaleValue );
float				Sarrus( Matrix4x4 &a, int x, int y );
float				Determinant( Matrix4x4 &a );
float				Minor( Matrix4x4 &a, int x, int y );
Matrix4x4			Cofactor( Matrix4x4 &a );
Matrix4x4			Inverse( Matrix4x4 &a );

Matrix4x4			Pitch( float pitch );
Matrix4x4			Yaw( float yaw );
Matrix4x4			Roll( float roll );
Matrix4x4			Position( float x, float y, float z );
Matrix4x4			View( Vector3 viewerPosition, Vector3 viewerDirection, Vector3 upVector );
Matrix4x4			Perspective( float fov, float aspectRatio, float fNear, float fFar );
Matrix4x4			operator *( Matrix4x4 &a, Matrix4x4 &b );


//-----------------------------------------------------------------------------
// Quaternion
//-----------------------------------------------------------------------------
struct Quaternion
{
	Quaternion( float _w, float _x, float _y, float _z)
		: w(_w), x(_x), y(_y), z(_z) {}
	
	float			w, x, y, z;

	float			GetLength();
	Quaternion		Conjugate();
	Quaternion		Inverse();
};

//-----------------------------------------------------------------------------
// Quaternion Functions
//-----------------------------------------------------------------------------
Quaternion			FromAxisAngle( Vector3 axis, float angle );
float				DotProduct( Quaternion &a, Quaternion &b );
Quaternion			operator +( Quaternion &a, Quaternion &b );
Quaternion			operator *( Quaternion &a, Quaternion &b );
Quaternion			operator *( float f, Quaternion &a );
Quaternion			Slerp( Quaternion &a, Quaternion &b, float alpha );


//-----------------------------------------------------------------------------
// Conversion Functions
//-----------------------------------------------------------------------------
// Convert from Euler into Quaternion
Quaternion ConvertToQuaternion( Matrix4x4 &a );
Matrix4x4 ConvertToMatrix4x4( Quaternion &a );


//-----------------------------------------------------------------------------
// Overload ostream
//-----------------------------------------------------------------------------
//ostream &operator <<( ostream &o, Vector3 &v );
//ostream &operator <<( ostream &o, Matrix4x4 &mat );

#endif // MATHS_H