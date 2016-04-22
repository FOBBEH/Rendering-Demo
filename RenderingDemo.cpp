// For text
#include <DXUT.h>
#include <SDKmisc.h>

// For rendering stuff
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 )								// disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#include <iostream>
#include <sstream>
using std::cout;
using std::endl;

#include "maths.h"
#include "GameDebugLog.h"




//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
// Text:
ID3DXFont*						g_pFont = NULL;					// Font for drawing text
ID3DXSprite*					g_pTextSprite = NULL;			// Sprite for batching draw text calls

// For scene:
LPDIRECT3D9						g_pD3D = NULL;					// Used to create the D3DDevice
LPDIRECT3DDEVICE9				g_pd3dDevice = NULL;			// Our rendering device
LPDIRECT3DVERTEXBUFFER9			g_pVBCylinder = NULL;			// Buffer for cylinder vertices
LPDIRECT3DTEXTURE9				g_pTexture = NULL;				// Textures
LPDIRECT3DTEXTURE9				g_pTexture2 = NULL;
LPDIRECT3DTEXTURE9				g_pTexture3 = NULL;
LPDIRECT3DTEXTURE9				g_pTexture4 = NULL;

Matrix4x4						g_matWorld;						// Set up world matrix, can extract values for text
																// I'm really lazy
Matrix4x4						g_rotationMatrix;	

float							g_startingTime = timeGetTime();
bool							g_vertexColouring = false;		// Swap between vertex colouring and texturing
bool							g_textureWrap = false;			// Change UV coordinates depending on texture wrap
bool							g_alphaChannel = false;			// Turn on/off alpha channel
unsigned int					g_framesPerSecond = NULL;

unsigned int					g_renderSetting = NULL;
unsigned int					g_colourSetting = NULL;			
unsigned int					g_lightingSetting = NULL;
unsigned int					g_ambientSetting = 1;
unsigned int					g_emissiveSetting = 1;
unsigned int					g_materialSetting = NULL;
unsigned int					g_texturingSetting = NULL;		
unsigned int					g_texturing2Setting = NULL;
unsigned int					g_blendingSetting = NULL;		


// A structure for our custom vertex type
struct CUSTOMVERTEXCYLINDER
{
	Vector3						position;						// The 3D position for the vertex
	Vector3						normal;							// The surface normal for the vertex
	DWORD						colour;							// The vertex color
    FLOAT						tu, tv;							// The texture coordinates
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEXCYLINDER		(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// Resolution values
#define							WIDTH			1280
#define							HEIGHT			720




//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
	// Debug window:
	GameDebugLogStartUp();

	// Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn on the zbuffer
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Initialise()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------
HRESULT Initialise()
{
	// Initialise the font
    if( FAILED( D3DXCreateFont( g_pd3dDevice, 25, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                              L"Arial", &g_pFont ) ) )
	{
		return E_FAIL;
	};
	//g_pFont->OnResetDevice();

	D3DXCreateSprite( g_pd3dDevice, &g_pTextSprite );

	// Use D3DX to create a texture from a file based image
	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"pattern.jpg", &g_pTexture ) ) )
    {
        // If texture is not in current folder, try parent folder
        if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"..\\pattern.jpg", &g_pTexture ) ) )
        {
            MessageBox( NULL, L"Could not find pattern.jpg", L"RenderingDemo.exe", MB_OK );
            return E_FAIL;
        }
    }
	
	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"smiley dog.jpg", &g_pTexture2 ) ) )
    {
        // If texture is not in current folder, try parent folder
        if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"..\\smiley dog.jpg", &g_pTexture2 ) ) )
        {
            MessageBox( NULL, L"Could not find smiley dog.jpg", L"RenderingDemo.exe", MB_OK );
            return E_FAIL;
        }
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"branch.bmp", &g_pTexture3 ) ) )
    {
        // If texture is not in current folder, try parent folder
        if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"..\\branch.bmp", &g_pTexture3 ) ) )
        {
            MessageBox( NULL, L"Could not find branch.bmp", L"RenderingDemo.exe", MB_OK );
            return E_FAIL;
        }
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"doge.jpg", &g_pTexture4 ) ) )
    {
        // If texture is not in current folder, try parent folder
        if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"..\\doge.jpg", &g_pTexture4 ) ) )
        {
            MessageBox( NULL, L"Could not find doge.jpg", L"RenderingDemo.exe", MB_OK );
            return E_FAIL;
        }
    }

	// Create the cylinder vertex buffer:
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 52 * 2 * sizeof( CUSTOMVERTEXCYLINDER ),
                                                  0, D3DFVF_CUSTOMVERTEXCYLINDER,
                                                  D3DPOOL_DEFAULT, &g_pVBCylinder, NULL ) ) )
    {
        return E_FAIL;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitialiseCylinder()
// Desc: Create the cylinder and reinitialise the variables
//-----------------------------------------------------------------------------
HRESULT InitialiseCylinder()
{
	// Fill the vertex buffer. We are algorithmically generating a cylinder
    // here, including the normals, which are used for lighting.
    CUSTOMVERTEXCYLINDER* pVertices;
    if( FAILED( g_pVBCylinder->Lock( 0, 0, ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    for( DWORD i = 0; i < 52; i++ )
    {
        FLOAT theta = ( 2 * D3DX_PI * i ) / ( 52 - 1 );

        pVertices[2 * i + 0].position = Vector3( sinf( theta ), -2.0f, cosf( theta ) );
		pVertices[2 * i + 0].normal = Vector3( sinf( theta ), 0.0f, cosf( theta ) );
		if ( !g_textureWrap )
		{
			pVertices[2 * i + 0].tu = ( ( FLOAT )i ) / ( 52 - 1 );
			pVertices[2 * i + 0].tv = 1.0f;
		}
		else if ( g_textureWrap )
		{
			pVertices[2 * i + 0].tu = ( ( FLOAT )i * 2 ) / ( 52 - 1 );
			pVertices[2 * i + 0].tv = 1.0f * 2;
		}

        pVertices[2 * i + 1].position = Vector3( sinf( theta ), 2.0f, cosf( theta ) );
		pVertices[2 * i + 1].normal = Vector3( sinf( theta ), 0.0f, cosf( theta ) );
		if ( !g_textureWrap )
		{
			pVertices[2 * i + 1].tu = ( ( FLOAT )i ) / ( 52 - 1 );
			pVertices[2 * i + 1].tv = 0.0f;
		}
		else if ( g_textureWrap )
		{
			pVertices[2 * i + 1].tu = ( ( FLOAT )i * 2 ) / ( 52 - 1 );
			pVertices[2 * i + 1].tv = 0.0f;	// * 2
		}

		if ( g_vertexColouring )
		{
			// Vertex colouring:
			if (i % 3 == 0)
			{
				// Red
				pVertices[2 * i + 0].colour = 0xffff0000;
				pVertices[2 * i + 1].colour = 0xffff0000;
			}
			if (i % 3 == 1)
			{
				// Green
				pVertices[2 * i + 0].colour = 0xff00ff00;
				pVertices[2 * i + 1].colour = 0xff00ff00;
			}
			if (i % 3 == 2)
			{
				// Blue
				pVertices[2 * i + 0].colour = 0xff0000ff;
				pVertices[2 * i + 1].colour = 0xff0000ff;
			}
		}
		else if ( !g_vertexColouring )
		{
			pVertices[2 * i + 0].colour = 0xffffffff;
			pVertices[2 * i + 1].colour = 0xffffffff;
		}
    }
    g_pVBCylinder->Unlock();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pFont != NULL )
		g_pFont->Release();
	
	if( g_pVBCylinder != NULL )
        g_pVBCylinder->Release();

	if (g_pTexture != NULL )
		g_pTexture->Release();

	if (g_pTexture2 != NULL )
		g_pTexture2->Release();

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
	// Yaw: Rotate around the Y axis
	// USING MATRIX (EULER ANGLE, YAW):
	//Matrix4x4 matYaw = Yaw( timeGetTime() / 2000.0f );
	
	// USING QUATERNION:
	Vector3 rotationAxis(0, 1, 0);		// Rotate Y axis
	float rotationAngle = sinf( timeGetTime() / 2000.0f );
	Quaternion myRotation( FromAxisAngle( rotationAxis, rotationAngle ) );
	Quaternion additionalRotation( 0, 0, 1, 0 );
	Quaternion slerpTest = Slerp( myRotation, additionalRotation, rotationAngle );
	g_rotationMatrix = ConvertToMatrix4x4( slerpTest );
	
	// NOTE: I DON'T KNOW IF I'M USING QUATERNIONS RIGHT!!!

	// Position matrix:
	Matrix4x4 matMove;
	if ( g_renderSetting == 6 )
		matMove = Position( 0.0f, 0.0f, 0.5f * sinf( timeGetTime() / 500.0f) * 3.0f );
	else
		matMove = Position( 0.0f, 0.0f, 0.0f );
	
	// Multiply the quaternion matrix by the position matrix
	g_matWorld = g_rotationMatrix * matMove;

	// Set it and forget it
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_matWorld.DXMatrix() );
	
    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    Vector3 vEyePt( 0.0f, 3.0f, -7.0f );
    Vector3 vLookatPt( 0.0f, 0.0f, 0.0f );
    Vector3 vUpVec( 0.0f, 1.0f, 0.0f );
	Matrix4x4 matView = View( vEyePt, vLookatPt, vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView.DXMatrix() );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
	Matrix4x4 matProj = Perspective( D3DX_PI / 4, (float)(WIDTH) / (float)(HEIGHT), 1.0f, 100.0f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj.DXMatrix() );
}




//-----------------------------------------------------------------------------
// Name: SetupMaterials()
// Desc: Sets up the RenderingDemo and materials for the scene.
//-----------------------------------------------------------------------------
VOID SetupMaterials()
{
    // Set up a material. The material here just has the diffuse and ambient
    // colors set to yellow. Note that only one material can be used at a time.
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
	// Diffuse material property
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	// Specular material property with varying power
	mtrl.Specular.r = sinf( timeGetTime() / 100.0f ) * 0.5f;
	mtrl.Specular.g = sinf( timeGetTime() / 100.0f ) * 0.5f;
	mtrl.Specular.b = sinf( timeGetTime() / 100.0f ) * 0.5f;
	mtrl.Specular.a = sinf( timeGetTime() / 100.0f ) * 0.5f;
	mtrl.Power = sinf( timeGetTime() / 200.0f );
	// Emissive material power
	switch ( g_emissiveSetting )
	{
	case 1:
		mtrl.Emissive.r = 0.0f;
		mtrl.Emissive.g = 0.0f;
		mtrl.Emissive.b = 0.0f;
		mtrl.Emissive.a = 1.0f;
		break;
	case 2:
		mtrl.Emissive.r = 1.0f;
		mtrl.Emissive.g = 0.0f;
		mtrl.Emissive.b = 0.0f;
		mtrl.Emissive.a = 1.0f;
		break;
	case 3:
		mtrl.Emissive.r = 0.0f;
		mtrl.Emissive.g = 1.0f;
		mtrl.Emissive.b = 0.0f;
		mtrl.Emissive.a = 1.0f;
		break;
	case 4:
		mtrl.Emissive.r = 0.0f;
		mtrl.Emissive.g = 0.0f;
		mtrl.Emissive.b = 3.0f;
		mtrl.Emissive.a = 1.0f;
		break;
	case 5:
		mtrl.Emissive.r = 1.0f;
		mtrl.Emissive.g = 1.0f;
		mtrl.Emissive.b = 1.0f;
		mtrl.Emissive.a = 1.0f;
		break;
	default:
		break;
	}
    g_pd3dDevice->SetMaterial( &mtrl );
}




//-----------------------------------------------------------------------------
// Name: SetupLights()
// Desc: Sets up the lights dawg.
//-----------------------------------------------------------------------------
VOID SetupLights()
{
    // Directional light:
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
    light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.5f * sinf( timeGetTime() / 250.0f ) + 0.5f;
    light.Diffuse.g = 0.5f * sinf( timeGetTime() / 500.0f ) + 0.5f;
    light.Diffuse.b = 0.5f * sinf( timeGetTime() / 750.0f ) + 0.5f;
	light.Specular.r = 0.5f;
	light.Specular.g = 0.5f;
	light.Specular.b = 0.5f;
	light.Range = 10.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 1.0f;

	// Point light:
	D3DLIGHT9 light2;
	ZeroMemory( &light2, sizeof( D3DLIGHT9 ) );
	light2.Type = D3DLIGHT_POINT;
	light2.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	light2.Position = D3DXVECTOR3( 2.5f * sinf( timeGetTime() / 500.0f ), 0.0f, 2.5f * cosf( timeGetTime() / 500.0f ) );
	light2.Specular.r = 1.0f;
	light2.Specular.g = 1.0f;
	light2.Specular.b = 1.0f;
	light2.Range = 5.0f;
	light2.Attenuation0 = 0.0f;
	light2.Attenuation1 = 0.0f;
	light2.Attenuation2 = 1.0f;

	// Spot light:
	D3DLIGHT9 light3;
	ZeroMemory( &light3, sizeof( D3DLIGHT9 ) );
	light3.Type = D3DLIGHT_SPOT;
	light3.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	light3.Position = D3DXVECTOR3( 0.0f, 0.0f, -10.0f );
	light3.Direction = D3DXVECTOR3( 2.5f * sinf( timeGetTime() / 500.0f ), 0.0f, 10.0f );
	light3.Specular.r = 1.0f;
	light3.Specular.g = 1.0f;
	light3.Specular.b = 1.0f;
	light3.Range = 10.0f;
	light3.Attenuation0 = 0.0f;
	light3.Attenuation1 = 1.0f;
	light3.Attenuation2 = 0.0f;
	light3.Phi = D3DXToRadian(30.0f);
	light3.Theta = D3DXToRadian(20.0f);
	light3.Falloff = 1.0f;

    Vector3 vecDir( cosf( timeGetTime() / 1000.0f ), 1.0f, sinf( timeGetTime() / 1000.0f ) );
	vecDir.Normalise();
	light.Direction.x = vecDir.x;
	light.Direction.y = vecDir.y;
	light.Direction.z = vecDir.z;

	switch ( g_ambientSetting )
	{
	case 1:		// Black / No ambient colour
		light.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		light2.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		light3.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		break;
	case 2:		// Red
		light.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		//g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
		light2.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		light3.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 3:		// Green
		light.Ambient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		light2.Ambient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		light3.Ambient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 4:		// Blue
		light.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		light2.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		light3.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case 5:		// White
		light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light2.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light3.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}

	switch ( g_lightingSetting )
	{
	case 2:
		g_pd3dDevice->SetLight( 0, &light );
		break;
	case 3:
		g_pd3dDevice->SetLight( 0, &light2 );
		break;
	case 4:
		g_pd3dDevice->SetLight( 0, &light3 );
		break;
	default:
		break;
	}
    
    // Finally, turn on some ambient light.
    g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );
}




//--------------------------------------------------------------------------------------
// Name: RenderText()
// Desc: Render the help and statistics text. This function uses the ID3DXFont
//       interface for efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    float currentTime = timeGetTime();

	if ( (INT)(currentTime) % 1000 < 100 ) 
	{
		g_framesPerSecond = (INT)( 1000.0f / ( currentTime - g_startingTime ) );
		g_startingTime = currentTime;
	}
	
	std::wostringstream FPSCounter;
	FPSCounter << "Dodgy FPS Counter: " << g_framesPerSecond;
	std::wstring FPSString = FPSCounter.str();

	// The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( g_pSprite, strMsg, -1, &rc, DT_NOCLIP, g_clr );
    // If NULL is passed in as the sprite object, then it will work however the 
    // pFont->DrawText() will not be batched together.  Batching calls will improves performance.
    CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 30 );
    
	txtHelper.Begin();

	//-----------------------------------------------------------------------------
	// Title
	//-----------------------------------------------------------------------------
	txtHelper.SetInsertionPos( 550, 30 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.7f, 0.2f, 1.0f ) );
	txtHelper.DrawTextLine( L"Rendering Demo" );

	//-----------------------------------------------------------------------------
	// Instructions
	//-----------------------------------------------------------------------------
	txtHelper.SetInsertionPos( 30, 70 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 0.6f, 0.4f, 0.7f, 1.0f ) );
	txtHelper.DrawTextLine( L"Press number for greatest walrus" );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	txtHelper.DrawTextLine( L"1. Colour" );
	txtHelper.DrawTextLine( L"2. Lighting" );
	txtHelper.DrawTextLine( L"3. Texturing Pt. 1" );
	txtHelper.DrawTextLine( L"4. Texturing Pt. 2" );
	txtHelper.DrawTextLine( L"5. Blending" );
	txtHelper.DrawTextLine( L"6. Maths" );

	txtHelper.SetInsertionPos( 30, 300 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 0.6f, 0.8f, 0.7f, 1.0f ) );
	txtHelper.DrawTextLine( L"Pls no coperino pasterino" );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	switch ( g_renderSetting )
	{
	case 1:
		txtHelper.DrawTextLine( L"Q. Flat shading" );
		txtHelper.DrawTextLine( L"W. Gouraud shading" );
		break;
	case 2:
		txtHelper.DrawTextLine( L"Q. Lighting off" );
		txtHelper.DrawTextLine( L"W. Lighting on (Directional)" );
		txtHelper.DrawTextLine( L"E. Lighting on (Point)" );
		txtHelper.DrawTextLine( L"R. Lighting on (Spot)" );
		txtHelper.DrawTextLine( L"" );
		txtHelper.DrawTextLine( L"A/Z. Ambient/Emissive colour off" );
		txtHelper.DrawTextLine( L"S/X. Ambient/Emissive colour (Red)" );
		txtHelper.DrawTextLine( L"D/C. Ambient/Emissive colour (Green)" );
		txtHelper.DrawTextLine( L"F/V. Ambient/Emissive colour (Blue)" );
		txtHelper.DrawTextLine( L"G/B. Ambient/Emissive colour (White)" );
		break;
	case 3:
		txtHelper.DrawTextLine( L"Q. Texturing + lighting off" );
		txtHelper.DrawTextLine( L"W. Texturing on (Minification, Point)" );
		txtHelper.DrawTextLine( L"E. Texturing on (Magnification, Point)" );
		txtHelper.DrawTextLine( L"R. Texturing on (Mipmapping, Point)" );
		txtHelper.DrawTextLine( L"T. Texturing on (Minification, Linear)" );
		txtHelper.DrawTextLine( L"Y. Texturing on (Magnification, Linear)" );
		txtHelper.DrawTextLine( L"U. Texturing on (Mipmapping, Linear)" );
		break;
	case 4:
		txtHelper.DrawTextLine( L"Q. Texturing + lighting off" );
		txtHelper.DrawTextLine( L"W. Texturing on (Wrap)" );
		txtHelper.DrawTextLine( L"E. Texturing on (Mirror, U)" );
		txtHelper.DrawTextLine( L"R. Texturing on (Mirror, V)" );
		txtHelper.DrawTextLine( L"T. Texturing on (Clamp, U)" );
		txtHelper.DrawTextLine( L"Y. Texturing on (Clamp, V)" );
		break;
	case 5:
		txtHelper.DrawTextLine( L"Q. Texturing + lighting off" );
		txtHelper.DrawTextLine( L"W. Alpha channel off, Vertex colouring off" );
		txtHelper.DrawTextLine( L"E. Alpha channel on, Vertex colouring off" );
		txtHelper.DrawTextLine( L"R. Alpha channel off, Vertex colouring on" );
		txtHelper.DrawTextLine( L"T. Alpha channel on, Vertex colouring on" );
		break;
	case 6:
		txtHelper.DrawTextLine( L"Nerdy stuff" );
		break;
	case 0:
		txtHelper.DrawTextLine( L"so doge" );
		txtHelper.DrawTextLine( L"such wow" );
		break;
	default:
		txtHelper.DrawTextLine( L"Buddy pls" );
		break;
	}

	//-----------------------------------------------------------------------------
	// Current render
	//-----------------------------------------------------------------------------
	txtHelper.SetInsertionPos( 850, 70 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 0.5f, 0.7f, 1.0f, 1.0f ) );
	txtHelper.DrawTextLine( L"Currently rendering" );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	switch ( g_renderSetting )
	{
	case 1:
		txtHelper.DrawTextLine( L"COLOUR DEMO" );
		switch ( g_colourSetting )
		{
		case 1:
			txtHelper.DrawTextLine( L"Shading: Flat" );
			break;
		case 2:
			txtHelper.DrawTextLine( L"Shading: Gouraud" );
			break;
		}

		break;
	case 2:
		txtHelper.DrawTextLine( L"LIGHTING DEMO" );
		switch ( g_lightingSetting )
		{
		case 1:
			txtHelper.DrawTextLine( L"Lighting: Off" );
			break;
		case 2:
			txtHelper.DrawTextLine( L"Lighting: Directional" );
			txtHelper.DrawTextLine( L"" );
			txtHelper.DrawTextLine( L"The directional light is" );
			txtHelper.DrawTextLine( L"rotating counter-clockwise" );
			txtHelper.DrawTextLine( L"around the cylinder." );
			txtHelper.DrawTextLine( L"" );
			break;
		case 3:
			txtHelper.DrawTextLine( L"Lighting: Point" );
			txtHelper.DrawTextLine( L"" );
			txtHelper.DrawTextLine( L"The point light is rotating" );
			txtHelper.DrawTextLine( L"clockwise around the cylinder." );
			txtHelper.DrawTextLine( L"" );
			break;
		case 4:
			txtHelper.DrawTextLine( L"Lighting: Spot" );
			txtHelper.DrawTextLine( L"" );
			txtHelper.DrawTextLine( L"The spot light is shining" );
			txtHelper.DrawTextLine( L"left to right from the front" );
			txtHelper.DrawTextLine( L"of the cylinder." );
			txtHelper.DrawTextLine( L"" );
			txtHelper.DrawTextLine( L"Shittiest looking spot light!" );
			txtHelper.DrawTextLine( L"" );
			break;
		}

		switch ( g_ambientSetting )
		{
		case 1:
			txtHelper.DrawTextLine( L"Ambient lighting: Off" );
			break;
		case 2:
			txtHelper.DrawTextLine( L"Ambient lighting: Red" );
			break;
		case 3:
			txtHelper.DrawTextLine( L"Ambient lighting: Green" );
			break;
		case 4:
			txtHelper.DrawTextLine( L"Ambient lighting: Blue" );
			break;
		case 5:
			txtHelper.DrawTextLine( L"Ambient lighting: White" );
			break;
		}

		switch ( g_emissiveSetting )
		{
		case 1:
			txtHelper.DrawTextLine( L"Emissive material property: Off" );
			break;
		case 2:
			txtHelper.DrawTextLine( L"Emissive material property: Red" );
			break;
		case 3:
			txtHelper.DrawTextLine( L"Emissive material property: Green" );
			break;
		case 4:
			txtHelper.DrawTextLine( L"Emissive material property: Blue" );
			break;
		case 5:
			txtHelper.DrawTextLine( L"Emissive material property: White" );
			break;
		}

		break;
	case 3:
		txtHelper.DrawTextLine( L"TEXTURING DEMO PT. 1" );
		switch ( g_texturingSetting )
		{
		case 1:
			txtHelper.DrawTextLine( L"Texturing and lighting: Off" );
			break;
		case 2:
			txtHelper.DrawTextLine( L"Texturing: Minification, Point" );
			break;
		case 3:
			txtHelper.DrawTextLine( L"Texturing: Magnification, Point" );
			break;
		case 4:
			txtHelper.DrawTextLine( L"Texturing: Mipmapping, Point" );
			break;
		case 5:
			txtHelper.DrawTextLine( L"Texturing: Minification, Linear" );
			break;
		case 6:
			txtHelper.DrawTextLine( L"Texturing: Magnification, Linear" );
			break;
		case 7:
			txtHelper.DrawTextLine( L"Texturing: Mipmapping, Linear" );
			break;
		}

		break;
	case 4:
		txtHelper.DrawTextLine( L"TEXTURING DEMO PT. 2" );
		switch ( g_texturing2Setting )
		{
		case 1:
			txtHelper.DrawTextLine( L"Texturing and lighting: Off" );
			break;
		case 2:
			txtHelper.DrawTextLine( L"Texturing: Wrap" );
			break;
		case 3:
			txtHelper.DrawTextLine( L"Texturing: Mirror, U" );
			break;
		case 4:
			txtHelper.DrawTextLine( L"Texturing: Mirror, V" );
			break;
		case 5:
			txtHelper.DrawTextLine( L"Texturing: Clamp, U" );
			break;
		case 6:
			txtHelper.DrawTextLine( L"Texturing: Clamp, V" );
			break;
		}
		break;
	case 5:
		txtHelper.DrawTextLine( L"BLENDING DEMO" );
		switch ( g_blendingSetting )
		{
		case 1:
			txtHelper.DrawTextLine( L"Texturing and lighting: Off" );
			break;
		case 2:
			txtHelper.DrawTextLine( L"Alpha channel: Off" );
			txtHelper.DrawTextLine( L"Vertex colouring: Off" );
			break;
		case 3:
			txtHelper.DrawTextLine( L"Alpha channel: On" );
			txtHelper.DrawTextLine( L"Vertex colouring: Off" );
			break;
		case 4:
			txtHelper.DrawTextLine( L"Alpha channel: Off" );
			txtHelper.DrawTextLine( L"Vertex colouring: On" );
			txtHelper.DrawTextLine( L"" );
			txtHelper.DrawTextLine( L"Some weird blend effect" );
			txtHelper.DrawTextLine( L"(Try it with lighting!)" );
			break;
		case 5:
			txtHelper.DrawTextLine( L"Alpha channel: On" );
			txtHelper.DrawTextLine( L"Vertex colouring: On" );
			txtHelper.DrawTextLine( L"" );
			txtHelper.DrawTextLine( L"Some weird blend effect" );
			txtHelper.DrawTextLine( L"(Try it with lighting!)" );
			break;
		}
		break;
	case 6:
		txtHelper.DrawTextLine( L"MATHS DEMO" );
		txtHelper.DrawTextLine( L"Check the debug window :)" );

		if ((INT)(currentTime) % 10000 < 20)
		{
			float determinant = Determinant( g_matWorld );
			cout << "Determinant: " << determinant << endl << endl;

			Matrix4x4 inverseMatrix = Inverse( g_matWorld );
			cout << "Inverse Matrix: " << endl;
			cout << inverseMatrix.m[0][0] << " " << inverseMatrix.m[1][0] << " " << inverseMatrix.m[2][0] << " " << inverseMatrix.m[3][0] << endl;
			cout << inverseMatrix.m[0][1] << " " << inverseMatrix.m[1][1] << " " << inverseMatrix.m[2][1] << " " << inverseMatrix.m[3][1] << endl;
			cout << inverseMatrix.m[0][2] << " " << inverseMatrix.m[1][2] << " " << inverseMatrix.m[2][2] << " " << inverseMatrix.m[3][2] << endl;
			cout << inverseMatrix.m[0][3] << " " << inverseMatrix.m[1][3] << " " << inverseMatrix.m[2][3] << " " << inverseMatrix.m[3][3] << endl << endl;

			float ePitch = asinf( g_matWorld.m[2][1]) * 180;	// in degrees
			cout << "Euler Angle, Pitch: " << ePitch << endl;

			float eYaw = asinf( g_matWorld.m[0][2] ) * 180;
			cout << "Euler Angle, Yaw: " << eYaw << endl;

			float eRoll = asinf( g_matWorld.m[0][1] ) * 180;
			cout << "Euler Angle, Roll: " << eRoll << endl << endl;

			cout << "Rotation Matrix: " << endl;
			cout << g_rotationMatrix.m[0][0] << " " << g_rotationMatrix.m[1][0] << " " << g_rotationMatrix.m[2][0] << " " << g_rotationMatrix.m[3][0] << endl;
			cout << g_rotationMatrix.m[0][1] << " " << g_rotationMatrix.m[1][1] << " " << g_rotationMatrix.m[2][1] << " " << g_rotationMatrix.m[3][1] << endl;
			cout << g_rotationMatrix.m[0][2] << " " << g_rotationMatrix.m[1][2] << " " << g_rotationMatrix.m[2][2] << " " << g_rotationMatrix.m[3][2] << endl;
			cout << g_rotationMatrix.m[0][3] << " " << g_rotationMatrix.m[1][3] << " " << g_rotationMatrix.m[2][3] << " " << g_rotationMatrix.m[3][3] << endl << endl;
		}

		break;

	default:
		txtHelper.DrawTextLine( L"NOTHING! Press a button you fool!" );
		txtHelper.DrawTextLine( L"" );
		txtHelper.DrawTextLine( L"NOTE. You can transfer some of the" );
		txtHelper.DrawTextLine( L"properties to the other demos." ); 
		txtHelper.DrawTextLine( L"ie. Lighting, Shading, etc." );
		break;
	}

	//-----------------------------------------------------------------------------
	// Dodgy walrus counter
	//-----------------------------------------------------------------------------
	txtHelper.SetInsertionPos( 515, 620 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	txtHelper.DrawTextLine( FPSString.c_str() );

    txtHelper.End();
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
    // Clear the backbuffer and the zbuffer
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	// | D3DCLEAR_STENCIL
                         D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Setup the materials and lights
        SetupMaterials();
		SetupLights();

        // Setup the world, view, and projection matrices
        SetupMatrices();

        // Render the vertex buffer contents
        g_pd3dDevice->SetStreamSource( 0, g_pVBCylinder, 0, sizeof( CUSTOMVERTEXCYLINDER ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEXCYLINDER );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 * 52 - 2 );

		// FUCK STENCILS ..|..
		// Enable stencil testing
		//g_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );
		//
		// Specify the stencil comparison function
		//g_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_ALWAYS );
		//g_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_INCR );
		//g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		//g_pd3dDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, TRUE );

		// Set the comparison reference value
		//g_pd3dDevice->SetRenderState( D3DRS_STENCILREF, 0x1 );

		// Specify a stencil mask 
		//g_pd3dDevice->SetRenderState( D3DRS_STENCILMASK, 0xff00ffff );
		//g_pd3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0x0000ffff );
		//g_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP ); 

		// Render all the text
		RenderText();

        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
		case WM_KEYDOWN:
			// Set the render setting FIRST
			//-----------------------------------------------------------------------------
			// GREATEST WALRUS
			//-----------------------------------------------------------------------------
			if (wParam == '1')
			{
				// Colouring:
				g_renderSetting = 1;
				g_colourSetting = 1;
				g_vertexColouring = true;

				InitialiseCylinder();
				g_pd3dDevice->SetTexture( 0, NULL );
				g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
				g_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );
			}
			if (wParam == '2')
			{
				// Lighting:
				g_renderSetting = 2;
				g_lightingSetting = 2;
				g_vertexColouring = false;

				InitialiseCylinder();
				g_pd3dDevice->SetTexture( 0, NULL );
				g_pd3dDevice->LightEnable( 0, TRUE );
				g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
			}
			if (wParam == '3')
			{
				// Texturing Pt. 1:
				g_renderSetting = 3;
				g_texturingSetting = 2;
				g_vertexColouring = false;
				g_textureWrap = false;
				g_alphaChannel = false;

				InitialiseCylinder();
				g_pd3dDevice->SetTexture( 0, g_pTexture );
				g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
				g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
				g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
			}
			if (wParam == '4')
			{
				// Texturing Pt. 2:
				g_renderSetting = 4;
				g_texturing2Setting = 2;
				g_vertexColouring = false;
				g_textureWrap = true;

				InitialiseCylinder();
				g_pd3dDevice->SetTexture( 0, g_pTexture2 );
				g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
				g_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
				g_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
			}
			if (wParam == '5')
			{
				// Blending:
				g_renderSetting = 5;
				g_blendingSetting = 2;
				g_vertexColouring = false;
				g_textureWrap = false;
				g_alphaChannel = false;

				InitialiseCylinder();
				g_pd3dDevice->SetTexture( 0, g_pTexture3 );
				g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
				g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
			}
			if (wParam == '6')
			{
				// Maths:
				g_renderSetting = 6;
			}
			if (wParam == '0')
			{
				// BONUS?!:
				g_renderSetting = 0;
				g_textureWrap = true;

				InitialiseCylinder();
				g_pd3dDevice->SetTexture( 0, g_pTexture4 );
				g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
			}

			// Input based on which demo is currently being rendered:
			switch ( g_renderSetting )
			{
			//-----------------------------------------------------------------------------
			// COLOUR DEMO INPUT
			//-----------------------------------------------------------------------------
			case 1:
				// Set the colour setting
				g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
				if (wParam == 'Q')
				{
					// Flat shading:
					g_colourSetting = 1;
					g_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );
				}
				if (wParam == 'W')
				{
					// Gouraud shading:
					g_colourSetting = 2;
					g_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
				}
				break;

			//-----------------------------------------------------------------------------
			// LIGHTING DEMO INPUT
			//-----------------------------------------------------------------------------
			case 2:
				// Set the lighting setting:
				if (wParam == 'Q')
				{
					// Lighting off:
					g_lightingSetting = 1;
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
				}
				if (wParam == 'W')
				{
					// Directional lighting:
					g_lightingSetting = 2;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}
				if (wParam == 'E')
				{
					// Point lighting:
					g_lightingSetting = 3;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}
				if (wParam == 'R')
				{
					// Spot lighting:
					g_lightingSetting = 4;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}

				if (wParam == 'A')
				{
					// Ambient off:
					g_ambientSetting = 1;
					SetupLights();
				}
				if (wParam == 'S')
				{
					// Ambient red:
					g_ambientSetting = 2;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}
				if (wParam == 'D')
				{
					// Ambient green:
					g_ambientSetting = 3;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}
				if (wParam == 'F')
				{
					// Ambient blue:
					g_ambientSetting = 4;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}
				if (wParam == 'G')
				{
					// Ambient white:
					g_ambientSetting = 5;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}

				if (wParam == 'Z')
				{
					// Emissive off:
					g_emissiveSetting = 1;
					SetupLights();
				}
				if (wParam == 'X')
				{
					// Emissive red:
					g_emissiveSetting = 2;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}
				if (wParam == 'C')
				{
					// Emissive green:
					g_emissiveSetting = 3;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}
				if (wParam == 'V')
				{
					// Emissive blue:
					g_emissiveSetting = 4;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}
				if (wParam == 'B')
				{
					// Emissive white:
					g_emissiveSetting = 5;
					SetupLights();
					g_pd3dDevice->LightEnable( 0, TRUE );
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
				}

				break;

			//-----------------------------------------------------------------------------
			// TEXTURING PT.1 DEMO INPUT
			//-----------------------------------------------------------------------------
			case 3:
				// Set the texturing setting:
				if (wParam == 'Q')
				{
					// Texturing and lighting off:
					g_texturingSetting = 1;
					g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
					g_pd3dDevice->SetTexture( 0, NULL );
				}
				if (wParam == 'W')
				{
					// Minification, Point:
					g_texturingSetting = 2;
					g_pd3dDevice->SetTexture( 0, g_pTexture );
					g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
				}
				if (wParam == 'E')
				{
					// Magnification, Point:
					g_texturingSetting = 3;
					g_pd3dDevice->SetTexture( 0, g_pTexture );
					g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
				}
				if (wParam == 'R')
				{
					// Mipmapping, Point:
					g_texturingSetting = 4;
					g_pd3dDevice->SetTexture( 0, g_pTexture );
					g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
				}
				if (wParam == 'T')
				{
					// Minification, Linear:
					g_texturingSetting = 5;
					g_pd3dDevice->SetTexture( 0, g_pTexture );
					g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
				}
				if (wParam == 'Y')
				{
					// Magnification, Linear:
					g_texturingSetting = 6;
					g_pd3dDevice->SetTexture( 0, g_pTexture );
					g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
				}
				if (wParam == 'U')
				{
					// Mipmapping, Linear:
					g_texturingSetting = 7;
					g_pd3dDevice->SetTexture( 0, g_pTexture );
					g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
				}
				break;

			//-----------------------------------------------------------------------------
			// TEXTURING PT.2 DEMO INPUT
			//-----------------------------------------------------------------------------
			case 4:
				// Set the texturing 2 setting:
				{
					if (wParam == 'Q')
					{
						// Texturing and lighting off:
						g_texturing2Setting = 1;
						g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
						g_pd3dDevice->SetTexture( 0, NULL );
					}
					if (wParam == 'W')
					{
						// Wrap:
						g_texturing2Setting = 2;
						g_pd3dDevice->SetTexture( 0, g_pTexture2 );
						g_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
						g_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
					}
					if (wParam == 'E')
					{
						// Mirror, U:
						g_texturing2Setting = 3;
						g_pd3dDevice->SetTexture( 0, g_pTexture2 );
						g_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR );
					}
					if (wParam == 'R')
					{
						// Mirror, V:
						g_texturing2Setting = 4;
						g_pd3dDevice->SetTexture( 0, g_pTexture2 );
						g_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR );
					}
					if (wParam == 'T')
					{
						// Clamp, U:
						g_texturing2Setting = 5;
						g_pd3dDevice->SetTexture( 0, g_pTexture2 );
						g_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
					}
					if (wParam == 'Y')
					{
						// Clamp, V:
						g_texturing2Setting = 6;
						g_pd3dDevice->SetTexture( 0, g_pTexture2 );
						g_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
					}
				}
				break;

			//-----------------------------------------------------------------------------
			// BLENDING DEMO INPUT
			//-----------------------------------------------------------------------------
			case 5:
				// Set the blending setting:
				{
					if (wParam == 'Q')
					{
						// Texturing and lighting off:
						g_blendingSetting = 1;
						g_pd3dDevice->SetTexture( 0, NULL );
						g_pd3dDevice->SetTexture( 1, NULL );
						g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
					}
					if (wParam == 'W')
					{
						// Alpha channel off:
						g_blendingSetting = 2;
						g_vertexColouring = false;
						InitialiseCylinder();
						g_pd3dDevice->SetTexture( 0, g_pTexture3 );
						g_pd3dDevice->SetTexture( 1, NULL );
						g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
					}
					if (wParam == 'E')
					{
						// Alpha channel on:
						g_blendingSetting = 3;
						g_vertexColouring = false;
						InitialiseCylinder();
						g_pd3dDevice->SetTexture( 0, g_pTexture3 );
						g_pd3dDevice->SetTexture( 1, NULL );
						g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
						g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, 1 );
						g_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
						g_pd3dDevice->SetRenderState( D3DRS_ALPHAREF, (DWORD)50 );
					}
					if (wParam == 'R')
					{
						g_blendingSetting = 4;
						g_vertexColouring = true;
						InitialiseCylinder();
						g_pd3dDevice->SetTexture( 0, g_pTexture3 );
						g_pd3dDevice->SetTexture( 1, g_pTexture );
						g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
						g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD );
						g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTOP_MULTIPLYADD );
						g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTOP_MULTIPLYADD );
						g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
					}
					if (wParam == 'T')
					{
						g_blendingSetting = 5;
						g_vertexColouring = true;
						InitialiseCylinder();
						g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, D3DBLEND_ONE );
						g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
						g_pd3dDevice->SetTexture( 0, g_pTexture3 );
						g_pd3dDevice->SetTexture( 1, g_pTexture );
						g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
						g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD );
						g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTOP_MULTIPLYADD );
						g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTOP_MULTIPLYADD );
						g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
					}
				}
				break;
			}
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        L"Rendering Demo", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"Rendering Demo", L"Greatest Walrus Rendering Demo",
                              WS_OVERLAPPEDWINDOW, 100, 100, WIDTH, HEIGHT,
                              NULL, NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
        // Create the geometry
        if( SUCCEEDED( Initialise() ) )
        {
            // Show the window
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            // Enter the message loop
            MSG msg;
            ZeroMemory( &msg, sizeof( msg ) );
            while( msg.message != WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
				{
                    Render();
				}
            }
        }
    }

    UnregisterClass( L"Rendering Demo", wc.hInstance );
    return 0;
}



