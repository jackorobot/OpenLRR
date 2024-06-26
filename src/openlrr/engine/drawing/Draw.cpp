// Draw.cpp : 
//

#include "../../platform/ddraw.h"
#include "../../platform/d3drm.h"			// For Viewports.h

#include "../core/Errors.h"
#include "../core/Maths.h"
#include "../gfx/Containers.h"
#include "../gfx/Mesh.h"
#include "../gfx/Viewports.h"
#include "../Graphics.h"
#include "../Main.h"
#include "DirectDraw.h"
#include "Images.h"

#include "Draw.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005417e8>
Gods98::Draw_Globs & Gods98::drawGlobs = *(Gods98::Draw_Globs*)0x005417e8; // = { nullptr };

/// CUSTOM: Basic handling for alpha (but not support for drawing with alpha other than 255).
static uint32 _drawAlphaBits  = 0;
static uint32 _drawAlphaMask  = 0;
static uint32 _drawRedShift   = 0;
static uint32 _drawGreenShift = 0;
static uint32 _drawBlueShift  = 0;
static uint32 _drawAlphaShift = 0;

/// CUSTOM: Draw_Begin() has been called, normal Draw operations will not unlock the drawing surface until Draw_End().
static bool _drawBegin = false;
/// CUSTOM: The current drawing effect while using Draw_Begin().
static Gods98::DrawEffect _drawEffect = Gods98::DrawEffect::None;

/// CUSTOM: If false, then the Draw module will not lock or draw to surfaces at all.
static bool _renderEnabled = true;
/// CUSTOM: True if the drawing surface is supposed to be locked, or Draw_Begin() has been called.
static bool _drawLocked = false;

/// CUSTOM: Width of the locked drawing surface.
static uint32 _drawWidth  = 0;
/// CUSTOM: Height of the locked drawing surface.
static uint32 _drawHeight = 0;

/// CUSTOM: The absolute scale that pixels are drawn at, defaults to Main_RenderScale().
static uint32 _drawScale  = 1;

/// CUSTOM: User-supplied image as a render target to reduce locking larger surfaces, and allow drawing on other surfaces.
static Gods98::Image* _drawRenderTarget = nullptr;

/// CUSTOM: Extra translation applied to drawing coordinates.
static Point2F _drawTranslation = { 0.0f, 0.0f };

/// CUSTOM: Backup for Draw_SetClipWindow used on the back surface, since the clip window depends on the render target.
static Point2F _drawBackClipStart = { 0.0f, 0.0f };
static Point2F _drawBackClipEnd   = { 0.0f, 0.0f };

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Draw_Pixel8Address(x, y)	static_cast<uint8*> (DirectDraw_PixelAddress(drawGlobs.buffer, drawGlobs.pitch,  8, x, y))
#define Draw_Pixel16Address(x, y)	static_cast<uint16*>(DirectDraw_PixelAddress(drawGlobs.buffer, drawGlobs.pitch, 16, x, y))
#define Draw_Pixel24Address(x, y)	static_cast<uint8*> (DirectDraw_PixelAddress(drawGlobs.buffer, drawGlobs.pitch, 24, x, y))
#define Draw_Pixel32Address(x, y)	static_cast<uint32*>(DirectDraw_PixelAddress(drawGlobs.buffer, drawGlobs.pitch, 32, x, y))

/// CUSTOM: Unlocks the surface during normal Draw calls while not using Draw_Begin().
#define Draw_TryUnlockSurface(surf)		if (!_drawBegin) Draw_UnlockSurface(surf)

#define Draw_TryDrawPixel(x, y, value)	_Draw_DrawPixel(x, y, value)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

/// CUSTOM: Gets if the Draw module rendering is enabled.
bool Gods98::Draw_IsRenderEnabled()
{
	return _renderEnabled;
}

/// CUSTOM: Sets if the Draw module rendering is enabled. For testing performance.
void Gods98::Draw_SetRenderEnabled(bool enabled)
{
	if (_renderEnabled != enabled) {
		if (!enabled && _drawBegin) {
			Draw_End();
		}
		_renderEnabled = enabled;
	}
}


/// CUSTOM: Returns true if the drawing surface is currently locked, i.e. after calling Draw_Begin().
bool Gods98::Draw_IsLocked()
{
	return _drawLocked || (drawGlobs.buffer != nullptr);
}

/// CUSTOM: Locks the drawing surface and waits for Draw_End() to be called before unlocking it.
bool Gods98::Draw_Begin(OPTIONAL Image* renderTarget)
{
	Error_Warn(Draw_IsLocked(), "Draw_Begin: Called more than once before calling Draw_End()");
	Error_Fatal(Draw_IsLocked() && renderTarget != _drawRenderTarget, "Draw_Begin: Called with a different render target before calling Draw_End()");

	// Will still set new effect if already locked.
	IDirectDrawSurface4* surf = (renderTarget ? Image_GetSurface(renderTarget) : DirectDraw_bSurf());
	const bool alreadyLocked = Draw_IsLocked();
	if (Draw_LockSurface(surf, Gods98::DrawEffect::None)) {
		_drawBegin = true;

		if (!alreadyLocked) {
			// If the user decided to continue past the fatal error, don't change the render target unless we were unlocked.
			_drawRenderTarget = renderTarget;

			if (_drawRenderTarget != nullptr) {
				// When drawing to an image, expect the image to be drawn at the render scale.
				// So we don't want to scale drawing to the image only to have the image be scaled again when being displayed.
				Draw_SetScale(1, false);
			}

			// When drawing to an image, we need to reset the clip window, since it's normally set for the back surface.
			// To stay consistent, we'll reset the clip window every time Draw_Begin is called.
			Draw_SetClipWindow(nullptr);
		}
	}
	return _drawBegin;
}

/// CUSTOM: Unlocks the drawing surface after Draw_Begin() was called.
void Gods98::Draw_End()
{
	Error_Warn(!Draw_IsLocked(), "Draw_End: Called without first calling Draw_Begin()");

	IDirectDrawSurface4* surf = (_drawRenderTarget ? Image_GetSurface(_drawRenderTarget) : DirectDraw_bSurf());
	Draw_UnlockSurface(surf);
	_drawBegin = false;
}

sint32 Gods98::Draw_GetScale()
{
	return _drawScale;
}

sint32 Gods98::Draw_SetScale(sint32 scale, bool relative)
{
	Error_Warn(!Draw_IsLocked(), "Draw_SetScale: Called without first calling Draw_Begin()");
	
	if (Draw_IsLocked()) {
		if (relative) {
			if (scale == 0) {
				scale = Main_RenderScale();
			}
			else if (scale < 0) {
				// This is pretty terrible...
				scale = (Main_RenderScale() / (-1 - scale)); // -1 = / 2, -2 = / 3
			}
			else {
				scale *= Main_RenderScale();
			}
		}
		else {
			if (scale <= 0) {
				scale = Main_RenderScale();
			}
		}
		_drawScale = static_cast<uint32>(std::max(1, scale));
	}
	return static_cast<sint32>(_drawScale);
}

/// CUSTOM: Gets the added translation offset for drawing coordinates.
Point2F Gods98::Draw_GetTranslation()
{
	return _drawTranslation;
}

/// CUSTOM: Sets the added translation offset for drawing coordinates.
void Gods98::Draw_SetTranslation(Point2F translation, bool relative)
{
	Error_Warn(!Draw_IsLocked(), "Draw_SetTranslation: Called without first calling Draw_Begin()");

	if (Draw_IsLocked()) {
		if (relative) {
			Gods98::Maths_Vector2DAdd(&_drawTranslation, &_drawTranslation, &translation);
		}
		else {
			_drawTranslation = translation;
		}
	}
}

/// CUSTOM: Gets the current effect while the drawing surface has been locked with Draw_Begin().
Gods98::DrawEffect Gods98::Draw_GetEffect()
{
	return _drawEffect;
}

/// CUSTOM: Sets the current effect while the drawing surface has been locked with Draw_Begin().
void Gods98::Draw_SetEffect(DrawEffect effect)
{
	Error_Warn(!Draw_IsLocked(), "Draw_SetEffect: Called without first calling Draw_Begin()");

	// We need to know the bitdepth to set the pixel function, so we must be locked.
	if (Draw_IsLocked()) {
		Draw_SetDrawPixelFunc(effect);
	}
}

/// CUSTOM:
void Gods98::Draw_AssertUnlocked(const char* caller, OPTIONAL Image* renderTarget)
{
	if (Draw_IsLocked()) {
		if (renderTarget == nullptr || renderTarget == _drawRenderTarget) {
			Error_FatalF(true, "%s: Draw_End() has not been called before the surface is needed", caller);
			Draw_End(); // End for users that want to continue past the error dialog.
		}
	}
}


// <LegoRR.exe @00486140>
void __cdecl Gods98::Draw_Initialise(OPTIONAL const Area2F* window)
{
	log_firstcall();

	drawGlobs.flags |= Draw_GlobFlags::DRAW_GLOB_FLAG_INITIALISED;
	Draw_SetClipWindow(window);
}

// <LegoRR.exe @00486160>
void __cdecl Gods98::Draw_SetClipWindow(OPTIONAL const Area2F* window)
{
	log_firstcall();

	IDirectDrawSurface4* surf = (_drawRenderTarget ? Image_GetSurface(_drawRenderTarget) : DirectDraw_bSurf());

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_GLOB_FLAG_INITIALISED), "Draw not initialised");

	drawGlobs.clipStart.x = 0.0f;
	drawGlobs.clipStart.y = 0.0f;
	if (window) {
		drawGlobs.clipStart.x = std::max(drawGlobs.clipStart.x, window->x);
		drawGlobs.clipStart.y = std::max(drawGlobs.clipStart.y, window->y);
	}

	DDSURFACEDESC2 desc = { 0 };
	desc.dwSize = sizeof(DDSURFACEDESC2);

	if (surf->GetSurfaceDesc(&desc) == DD_OK) {
		drawGlobs.clipEnd.x = static_cast<real32>(desc.dwWidth);
		drawGlobs.clipEnd.y = static_cast<real32>(desc.dwHeight);
		if (window) {
			drawGlobs.clipEnd.x = std::min(drawGlobs.clipEnd.x, (window->x + window->width));
			drawGlobs.clipEnd.y = std::min(drawGlobs.clipEnd.y, (window->y + window->height));
		}
	}
	else {
		Error_Warn(true, "Draw_SetClipWindow: Failed to GetSurfaceDesc");
	}

	if (_drawRenderTarget == nullptr) {
		// If this is the back surface, then *backup* our clip window for when using a different render target.
		_drawBackClipStart = drawGlobs.clipStart;
		_drawBackClipEnd   = drawGlobs.clipEnd;
	}

//	drawGlobs.lockRect.left = (uint32) drawGlobs.clipStart.x;
//	drawGlobs.lockRect.top = (uint32) drawGlobs.clipStart.y;
//	drawGlobs.lockRect.right = (uint32) drawGlobs.clipEnd.x;
//	drawGlobs.lockRect.bottom = (uint32) drawGlobs.clipEnd.y;
}

// <LegoRR.exe @00486270>
void __cdecl Gods98::Draw_GetClipWindow(OUT Area2F* window)
{
	log_firstcall();

	window->x = drawGlobs.clipStart.x;
	window->y = drawGlobs.clipStart.y;
	window->width  = drawGlobs.clipEnd.x - drawGlobs.clipStart.x;
	window->height = drawGlobs.clipEnd.y - drawGlobs.clipStart.y;
}

// <unused>
void __cdecl Gods98::Draw_WorldLineListEx(Viewport* vp, const Vector3F* fromList, const Vector3F* toList, uint32 count, real32 r, real32 g, real32 b, real32 a, DrawEffect effect)
{
	IDirect3DViewport* viewport1 = nullptr;
	IDirect3DViewport3* viewport3 = nullptr;
	D3DMATRIX m = { 0.0f };

	m._11 = m._22 = m._33 = m._44 = 1.0f; // Identity matrix.

	// We allocate vertexList, so no bounds check needed.
	//Error_Fatal(count > DRAW_MAXLINES, "DRAW_MAXLINES too small");

	DrawLineVertex* vertexList = (DrawLineVertex*)Mem_Alloc(sizeof(DrawLineVertex) * count * 2);
	//Mesh_Vertex* vertex2List = (Mesh_Vertex*)Mem_Alloc(sizeof(Mesh_Vertex) * count * 2);

	const uint32 colour = Container_GetRGBAColour(r, g, b, a);

	for (uint32 loop = 0; loop < count; loop++) {
		vertexList[(loop*2) + 0].position = fromList[loop];
		vertexList[(loop*2) + 0].colour   = colour;
		vertexList[(loop*2) + 1].position = toList[loop];
		vertexList[(loop*2) + 1].colour   = colour;
		//vertex2List[(loop*2) + 0].position = fromList[loop];
		//vertex2List[(loop*2) + 0].colour = colour;
		//vertex2List[(loop*2) + 1].position = toList[loop];
		//vertex2List[(loop*2) + 1].colour = colour;
	}

	vp->lpVP->GetDirect3DViewport(&viewport1);
	viewport1->QueryInterface(IID_IDirect3DViewport3, (void**)&viewport3);
	viewport1->Release();
	lpIMDevice()->SetCurrentViewport(viewport3);
	lpIMDevice()->SetTexture(0, nullptr);
	lpIMDevice()->SetLightState(D3DLIGHTSTATE_MATERIAL, 0);
	lpIMDevice()->SetLightState(D3DLIGHTSTATE_COLORVERTEX, true);
	lpIMDevice()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m);
	//lpIMDevice()->SetTransform(D3DTRANSFORMSTATE_VIEW, &m);

	lpIMDevice()->BeginScene();

	Graphics_ChangeRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, true);
	Graphics_ChangeRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics_ChangeRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
	Graphics_ChangeRenderState(D3DRENDERSTATE_ZWRITEENABLE, false);
//	Graphics_ChangeRenderState(D3DRENDERSTATE_ZBIAS, 2);

	//#define DRAW_VERTEXFLAGS	(D3DFVF_DIFFUSE|D3DFVF_XYZ)
	lpIMDevice()->DrawPrimitive(D3DPT_LINELIST, DRAW_VERTEXFLAGS, vertexList, count * 2, D3DDP_WAIT);

	/*if (lpIMDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, renderFlags,
										   vertices, vertexCount, faceData, indexCount, 0) != D3D_OK)
	{
		Error_Warn(true, "Cannot 'DrawIndexedPrimitive'.");
	}*/

	lpIMDevice()->SetLightState(D3DLIGHTSTATE_COLORVERTEX, false);
	Graphics_ChangeRenderState(D3DRENDERSTATE_ZWRITEENABLE, true);
//	Graphics_ChangeRenderState(D3DRENDERSTATE_ZBIAS, 0);

	Graphics_RestoreStates();// false);
	lpIMDevice()->EndScene();


	viewport3->Release();

	Mem_Free(vertexList);
	vertexList = nullptr;
}

// <unused>
void __cdecl Gods98::Draw_PixelListEx(const Point2F* pointList, uint32 count, real32 r, real32 g, real32 b, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_GLOB_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {

		const uint32 colour = Draw_GetColour(r, g, b);

		for (uint32 loop = 0; loop < count; loop++) {
			Point2F point = pointList[loop];
			// Add translation:
			Gods98::Maths_Vector2DAdd(&point, &point, &_drawTranslation);

			if (point.x >= drawGlobs.clipStart.x && point.y >= drawGlobs.clipStart.y && point.x < drawGlobs.clipEnd.x && point.y < drawGlobs.clipEnd.y) {
				Draw_TryDrawPixel(static_cast<sint32>(point.x), static_cast<sint32>(point.y), colour);
			}
		}
		Draw_TryUnlockSurface(surf);
	}
}

// <LegoRR.exe @004862b0>
void __cdecl Gods98::Draw_LineListEx(const Point2F* fromList, const Point2F* toList, uint32 count, real32 r, real32 g, real32 b, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_GLOB_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {
		
		const uint32 colour = Draw_GetColour(r, g, b);
		
		for (uint32 loop = 0; loop < count; loop++) {
			Point2F from = fromList[loop];
			Point2F to   = toList[loop];
			// Add translation:
			Gods98::Maths_Vector2DAdd(&from, &from, &_drawTranslation);
			Gods98::Maths_Vector2DAdd(&to,   &to,   &_drawTranslation);

			Draw_LineActual(static_cast<sint32>(from.x), static_cast<sint32>(from.y),
							static_cast<sint32>(to.x),   static_cast<sint32>(to.y), colour);
		}
		Draw_TryUnlockSurface(surf);
	}
}

// <LegoRR.exe @00486350>
void __cdecl Gods98::Draw_RectListEx(const Area2F* rectList, uint32 count, real32 r, real32 g, real32 b, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_GLOB_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {

		const uint32 colour = Draw_GetColour(r, g, b);

		for (uint32 loop = 0; loop < count; loop++) {
			Area2F rect = rectList[loop];
			// Add translation:
			Gods98::Maths_Vector2DAdd(&rect.point, &rect.point, &_drawTranslation);

			Point2F end = {
				rect.x + rect.width,
				rect.y + rect.height
			};

			if (rect.x < drawGlobs.clipStart.x) rect.x = drawGlobs.clipStart.x;
			if (rect.y < drawGlobs.clipStart.y) rect.y = drawGlobs.clipStart.y;
			if (end.x >= drawGlobs.clipEnd.x) end.x = (drawGlobs.clipEnd.x - 1);
			if (end.y >= drawGlobs.clipEnd.y) end.y = (drawGlobs.clipEnd.y - 1);

			const sint32 ex = static_cast<sint32>(end.x);
			const sint32 ey = static_cast<sint32>(end.y);
			for (sint32 y = static_cast<sint32>(rect.y); y < ey; y++) {
				for (sint32 x = static_cast<sint32>(rect.x); x < ex; x++) {
					Draw_TryDrawPixel(x, y, colour);
				}
			}
		}
		Draw_TryUnlockSurface(surf);
	}
}

// <LegoRR.exe @004864d0>
void __cdecl Gods98::Draw_RectList2Ex(const Draw_Rect* rectList, uint32 count, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_GLOB_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {

		for (uint32 loop = 0; loop < count; loop++) {

			const uint32 colour = Draw_GetColour(rectList[loop].r, rectList[loop].g, rectList[loop].b);

			Area2F rect = rectList[loop].rect;
			// Add translation:
			Gods98::Maths_Vector2DAdd(&rect.point, &rect.point, &_drawTranslation);

			Point2F end = {
				rect.x + rect.width,
				rect.y + rect.height
			};

			if (rect.x < drawGlobs.clipStart.x) rect.x = drawGlobs.clipStart.x;
			if (rect.y < drawGlobs.clipStart.y) rect.y = drawGlobs.clipStart.y;
			if (end.x >= drawGlobs.clipEnd.x) end.x = (drawGlobs.clipEnd.x - 1);
			if (end.y >= drawGlobs.clipEnd.y) end.y = (drawGlobs.clipEnd.y - 1);

			const sint32 ex = static_cast<sint32>(end.x);
			const sint32 ey = static_cast<sint32>(end.y);
			for (sint32 y = static_cast<sint32>(rect.y); y < ey; y++) {
				for (sint32 x = static_cast<sint32>(rect.x); x < ex; x++) {
					Draw_TryDrawPixel(x, y, colour);
				}
			}
		}
		Draw_TryUnlockSurface(surf);
	}
}

// <LegoRR.exe @00486650>
void __cdecl Gods98::Draw_DotCircle(const Point2F* pos, uint32 radius, uint32 dots, real32 r, real32 g, real32 b, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_GLOB_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {

		const uint32 colour = Draw_GetColour(r, g, b);

		const real32 step = (2.0f * M_PI) / dots;

		// Add translation:
		Point2F point = *pos;
		Gods98::Maths_Vector2DAdd(&point, &point, &_drawTranslation);

		for (uint32 loop = 0; loop < dots; loop++) {
			const real32 angle = step * loop;
			const real32 x = (point.x + (Maths_Sin(angle) * radius));
			const real32 y = (point.y + (Maths_Cos(angle) * radius));
			if (x >= drawGlobs.clipStart.x && y >= drawGlobs.clipStart.y && x < drawGlobs.clipEnd.x && y < drawGlobs.clipEnd.y) {
				Draw_TryDrawPixel(static_cast<sint32>(x), static_cast<sint32>(y), colour);
			}
		}
		Draw_TryUnlockSurface(surf);
	}
}

// <LegoRR.exe @00486790>
uint32 __cdecl Gods98::Draw_GetColour(real32 r, real32 g, real32 b)
{
	log_firstcall();

	if (!Draw_IsRenderEnabled()) {
		return 0; // Behave as if colour was returned.
	}

	Error_Fatal(!Draw_IsLocked(), "Draw_GetColour: Must be called after Draw_LockSurface()");
	
	if (drawGlobs.bpp == 8) {
		/// FIXME: not implemented by GODS98 or LegoRR
		return 0;
	}
	else {
		return
			DirectDraw_ShiftChannelByte(static_cast<uint8>(r * 255.0f), drawGlobs.redBits,   _drawRedShift)   |
			DirectDraw_ShiftChannelByte(static_cast<uint8>(g * 255.0f), drawGlobs.greenBits, _drawGreenShift) |
			DirectDraw_ShiftChannelByte(static_cast<uint8>(b * 255.0f), drawGlobs.blueBits,  _drawBlueShift)  |
			_drawAlphaMask;
	}
}

// <LegoRR.exe @00486810>
bool32 __cdecl Gods98::Draw_LockSurface(IDirectDrawSurface4* surf, DrawEffect effect)
{
	log_firstcall();

	if (Draw_IsLocked()) {
		Draw_SetDrawPixelFunc(effect);
		return true; // Already locked.
	}

	if (!Draw_IsRenderEnabled()) {
		_drawLocked = true;
		Draw_SetTranslation(Point2F { 0.0f, 0.0f });
		Draw_SetScale(1, true);
		Draw_SetDrawPixelFunc(effect);
		return true; // Behave as if locked.
	}

	DDSURFACEDESC2 desc = { 0 };
	desc.dwSize = sizeof(DDSURFACEDESC2);

//	if (surf->Lock(reinterpret_cast<RECT*>(&drawGlobs.lockRect), &desc, DDLOCK_WAIT, nullptr) == DD_OK) {
	if (surf->Lock(nullptr, &desc, DDLOCK_WAIT, nullptr) == DD_OK) {

		_drawWidth          = desc.dwWidth;
		_drawHeight         = desc.dwHeight;
		drawGlobs.buffer    = desc.lpSurface;
		drawGlobs.pitch     = desc.lPitch;
		drawGlobs.redMask   = desc.ddpfPixelFormat.dwRBitMask;
		drawGlobs.greenMask = desc.ddpfPixelFormat.dwGBitMask;
		drawGlobs.blueMask  = desc.ddpfPixelFormat.dwBBitMask;
		_drawAlphaMask      = desc.ddpfPixelFormat.dwRGBAlphaBitMask;
		drawGlobs.bpp       = desc.ddpfPixelFormat.dwRGBBitCount;

		drawGlobs.redBits   = DirectDraw_CountMaskBits(drawGlobs.redMask);
		drawGlobs.greenBits = DirectDraw_CountMaskBits(drawGlobs.greenMask);
		drawGlobs.blueBits  = DirectDraw_CountMaskBits(drawGlobs.blueMask);
		_drawAlphaBits      = DirectDraw_CountMaskBits(_drawAlphaMask);

		_drawRedShift       = DirectDraw_CountMaskBitShift(drawGlobs.redMask);
		_drawGreenShift     = DirectDraw_CountMaskBitShift(drawGlobs.greenMask);
		_drawBlueShift      = DirectDraw_CountMaskBitShift(drawGlobs.blueMask);
		_drawAlphaShift     = DirectDraw_CountMaskBitShift(_drawAlphaMask);

		_drawLocked = true;
		Draw_SetTranslation(Point2F { 0.0f, 0.0f });
		Draw_SetScale(1, true);

		if (Draw_SetDrawPixelFunc(effect)) {
			return true;
		}

		Draw_UnlockSurface(surf);
	}

	return false;
}

// <LegoRR.exe @00486910>
void __cdecl Gods98::Draw_UnlockSurface(IDirectDrawSurface4* surf)
{
	log_firstcall();

	if (!Draw_IsRenderEnabled()) {
		// Behave as if unlocked.
	}
	else if (Draw_IsLocked()) {

	//	surf->Unlock(reinterpret_cast<RECT*>(&drawGlobs.lockRect));
		surf->Unlock(nullptr);
		_drawWidth          = 0;
		_drawHeight         = 0;
		drawGlobs.buffer    = nullptr;
		drawGlobs.pitch     = 0;
		drawGlobs.redMask   = 0;
		drawGlobs.greenMask = 0;
		drawGlobs.blueMask  = 0;
		_drawAlphaMask      = 0;
		drawGlobs.bpp       = 0;
		drawGlobs.drawPixelFunc = nullptr;
	}

	_drawEffect = DrawEffect::None;
	_drawBegin = false;
	_drawLocked = false;
	_drawScale = 1;
	_drawTranslation = Point2F { 0.0f, 0.0f };

	if (_drawRenderTarget != nullptr) {
		_drawRenderTarget = nullptr;

		// Restore back surface clip window in-case we were rendering using a render target.
		drawGlobs.clipStart = _drawBackClipStart;
		drawGlobs.clipEnd   = _drawBackClipEnd;
	}
}

// <LegoRR.exe @00486950>
bool32 __cdecl Gods98::Draw_SetDrawPixelFunc(DrawEffect effect)
{
	log_firstcall();

	if (!Draw_IsRenderEnabled()) {
		drawGlobs.drawPixelFunc = Draw_PixelDummy;
		_drawEffect = effect;
		return true; // Behave as if pixel function is set.
	}

	switch (drawGlobs.bpp) {
	case 8:
		drawGlobs.drawPixelFunc = Draw_Pixel8;
		_drawEffect = DrawEffect::None;
		return true;

	case 16:
		switch (effect) {
		case DrawEffect::None:		drawGlobs.drawPixelFunc = Draw_Pixel16; break;
		case DrawEffect::XOR:		drawGlobs.drawPixelFunc = Draw_Pixel16XOR; break;
		case DrawEffect::HalfTrans:	drawGlobs.drawPixelFunc = Draw_Pixel16HalfTrans; break;
		default:					return false;
		}
		_drawEffect = effect;
		return true;

	case 24:
		switch (effect) {
		case DrawEffect::None:		drawGlobs.drawPixelFunc = Draw_Pixel24; break;
		case DrawEffect::XOR:		drawGlobs.drawPixelFunc = Draw_Pixel24XOR; break;
		case DrawEffect::HalfTrans:	drawGlobs.drawPixelFunc = Draw_Pixel24HalfTrans; break;
		default:					return false;
		}
		_drawEffect = effect;
		return true;

	case 32:
		switch (effect) {
		case DrawEffect::None:		drawGlobs.drawPixelFunc = Draw_Pixel32; break;
		case DrawEffect::XOR:		drawGlobs.drawPixelFunc = Draw_Pixel32XOR; break;
		case DrawEffect::HalfTrans:	drawGlobs.drawPixelFunc = Draw_Pixel32HalfTrans; break;
		default:					return false;
		}
		_drawEffect = effect;
		return true;

	default:
		drawGlobs.drawPixelFunc = nullptr;
		return false;
	}
}

// Bresenham's algorithm (line drawing)
// see: <http://www.brackeen.com/vga/shapes.html#shapes3.0>
// also see: <https://github.com/trigger-segfault/AsciiArtist/blob/75c75467d4f5fa2da02c1ba9712deb16529bbbde/PowerConsoleLib/PowerConsole/Drawing/Graphics.cpp#L459-L499>
// <LegoRR.exe @004869e0>
void __cdecl Gods98::Draw_LineActual(sint32 x1, sint32 y1, sint32 x2, sint32 y2, uint32 colour)
{
	// Note: Translation should be applied before calling this function.

	log_firstcall();

	sint32 numpixels;
	sint32 d, dinc1, dinc2;

	// Sign of the distance
	sint32 xinc1, xinc2;
	sint32 yinc1, yinc2;

	// Absolute distance
	const sint32 deltax = std::abs(x2 - x1);
	const sint32 deltay = std::abs(y2 - y1);

	// different drawing behavior depending on the larger coordinate plane distance
	if (deltax >= deltay) {
		numpixels = deltax + 1;
		d = 2 * deltay - deltax;
		dinc1 = deltay * 2;
		dinc2 = (deltay - deltax) * 2;
		xinc1 = 1;
		xinc2 = 1;
		yinc1 = 0;
		yinc2 = 1;
	}
	else {
		numpixels = deltay + 1;
		d = (2 * deltax) - deltay;
		dinc1 = deltax * 2;
		dinc2 = (deltax - deltay) * 2;
		xinc1 = 0;
		xinc2 = 1;
		yinc1 = 1;
		yinc2 = 1;
	}
	
	if (x1 > x2) {
		xinc1 = -xinc1;
		xinc2 = -xinc2;
	}
	if (y1 > y2) {
		yinc1 = -yinc1;
		yinc2 = -yinc2;
	}
	sint32 x = x1;
	sint32 y = y1;
	
	for (sint32 loop = 1; loop <= numpixels; loop++) {
		
		if (x >= drawGlobs.clipStart.x && y >= drawGlobs.clipStart.y && x < drawGlobs.clipEnd.x && y < drawGlobs.clipEnd.y) {
			Draw_TryDrawPixel(x, y, colour);
		}
		
		if (d < 0) {
			d += dinc1;
			x += xinc1;
			y += yinc1;
		}
		else {
			d += dinc2;
			x += xinc2;
			y += yinc2;
		}
	}
}

/// CUSTOM: Function to wrap around calling pixelDrawFunc.
void Gods98::_Draw_DrawPixel(sint32 x, sint32 y, uint32 value)
{
	x *= static_cast<sint32>(_drawScale);
	y *= static_cast<sint32>(_drawScale);
	for (uint32 yOff = 0; yOff < _drawScale; yOff++) {
		for (uint32 xOff = 0; xOff < _drawScale; xOff++) {
			// Use unsigned int for cheaper comparisons without `> 0`.
			if (static_cast<uint32>(x + xOff) < _drawWidth &&
				static_cast<uint32>(y + yOff) < _drawHeight)
			{
				Gods98::drawGlobs.drawPixelFunc(x + xOff, y + yOff, value);
			}
		}
	}
}

/// CUSTOM:
uint32 Gods98::_Draw_ConvertHalfTrans(uint32 pixel, uint32 value)
{
	const uint32 pix_r = ((pixel & drawGlobs.redMask)   >> _drawRedShift);
	const uint32 pix_g = ((pixel & drawGlobs.greenMask) >> _drawGreenShift);
	const uint32 pix_b = ((pixel & drawGlobs.blueMask)  >> _drawBlueShift);

	const uint32 val_r = ((value & drawGlobs.redMask)   >> _drawRedShift);
	const uint32 val_g = ((value & drawGlobs.greenMask) >> _drawGreenShift);
	const uint32 val_b = ((value & drawGlobs.blueMask)  >> _drawBlueShift);

	const uint32 r = (((pix_r + val_r) / 2) << _drawRedShift)   & drawGlobs.redMask;
	const uint32 g = (((pix_g + val_g) / 2) << _drawGreenShift) & drawGlobs.greenMask;
	const uint32 b = (((pix_b + val_b) / 2) << _drawBlueShift)  & drawGlobs.blueMask;

	return (r|g|b);
}

/// CUSTOM:
void __cdecl Gods98::Draw_PixelDummy(sint32 x, sint32 y, uint32 value)
{
	// Do nothing.
}

// <LegoRR.exe @00486b40>
void __cdecl Gods98::Draw_Pixel8(sint32 x, sint32 y, uint32 value)
{
	uint8* addr = Draw_Pixel8Address(x, y);
	*addr = static_cast<uint8>(value);
}

// <LegoRR.exe @00486b60>
void __cdecl Gods98::Draw_Pixel16(sint32 x, sint32 y, uint32 value)
{
	uint16* addr = Draw_Pixel16Address(x, y);
	*addr = static_cast<uint16>(value);
}

// <LegoRR.exe @00486b90>
void __cdecl Gods98::Draw_Pixel16XOR(sint32 x, sint32 y, uint32 value)
{
	uint16* addr = Draw_Pixel16Address(x, y);
	*addr ^= static_cast<uint16>(value);
}

// <LegoRR.exe @00486bc0>
void __cdecl Gods98::Draw_Pixel16HalfTrans(sint32 x, sint32 y, uint32 value)
{
	uint16* addr = Draw_Pixel16Address(x, y);

	value = _Draw_ConvertHalfTrans(*addr, static_cast<uint16>(value));
	*addr = static_cast<uint16>(value);

	//uint16 r = (((*addr >> 12) + ((uint16) value >> 12)) << 11) & (uint16) drawGlobs.redMask;
	//uint16 g = ((((*addr & (uint16) drawGlobs.greenMask) >> 6) + (((uint16) value & (uint16) drawGlobs.greenMask) >> 6)) << 5) & (uint16) drawGlobs.greenMask;
	//uint16 b = (((*addr & (uint16) drawGlobs.blueMask) >> 1) + (((uint16) value & (uint16) drawGlobs.blueMask) >> 1)) & (uint16) drawGlobs.blueMask;

	//*addr = (r|g|b);
}

// <LegoRR.exe @00486c60>
void __cdecl Gods98::Draw_Pixel24(sint32 x, sint32 y, uint32 value)
{
	uint8* addr = Draw_Pixel24Address(x, y);
	addr[0] = static_cast<uint8>(value      );
	addr[1] = static_cast<uint8>(value >>  8);
	addr[2] = static_cast<uint8>(value >> 16);
}

/// CUSTOM:
void __cdecl Gods98::Draw_Pixel24XOR(sint32 x, sint32 y, uint32 value)
{
	uint8* addr = Draw_Pixel24Address(x, y);
	addr[0] ^= static_cast<uint8>(value      );
	addr[1] ^= static_cast<uint8>(value >>  8);
	addr[2] ^= static_cast<uint8>(value >> 16);
}

/// CUSTOM:
void __cdecl Gods98::Draw_Pixel24HalfTrans(sint32 x, sint32 y, uint32 value)
{
	uint8* addr = Draw_Pixel24Address(x, y);

	const uint32 pixel = (addr[0] | (addr[1] << 8) | (addr[2] << 16));
	value = _Draw_ConvertHalfTrans(pixel, value);
	addr[0] = static_cast<uint8>(value      );
	addr[1] = static_cast<uint8>(value >>  8);
	addr[2] = static_cast<uint8>(value >> 16);
}

// <LegoRR.exe @00486c90>
void __cdecl Gods98::Draw_Pixel32(sint32 x, sint32 y, uint32 value)
{
	uint32* addr = Draw_Pixel32Address(x, y);
	// Ignore alpha, Draw isn't designed to work with it.
	*addr = (value | (*addr & _drawAlphaMask));
}

/// CUSTOM:
void __cdecl Gods98::Draw_Pixel32XOR(sint32 x, sint32 y, uint32 value)
{
	uint32* addr = Draw_Pixel32Address(x, y);
	// Ignore alpha, Draw isn't designed to work with it.
	*addr ^= (value & ~_drawAlphaMask);
}

/// CUSTOM:
void __cdecl Gods98::Draw_Pixel32HalfTrans(sint32 x, sint32 y, uint32 value)
{
	uint32* addr = Draw_Pixel32Address(x, y);

	// Ignore alpha, Draw isn't designed to work with it.
	value = _Draw_ConvertHalfTrans(*addr & ~_drawAlphaMask, value & ~_drawAlphaMask);
	*addr = (value | (*addr & _drawAlphaMask));
}


#pragma endregion
