// Bmp.h : 
//
/// APIS: -
/// DEPENDENCIES: -, (Memory)
/// DEPENDENTS: Containers, Image

#pragma once

#include "../../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define BMP_MAXPALETTEENTRIES			256u

#pragma endregion
/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

// BMP_PaletteEntry flags are only ever set to 0, this is merely here for reference.
#if 0
// palette entry flags (DDRAW
flags_scoped(DirectDraw_PaletteFlags) : uint8
{
	DIRECTDRAW_PALETTE_FLAG_RESERVED   = 0x1,	/* renderer may use this entry freely */
	DIRECTDRAW_PALETTE_FLAG_EXPLICIT   = 0x2,	/* fixed but may be used by renderer */
	DIRECTDRAW_PALETTE_FLAG_NOCOLLAPSE = 0x3,	/* may not be used by renderer */
};
flags_scoped_end(DirectDraw_PaletteFlags, 0x1);


// D3DRMPALETTEFLAGS
flags_scoped(D3DRM_PaletteFlags) : uint8
{
	D3DRM_PALETTE_FLAG_FREE,			/* renderer may use this entry freely */
	D3DRM_PALETTE_FLAG_READONLY,		/* fixed but may be used by renderer */
	D3DRM_PALETTE_FLAG_RESERVED,		/* may not be used by renderer */
};
flags_scoped_end(D3DRM_PaletteFlags, 0x1);
#endif

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

#pragma pack(push, 1)

struct BMP_InfoHeader
{
	/*00,4*/ uint32	img_header_size;
	/*04,4*/ uint32	width;
	/*08,4*/ uint32	height;
	/*0c,2*/ uint16	planes;				// Always 1
	/*0e,2*/ uint16	bits_per_pixel;
	/*10,4*/ uint32	compression;
	/*14,4*/ uint32	comp_img_size;
	/*18,4*/ uint32	horz_res;			// pels/m
	/*1c,4*/ uint32	vert_res;			// pels/m
	/*20,4*/ uint32	colours;
	/*24,4*/ uint32	important_colours;
	/*28*/
};
assert_sizeof(BMP_InfoHeader, 0x28);

struct BMP_FileHeader
{
	/*00,2*/ char	bmp_str[2];
	/*02,4*/ uint32	file_size;
	/*06,2*/ uint16	reserved1;
	/*08,2*/ uint16	reserved2;
	/*0a,4*/ uint32	img_offset;
	/*0e*/
	#if false
	/*0e,28*/ BMP_InfoHeader info;
	/*0e,4*/ uint32	img_header_size;
	/*12,4*/ uint32	width;
	/*16,4*/ uint32	height;
	/*1a,2*/ uint16	planes;				// Always 1
	/*1c,2*/ uint16	bits_per_pixel;
	/*1e,4*/ uint32	compression;
	/*22,4*/ uint32	comp_img_size;
	/*26,4*/ uint32	horz_res;			// pels/m
	/*2a,4*/ uint32	vert_res;			// pels/m
	/*2e,4*/ uint32	colours;
	/*32,4*/ uint32	important_colours;
	#endif
	/*36*/
};
assert_sizeof(BMP_FileHeader, 0x0e);
//assert_sizeof(BMP_FileHeader, 0x36);

struct BMP_Header
{
	/*00,0e*/ BMP_FileHeader file;
	/*0e,28*/ BMP_InfoHeader info;
	/*36*/
};
assert_sizeof(BMP_Header, 0x36);


// PALETTEENTRY, D3DRMPALETTEENTRY
struct BMP_PaletteEntry
{
	/*0,1*/ uint8 r;		// 0 .. 255
	/*1,1*/ uint8 g;	    // 0 .. 255
	/*2,1*/ uint8 b;		// 0 .. 255
	/*3,1*/ uint8 flags;	// Only ever set to 0 in Gods98, no need to define these.
							// NOTE: flags may differ between those used by D3DRM and DDRAW.
	/*4*/
};
assert_sizeof(BMP_PaletteEntry, 0x4);

#pragma pack(pop)


// D3DRMIMAGE
struct BMP_Image
{
	/*00,4*/ uint32 width;		/* width and height in pixels */
	/*04,4*/ uint32 height;
	/*08,4*/ uint32 aspectx;	/* aspect ratio for non-square pixels */
	/*0c,4*/ uint32 aspecty;
	/*10,4*/ uint32 depth;		/* bits per pixel */
	/*14,4*/ bool32 rgb;		/* if false, pixels are indices into a
									palette otherwise, pixels encode
									RGB values. */
	/*18,4*/ uint32 bytes_per_line;		/* number of bytes of memory for a
										   scanline. This must be a multiple
										   of 4. */
	/*1c,4*/ void* buffer1;		/* memory to render into (first buffer). */
	/*20,4*/ void* buffer2;		/* second rendering buffer for double
								   buffering, set to NULL for single
								   buffering. */
	/*24,4*/ uint32 red_mask;
	/*28,4*/ uint32 green_mask;
	/*2c,4*/ uint32 blue_mask;
	/*30,4*/ uint32 alpha_mask;	/* if rgb is true, these are masks for
								   the red, green and blue parts of a
								   pixel.  Otherwise, these are masks
								   for the significant bits of the
								   red, green and blue elements in the
								   palette.  For instance, most SVGA
								   displays use 64 intensities of red,
								   green and blue, so the masks should
								   all be set to 0xfc. */
	/*34,4*/ uint32 palette_size;       /* number of entries in palette */
	/*38,4*/ BMP_PaletteEntry* palette;	/* description of the palette (only if
										   rgb is false).  Must be (1<<depth)
										   elements. */
	/*3c*/
};
assert_sizeof(BMP_Image, 0x3c);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00489ef0>
void __cdecl BMP_Parse(const void* data, uint32 fileSize, OUT BMP_Image* image);

/// CUSTOM: Parsing for just the BITMAPINFOHEADER struct without the file header.
void BMP_ParseInfoHeader(const void* data, OUT BMP_Image* image);

/// CUSTOM: Returns a pointer to the allocated buffer, which must be freed by the caller.
void* BMP_Create(OUT BMP_Image* image, uint32 width, uint32 height, uint32 bpp, OPTIONAL OUT uint32* bufferSize, bool use15Bit = true);

/// CUSTOM: Setup channel masks for different bit depths.
void BMP_SetupChannelMasks(IN OUT BMP_Image* image, bool use15Bit = true);

/// CUSTOM: Assigns all fields of a bitmap header for writing to file.
void BMP_SetupHeader(OUT BMP_Header* header, uint32 width, uint32 height, uint32 bpp, uint32 bufferSize);

// <LegoRR.exe @0048a030>
void __cdecl BMP_Cleanup(BMP_Image* image);

#pragma endregion

}
