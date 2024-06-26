// Fallin.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define FALLIN_MAXTRIES			100 // Number of attempts to generate fallin at random block

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Fallin_Globs // [LegoRR/Fallin.c|struct:0x4|tags:GLOBS] Just a single field for Fallins (most other settings are found in Lego_Globs)
{
	/*0,4*/	uint32 numLandSlidesTillCaveIn;
	/*4*/
};
assert_sizeof(Fallin_Globs, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a2ee4>
extern Fallin_Globs & fallinGlobs; // fallinGlobs_NumberOfLandSlidesTillCaveIn;

// <LegoRR.exe @004d88c4>
extern real32 & s_fallinUpdateTimer; // Count-down timer until next fallin generation.

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040f010>
//#define Fallin_Update ((void (__cdecl* )(real32 elapsedWorld))0x0040f010)
void __cdecl Fallin_Update(real32 elapsedWorld);

// <LegoRR.exe @0040f0c0>
//#define Fallin_TryGenerateLandSlide ((bool32 (__cdecl* )(const Point2I* blockPos, bool32 allowCaveIn))0x0040f0c0)
bool32 __cdecl Fallin_TryGenerateLandSlide(const Point2I* blockPos, bool32 allowCaveIn);

// <LegoRR.exe @0040f1e0>
//#define Fallin_CanLandSlideAtBlock ((bool32 (__cdecl* )(const Point2I* blockPos))0x0040f1e0)
bool32 __cdecl Fallin_CanLandSlideAtBlock(const Point2I* blockPos);

// <LegoRR.exe @0040f260>
//#define Fallin_GenerateLandSlide ((void (__cdecl* )(const Point2I* blockPos, DirectionFlags fallinDirs, bool32 allowCaveIn))0x0040f260)
void __cdecl Fallin_GenerateLandSlide(const Point2I* blockPos, DirectionFlags fallinDirs, bool32 allowCaveIn);

// <LegoRR.exe @0040f510>
//#define Fallin_Initialise ((void (__cdecl* )(uint32 numLandSlidesTillCaveIn))0x0040f510)
void __cdecl Fallin_Initialise(uint32 numLandSlidesTillCaveIn);

// <LegoRR.exe @0040f520>
//#define Fallin_GenerateCaveIn ((void (__cdecl* )(const Point2I* blockPos, DirectionFlags fallinDirs))0x0040f520)
void __cdecl Fallin_GenerateCaveIn(const Point2I* blockPos, DirectionFlags fallinDirs);

#pragma endregion

}
