// Input.h : 
//
/// APIS: IDirectInputA(8), IDirectInputDeviceA(8), IDirectInputDevice2A(unused)
/// DEPENDENCIES: Main, (Dxbug, Errors)
/// DEPENDENTS: Main, Credits, FrontEnd, NERPs, Objective, Panel, Priorities

#pragma once

#include "../../common.h"
#include "../geometry.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0500
#elif DIRECTINPUT_VERSION != 0x0500
#undef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0500
#endif
#include "../../platform/dinput.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

/*typedef struct _DIMOUSESTATE {
	LONG    lX;
	LONG    lY;
	LONG    lZ;
	BYTE    rgbButtons[4];
} DIMOUSESTATE, * LPDIMOUSESTATE;*/

#if DIRECTINPUT_VERSION == 0x0500
struct IDirectInputA;
struct IDirectInputDeviceA;

#else
struct IDirectInput8A;
struct IDirectInputDevice8A;
#endif

struct IDirectInputDevice2A;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

// looked at the win98 joystick stuff and it doesn't let you have more than 4 buttons
#define MAX_JOYSTICK_BUTTONS		4

#define INPUT_MAXKEYS				256

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct joystickType
{
	/*00,4*/ bool32					found;
	/*04,4*/ IDirectInputDevice2A* dev;
	/*08,10*/ bool32					buttons[MAX_JOYSTICK_BUTTONS];
	/*18,4*/ sint32						xaxis;
	/*1c,4*/ sint32						yaxis;
	/*20*/
};
assert_sizeof(joystickType, 0x20);


struct DIcallbackData
{
#if DIRECTINPUT_VERSION == 0x0500
	/*0,4*/ IDirectInputA* di;
#else
	/*0,4*/ IDirectInput8A* di;
#endif
	/*4,4*/ HWND hWnd;
	/*8,4*/ joystickType* joystick;
	/*c*/
};
assert_sizeof(DIcallbackData, 0xc);


struct Input_Globs
{
	/*000,100*/ bool8 prevKey_Map[INPUT_MAXKEYS]; // Keyboard state
	// [globs: start]
#if DIRECTINPUT_VERSION == 0x0500
	/*100,4*/ IDirectInputA* lpdi;
	/*104,4*/ IDirectInputDeviceA* lpdiKeyboard;
	/*108,4*/ IDirectInputDeviceA* lpdiMouse;		// (unused)
#else
	/*100,4*/ IDirectInput8A* lpdi;
	/*104,4*/ IDirectInputDevice8A* lpdiKeyboard;
	/*108,4*/ IDirectInputDevice8A* lpdiMouse;		// (unused)
#endif
	/*10c,4*/ bool32 fKeybdAcquired;
	/*110,4*/ bool32 fMouseAcquired;							// (unused)
	/*114,10*/ Rect2I MouseBounds;						// Bounding box in which mouse may move
	/*124,4*/ sint32 msx;
	/*128,4*/ sint32 msy;
	/*12c,4*/ sint32 prev_msx;
	/*130,4*/ sint32 prev_msy;		// Current and previous positions of the mouse
	/*134,4*/ sint32 diffx;
	/*138,4*/ sint32 diffy;						// Relative movement of mouse cursor
	/*13c,4*/ bool32 mslb;
	/*140,4*/ bool32 msrb;						// Mouse buttons
	/*144,4*/ bool32 mslblast;
	/*148,4*/ bool32 mslbheld;
	/*14c,4*/ bool32 lClicked;
	/*150,4*/ bool32 rClicked;
	/*154,4*/ bool32 lDoubleClicked;
	/*158,4*/ bool32 rDoubleClicked;
	/*15c,4*/ bool32 caps;
	/*160,10*/ DIMOUSESTATE dims;
	// [globs: end]
	/*170,c*/ DIcallbackData cbd;
	/*17c,1*/ char Input_KeyTemp;
	// THIS PADDING IS NEEDED so that Key_Map aligns to 180
	/*17d,3*/ uint8 padding1[3];
	/*180,100*/ bool8 Key_Map[INPUT_MAXKEYS]; // Keyboard state
	/*280*/
};
assert_sizeof(Input_Globs, 0x280);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0076ba00>
extern Input_Globs & INPUT;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// External Function Prototypes

// Mouse data
// <LegoRR.exe @00410a60>
sint32 __cdecl noinline(msx)(void);
__inline sint32 msx(void) { return INPUT.msx; }		// Mouse positions

// <LegoRR.exe @00410a70>
sint32 __cdecl noinline(msy)(void);
__inline sint32 msy(void) { return INPUT.msy; }


// <LegoRR.exe @00410a80>
bool32 __cdecl noinline(mslb)(void);
__inline bool32 mslb(void) { return INPUT.mslb; }		// Left and right mouse buttons


// <inlined>
__inline bool32 msrb(void) { return INPUT.msrb; }

// <inlined>
__inline sint32 msxr(void) { return INPUT.diffx; }		// Relative mouse movement

// <inlined>
__inline sint32 msyr(void) { return INPUT.diffy; }

// <inlined>
__inline bool32 mslbheld(void) { return INPUT.mslbheld; }

// <inlined>
__inline bool32 Input_LClicked(void) { return INPUT.lClicked; }



// Initialise direct input and the key board
// <LegoRR.exe @0047f050>
bool32 __cdecl Input_InitKeysAndDI(void);

// Read the key board (called from message peek)
// <LegoRR.exe @0047f1b0>
void __cdecl Input_ReadKeys(void);

// Any key pressed (returns number of keys pressed)
// <LegoRR.exe @0047f270>
uint32 __cdecl Input_AnyKeyPressed(void);

// Release the keys and direct input
// <LegoRR.exe @0047f290>
void __cdecl Input_ReleaseKeysAndDI(void);

// Read the mouse state
// <LegoRR.exe @0047f2d0>
void __cdecl Input_ReadMouse2(void);

// <LegoRR.exe @0047f390>
BOOL __cdecl Input_SetCursorPos(sint32 x, sint32 y);




// Initialize the mouse
//bool32 __cdecl Input_InitMouse(bool32 GrabExclusiveAccess, sint32 sx, sint32 sy, Rect2I Bounds);

/*// Release the mouse
void __cdecl Input_ReleaseMouse(void);


extern	bool32			ReadJoystickInput(IN OUT joystickType*	joystick);
extern	bool32			InitJoystick(HINSTANCE hInst, HWND hWnd, lpjoystickType joystick);

extern	HRESULT			SetDIDwordProperty(LPDIRECTINPUTDEVICE pdev, REFGUID guidProperty, DWORD dwObject, DWORD dwHow, DWORD dwValue);
extern	BOOL __stdcall	InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef);

extern	bool32			ReacquireInput(joystickType* joystick);
extern	void			CleanupJoystickInput(joystickType* joystick);

extern	bool32			IsJoystickInitialised(const joystickType* joystick);
extern	void			Input_ClearClicked(void);
extern	uint8			Input_GetAsciiFromScanCode(uint32 i);


VOID Input_ReadMouse2(VOID);*/

// Static Function Prototypes

/**
 * @brief Gets if the current key state is 'down'.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsKeyDown(k)		((bool)Gods98::INPUT.Key_Map[(k)])
/**
 * @brief Gets if the current key state is 'up'.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsKeyUp(k)		(!Gods98::INPUT.Key_Map[(k)])

/**
 * @brief Gets if the previous key state was 'down'.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsPrevKeyDown(k)	((bool)Gods98::INPUT.prevKey_Map[(k)])
/**
 * @brief Gets if the previous key state was 'up'.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsPrevKeyUp(k)	(!Gods98::INPUT.prevKey_Map[(k)])

/**
 * @brief Gets if the current key state has changed from 'up' to 'down'.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsKeyPressed(k)	(Input_IsKeyDown((k)) && Input_IsPrevKeyUp((k)))
/**
 * @brief Gets if the current key state has changed from 'down' to 'up'.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsKeyReleased(k)	(Input_IsKeyUp((k)) && Input_IsPrevKeyDown((k)))
/**
 * @brief Gets if the current key state is different from the previous.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsKeyChanged(k)	(Input_IsKeyDown((k)) != Input_IsPrevKeyDown((k)))

/**
 * @brief Gets if the current AND PREVIOUS key state is 'down'.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsKeyHeld(k)		(Input_IsKeyDown((k)) && Input_IsPrevKeyDown((k)))
/**
 * @brief Gets if the current AND PREVIOUS key state is 'up'.
 * @param k Key code defined by `Keys` enum.
 */
#define Input_IsKeyUntouched(k)	(Input_IsKeyUp((k)) && Input_IsPrevKeyUp((k)))


/**
 * @brief Clears the CURRENT key state by setting to 'up'.
 */
#define Input_ClearKey(k)		(Gods98::INPUT.Key_Map[(k)] = false)
/**
 * @brief Clears the CURRENT keyboard state by setting all keys to 'up'.
 */
#define Input_ClearAllKeys()	std::memset(Gods98::INPUT.Key_Map, 0, sizeof(Gods98::INPUT.Key_Map))



//#define Input_KeyHit(k)			(Gods98::INPUT.Key_Map[(k)]&&(Gods98::INPUT.Key_Map[(k)]^Gods98::INPUT.prevKey_Map[(k)]))
//#define Input_KeyReleased(k)	((!Gods98::INPUT.Key_Map[(k)])&&(Gods98::INPUT.Key_Map[(k)]^Gods98::INPUT.prevKey_Map[(k)]))
//#define Input_KeyIsDown(k)		Gods98::INPUT.Key_Map[(k)]

#pragma endregion

}

