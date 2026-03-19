#pragma once

// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004

// Font rendering flags
#define D3DFONT_CENTERED_X  0x0001
#define D3DFONT_CENTERED_Y  0x0002
#define D3DFONT_TWOSIDED    0x0004
#define D3DFONT_FILTERED    0x0008
#define D3DFONT_BORDER		0x0010
#define D3DFONT_COLORTABLE	0x0020

#define D3DFONT_CLEARTARGET 0x0080

#define D3DX_PI 3.1415926535897932384626f

//
// Defined deprecated D3Dx9.h struct because the old DX9 code uses these structs nowadays deprecated d3dx9 SDK data types. 
// Better option would be to modify the DX9 code ot use the newer DX SDK API, but let's do it this way for now because it does the job good enough.
//

typedef struct D3DXVECTOR4 {
#pragma pack(push,1)
	float x;
	float y;
	float z;
	float w;
#pragma pack(pop)

	D3DXVECTOR4() { x = y = z = w = 0; }

	D3DXVECTOR4(float initX, float initY, float initZ, float initW)
	{
		x = initX; y = initY; z = initZ; w = initW;
	}
} D3DXVECTOR4, * LPD3DXVECTOR4;

// Defined deprecated D3Dx9.h struct
typedef struct D3DXVECTOR3 {
#pragma pack(push,1)
	float x;
	float y;
	float z;
#pragma pack(pop)

	//D3DXVECTOR3() { x = y = z = 0; }

} D3DXVECTOR3, * LPD3DXVECTOR3;

// Defined deprecated D3Dx9math.h struct
typedef struct D3DXQUATERNION {
#pragma pack(push,1)
	float  x;
	float  y;
	float  z;
	float  w;
#pragma pack(pop)
} D3DXQUATERNION, * LPD3DXQUATERNION;