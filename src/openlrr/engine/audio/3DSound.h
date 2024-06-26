// 3DSound.h : 
//
/// FILEIO: Data[wad,std,cd]
/// APIS: IDirect3DRMFrame3,
///       IDirectSound, IDirectSoundBuffer, IDirectSound3DBuffer, IDirectSound3DListener
/// DEPENDENCIES: Containers, Files, Sound, (Errors, Memory)
/// DEPENDENTS: Sound, ...

#pragma once

#include "../../platform/windows.h"

// no getting around this include without some very ugly work-arounds :(
#include <mmsystem.h>

#include "../../common.h"
#include "../geometry.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirectSound;
struct IDirectSoundBuffer;
struct IDirectSound3DBuffer;
struct IDirectSound3DListener;

struct IDirect3DRMFrame3;
typedef IDirect3DRMFrame3* LPDIRECT3DRMFRAME3;
typedef float D3DVALUE;

/*struct HMMIO__;
typedef struct HMMIO__ *HMMIO;
struct _MMCKINFO;
typedef struct _MMCKINFO MMCKINFO;
struct tWAVEFORMATEX;
typedef struct tWAVEFORMATEX WAVEFORMATEX;*/

#pragma endregion


namespace Gods98
{; // !<---
	
/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define SOUND3D_MAXSAMPLES						600

#define SOUND3D_MAXSIMULTANEOUS					3

#define SOUND3D_MAXUPDATEFRAMES					20

#define SOUND3D_DEFAULTSOUNDVOLUME				(-300)

#define SOUND3D_NUMOFPLAYNOTIFICATIONS			4

#pragma endregion
	
/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

flags_scoped(Sound3DFlags) : uint32
{
	SOUND3D_FLAG_NONE   = 0,

	SOUND3D_FLAG_USED   = 0x1,
	SOUND3D_FLAG_ACTIVE = 0x2,
	SOUND3D_FLAG_MULTI  = 0x4,
	SOUND3D_FLAG_STREAM = 0x8,

	// Storage for Sound3DPlay::Normal flag, so that we can adjust playback volume of live sounds.
	// This flag takes up SOUND3D_MAXSIMULTANEOUS bits.
	SOUND3D_PLAY_FLAG_NORMAL = (1<<(32-SOUND3D_MAXSIMULTANEOUS)), // Use the highest bits of the flags.
};
flags_scoped_end(Sound3DFlags, 0x4);


// (unused)
flags_scoped(Sound3D_GlobFlags) : uint32
{
	SOUND3D_GLOB_FLAG_NONE = 0,
};
flags_scoped_end(Sound3D_GlobFlags, 0x4);


enum class Sound3DPlay : sint32 // Sound3D_PlayTAG
{
	OnFrame = 0,
	OnPos   = 1,
	Normal  = 2,
};
assert_sizeof(Sound3DPlay, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Sound3D_WaveData
{
	/*00,4*/ WAVEFORMATEX* pwfx;		    // Wave Format data structure
	/*04,4*/ HMMIO hmmio;		    // MM I/O handle for the WAVE
	/*08,14*/ MMCKINFO mmck;		    // Multimedia RIFF chunk
	/*1c,14*/ MMCKINFO mmckInRIFF;	    // Use in opening a WAVE file
	//IDirectSoundBuffer* lpDSBStreamBuffer; // Points to DirectSoundBuffer
	/*30,4*/ DWORD dwBufferSize;	    // Size of the entire buffer
	/*34,4*/ DWORD dwNotifySize;		// size of each notification period.
	/*38,4*/ DWORD dwNextWriteOffset; // Offset to next buffer segment
	/*3c,4*/ DWORD dwProgress;	    // Used with above to show prog.
	/*40,4*/ DWORD dwNextProgressCheck;
	/*44,4*/ DWORD dwLastPos;			// the last play position returned by GetCurrentPos().
	//DWORD dwBytesRemaining;  // Bytes 'til timer shutdown
	/*48,4*/ bool32 bDonePlaying;	    // Signals early abort to timer
	/*4c,4*/ bool32 bLoopFile;	    // Should we loop playback?
	/*50,4*/ bool32 bFoundEnd;	    // Timer found file end
	/*54*/
};
assert_sizeof(Sound3D_WaveData, 0x54);


struct Sound3D_StreamData
{
	/*00,4*/ bool32 fileOpen;
	/*04,54*/ Sound3D_WaveData wiWave;
	//DWORD dsbCheckPos[NUM_PLAY_NOTIFICATIONS + 1];
	/*58,4*/ bool32 playing;
	/*c*/
};
assert_sizeof(Sound3D_StreamData, 0x5c);


struct Sound3D_SoundRecord
{
	/*00,4*/ IDirect3DRMFrame3* frame;
	/*04,4*/ IDirectSoundBuffer* soundBuff;
	/*08,4*/ IDirectSound3DBuffer* sound3DBuff;
	/*0c,4*/ Sound3D_SoundRecord* next;
	/*10*/
};
assert_sizeof(Sound3D_SoundRecord, 0x10);


struct Sound3D_SoundFrameRecord
{
	/*00,4*/ IDirectSound3DBuffer* sound3DBuff;
	/*04,c*/ Vector3F pos;
	/*10,4*/ Sound3D_SoundFrameRecord* next;
	/*14*/
};
assert_sizeof(Sound3D_SoundFrameRecord, 0x14);


struct Sound3D_SoundData
{
	/*000,104*/ char fName[MAX_PATH];
	/*104,4*/ uint32 size;
	/*108,4*/ uint32 avgBytesPerSec;				//ONLY SET FOR STREAMING BUFFERS
	/*10c,4*/ uint32 freq;
	/*110,4*/ sint32 volume;
	/*114,4*/ uint32 offset;
	/*118,4*/ uint8* data;
	/*11c,4*/ WAVEFORMATEX* pwf;
	/*120,c*/ IDirectSoundBuffer* lpDsb3D[SOUND3D_MAXSIMULTANEOUS];
	/*12c,4*/ uint32 voice;
	/*130,4*/ Sound3DFlags flags;
	/*134*/
};
assert_sizeof(Sound3D_SoundData, 0x134);


struct Sound3D_Globs
{
	// [globs: start]
	/*00000,4*/ IDirectSound* lpDSnd;
	/*00004,4*/ IDirectSoundBuffer* lpDSBuff;
	/*00008,4*/ IDirectSound3DListener* lp3DListenerInfo;
	/*0000c,4*/ IDirectSoundBuffer* lpDSStreamBuff;
	/*00010,4*/ IDirectSoundBuffer* lpDSLoopStreamBuff;
	/*00014,5c*/ Sound3D_StreamData streamData;
	/*00070,5c*/ Sound3D_StreamData loopStreamData;
	/*000cc,4*/ Sound3D_SoundRecord* soundRecord;
	/*000d0,4*/ IDirect3DRMFrame3* listenerFrame;
	/*000d4,4*/ real32 minDistanceForAttentuation;
	/*000d8,4*/ real32 maxDistance;
	/*000dc,4*/ sint32 volume;
	/*000e0,2d1e0*/ Sound3D_SoundData soundTable[SOUND3D_MAXSAMPLES];
	/*2d2c0,4*/ bool32 intialised;
	/*2d2c4,4*/ sint32 windowsVolume;
	/*2d2c8,4*/ Sound3D_GlobFlags flags; // (unused)
	/*2d2cc,50*/ IDirect3DRMFrame3* updateFrameList[SOUND3D_MAXUPDATEFRAMES];
	// [globs: end]
	/*2d31c,4*/ uint32 reserved1;
	/*2d320,c*/ Vector3F s_ListenerCallback_oldPos;
	/*2d32c,4*/ uint32 reserved2;
	/*2d330,c*/ Vector3F s_ListenerCallback_oldOrien;
	/*2d33c,4*/ uint32 reserved3;
	/*2d340,c*/ Vector3F s_ListenerCallback_oldOrienUp;
	/*2d34c*/
};
assert_sizeof(Sound3D_Globs, 0x2d34c);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

//[0xFFFFD8F0, 0xFFFFEC78, 0xFFFFF254, 0xFFFFF830, 0xFFFFFA24, 0xFFFFFC18, 0xFFFFFD44, 0xFFFFFE70, 0xFFFFFF38, 0xFFFFFFB0, 0x0]
// { -10000, -5000, -3500, -2000, -1500, -1000, -700, -400, -200, -80, 0 }
// <LegoRR.exe @004abe30>
extern const sint32 (& c_SetGlobalVolumePrescaled_realVol)[11];

// <LegoRR.exe @005075b8>
extern Sound3D_Globs & sound3DGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Sound3D_GetNormalPlayFlag(flags, idx)		\
	((flags) & (Gods98::Sound3DFlags)((uint32)Gods98::Sound3DFlags::SOUND3D_PLAY_FLAG_NORMAL << (idx)))

#define Sound3D_SetNormalPlayFlag(flags, idx, on)   \
	if ((on)) (flags) |= (Gods98::Sound3DFlags)((uint32)Gods98::Sound3DFlags::SOUND3D_PLAY_FLAG_NORMAL << (idx)); \
	else      (flags) &= ~(Gods98::Sound3DFlags)((uint32)Gods98::Sound3DFlags::SOUND3D_PLAY_FLAG_NORMAL << (idx))

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// CUSTOM: Clamps the volume in millibels to the range [Sound3D_MinVolume(), Sound3D_MaxVolume()].
sint32 Sound3D_ClampVolume(sint32 mbVol);

/// CUSTOM: Clamps the volume as a real to the range [0,1].
real32 Sound3D_ClampVolumeReal(real32 fVol);

/// CUSTOM: Converts the volume from millibels to a real in the range [0,1].
real32 Sound3D_VolumeToReal(sint32 mbVol);

/// CUSTOM: Converts the volume to millibels from a real in the range [0,1].
sint32 Sound3D_VolumeFromReal(real32 fVol);

/// CUSTOM: Gets the actual volume to use by scaling mbVol with the global volume.
sint32 Sound3D_ScaleByGlobalVolume(sint32 mbVol);

/// CUSTOM: Applies a reduction for non-3D sounds and then calls Sound3D_ScaleByGlobalVolume.
sint32 Sound3D_GetFinalBufferVolume(Sound3D_SoundData* sound, uint32 voice);


// <LegoRR.exe @0047a900>
bool32 __cdecl Sound3D_Initialise(HWND hwndParent);

// <LegoRR.exe @0047aac0>
void __cdecl Sound3D_ShutDown(void);

// <LegoRR.exe @0047ab10>
bool32 __cdecl Sound3D_CheckVolumeLimits(sint32 vol);

// <LegoRR.exe @0047ab30>
sint32 __cdecl Sound3D_Load(const char* fName, bool32 stream, bool32 simultaneous, sint32 volume);

// <LegoRR.exe @0047ad90>
bool32 __cdecl Sound3D_GetFreeSoundIndex(OUT uint32* soundTableIndex);

// <LegoRR.exe @0047adc0>
bool32 __cdecl Sound3D_Remove(sint32 soundTableIndex);

// <LegoRR.exe @0047ae40>
void __cdecl Sound3D_MakeListener(IDirect3DRMFrame3* frame);

// <LegoRR.exe @0047ae60>
void __cdecl Sound3D_UpdateListener(void);

// <LegoRR.exe @0047ae80>
void __cdecl Sound3D_ListenerCallback(IDirect3DRMFrame3* obj, void* arg, real32 delta);

// <LegoRR.exe @0047afd0>
void __cdecl Sound3D_SetWorldPos(IDirectSound3DBuffer* sound3DBuff, const Vector3F* wPos, const Vector3F* vel);

// <LegoRR.exe @0047aff0>
bool32 __cdecl Sound3D_CheckAlreadyExists(IDirect3DRMFrame3* frame, IDirectSound3DBuffer* sound3DBuff);

// <LegoRR.exe @0047b030>
sint32 __cdecl Sound3D_Play2(Sound3DPlay play, IDirect3DRMFrame3* frame, sint32 soundTableIndex, bool32 loop, OPTIONAL const Vector3F* wPos);

// <LegoRR.exe @0047b2e0>
void __cdecl Sound3D_AddSoundRecord(IDirect3DRMFrame3* frame, IDirectSoundBuffer* soundBuff, IDirectSound3DBuffer* sound3DBuff);

// <LegoRR.exe @0047b310>
void __cdecl Sound3D_SetBufferVolume(sint32 handle, sint32 newvolume);

/// CUSTOM: Updates global volume scaling for the sound while maintaining the normal sound volume reduction.
void __cdecl Sound3D_UpdateBufferVolume(sint32 handle);

// <LegoRR.exe @0047b390>
sint32 __cdecl Sound3D_GetBufferVolume(sint32 handle);

/// CUSTOM: Sets the sound buffer volume as a real in the range [0,1].
inline void Sound3D_SetBufferVolumeReal(sint32 handle, real32 fVol) { Sound3D_SetBufferVolume(handle, Sound3D_VolumeFromReal(fVol)); }

/// CUSTOM: Gets the sound buffer volume as a real in the range [0,1].
inline real32 Sound3D_GetBufferVolumeReal(sint32 handle) { return Sound3D_VolumeToReal(Sound3D_GetBufferVolume(handle)); }


// <LegoRR.exe @0047b3b0>
IDirectSoundBuffer* __cdecl Sound3D_GetSoundBuffer(sint32 soundHandle);

// <LegoRR.exe @0047b3f0>
void __cdecl Sound3D_StopSound(sint32 soundHandle);

// <LegoRR.exe @0047b420>
void __cdecl Sound3D_StopAllSounds(void);


// <inlined>
/*__inline*/ void __cdecl Sound3D_RegisterUpdateFrame(IDirect3DRMFrame3* frame);


// <LegoRR.exe @0047b460>
void __cdecl Sound3D_AttachSound(IDirect3DRMFrame3* frame, IDirectSound3DBuffer* sound3DBuff);


// <inlined>
/*__inline*/ void __cdecl Sound3D_RemoveUpdateFrame(IDirect3DRMFrame3* frame);


// <LegoRR.exe @0047b4e0>
void __cdecl Sound3D_RemoveSound(IDirect3DRMFrame3* owner, IDirectSound3DBuffer* sound3DBuff);

// <LegoRR.exe @0047b560>
void __cdecl Sound3D_RemoveSoundRecord(IDirect3DRMFrame3* frame, IDirectSound3DBuffer* sound3DBuff);

// <LegoRR.exe @0047b5a0>
bool32 __cdecl Sound3D_RecurseRemoveSoundRecord(IDirect3DRMFrame3* owner, IDirectSound3DBuffer* sound3DBuff, Sound3D_SoundRecord* record);

// <LegoRR.exe @0047b5f0>
bool32 __cdecl Sound3D_RecurseRemoveSound(IDirect3DRMFrame3* owner, IDirectSound3DBuffer* sound3DBuff, Sound3D_SoundFrameRecord* record);


// <inlined>
/*__inline*/ void __cdecl Sound3D_UpdateFrames(void);


// <LegoRR.exe @0047b650>
void __cdecl Sound3D_Update(void);

// <LegoRR.exe @0047b6d0>
void __cdecl Sound3D_SoundCallback(IDirect3DRMFrame3* tFrame, void* arg, real32 delay);

// <LegoRR.exe @0047b760>
void __cdecl Sound3D_SetMinDistForAtten(real32 dist);

// <LegoRR.exe @0047b790>
void __cdecl Sound3D_SetMaxDist(real32 dist);

// <LegoRR.exe @0047b7c0>
void __cdecl Sound3D_SetGlobalVolume(sint32 vol);

// <LegoRR.exe @0047b7f0>
void __cdecl Sound3D_SetGlobalVolumePrescaled(sint32 vol);

// <LegoRR.exe @0047b810>
void __cdecl Sound3D_SetVolumeToDefault(void);


/// CUSTOM: Sets the global sound volume as a real in the range [0,1].
inline void Sound3D_SetGlobalVolumeReal(real32 fVol) { Sound3D_SetGlobalVolume(Sound3D_VolumeFromReal(fVol)); }

/// CUSTOM: Gets the global sound volume in millibels.
inline sint32 Sound3D_GetGlobalVolume() { return sound3DGlobs.volume; }

/// CUSTOM: Gets the global sound volume as a real in the range [0,1].
inline real32 Sound3D_GetGlobalVolumeReal() { return Sound3D_VolumeToReal(Sound3D_GetGlobalVolume()); }


// <LegoRR.exe @0047b840>
bool32 __cdecl Sound3D_LoadSample(OUT Sound3D_SoundData* sound, const char* fName, bool32 simultaneous);

// <LegoRR.exe @0047b980>
bool32 __cdecl Sound3D_CreateSoundBuffer(Sound3D_SoundData* sound, bool32 simultaneous);

// <LegoRR.exe @0047ba50>
bool32 __cdecl Sound3D_SendSoundToBuffer(Sound3D_SoundData* sound);

// <LegoRR.exe @0047bba0>
real32 __cdecl Sound3D_GetSamplePlayTime(sint32 handle);

// <LegoRR.exe @0047bc30>
bool32 __cdecl Sound3D_Stream_Play(const char* fName, bool32 loop, sint32 volume);

// <LegoRR.exe @0047bce0>
bool32 __cdecl Sound3D_Stream_Stop(bool32 looping);

// <LegoRR.exe @0047bd60>
bool32 __cdecl Sound3D_Stream_BufferSetup(const char* waveFName, bool32 loop, sint32 volume);

// <LegoRR.exe @0047bef0>
bool32 __cdecl Sound3D_Stream_FillDataBuffer(bool32 looping);

// <LegoRR.exe @0047c070>
void __cdecl Sound3D_Stream_CheckPosition(bool32 looping);

// <LegoRR.exe @0047c380>
bool32 __cdecl Sound3D_D3DVectorEqual(const Vector3F* a, const Vector3F* b);

// <LegoRR.exe @0047c3c0>
real32 __cdecl Sound3D_SetRollOffFactor(real32 rollOff);

// <LegoRR.exe @0047c420>
sint32 __cdecl Sound3D_MinVolume(void);

// return 0;
// <shared>
sint32 __cdecl Sound3D_MaxVolume(void); // return 0;


// <inlined>
__inline IDirectSound* lpDSnd(void) { return sound3DGlobs.lpDSnd; }

// <inlined>
__inline IDirectSoundBuffer* lpDSBuff(void) { return sound3DGlobs.lpDSBuff; }

// <inlined>
__inline IDirectSound3DListener* lp3DListenerInfo(void) { return sound3DGlobs.lp3DListenerInfo; }

// <inlined>
__inline IDirectSoundBuffer* lpDSStreamBuff(bool32 looping) { return looping?sound3DGlobs.lpDSLoopStreamBuff:sound3DGlobs.lpDSStreamBuff; }


#define Sound3D_Initialised()								sound3DGlobs.intialised

#define Sound3D_PlayOnFrame(frame, sound, loop)				Sound3D_Play2(Gods98::Sound3DPlay::OnFrame, (frame), (sound), (loop), nullptr)
#define Sound3D_PlayOnCont(cont, sound, loop)				Sound3D_Play2(Gods98::Sound3DPlay::OnFrame, (cont)->masterFrame, (sound), (loop), nullptr)
#define Sound3D_PlayOnPos(sound, loop, wPos)				Sound3D_Play2(Gods98::Sound3DPlay::OnPos, nullptr, (sound), (loop), (wPos))
#define Sound3D_PlayNormal(sound, loop)						Sound3D_Play2(Gods98::Sound3DPlay::Normal, nullptr, (sound), (loop), nullptr)
#define Sound3D_MakeContainerListener(cont)					Sound3D_MakeListener(Gods98::Container_GetMasterFrame((cont)))
#define SOUND3D_VOLUME_MAX		Gods98::Sound3D_MaxVolume()
#define SOUND3D_VOLUME_MIN		Gods98::Sound3D_MinVolume()

#pragma endregion

}
