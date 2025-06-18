# Sound System Overview

This guide lists each source file involved in audio playback along with its primary functions. Citations reference the original code for context.

## WIN32LIB/AUDIO/SOUND.H
- Defines `MAX_SFX` and `SECONDARY_BUFFER_SIZE` used to size DirectSound buffers【F:WIN32LIB/AUDIO/SOUND.H†L37-L52】.

## WIN32LIB/AUDIO/SOUNDINT.H
- Declares the `SCompressType` enum and the `SampleTrackerType` structure that holds per-sample state like buffer pointers and streaming fields【F:WIN32LIB/AUDIO/SOUNDINT.H†L230-L259】.
- `LockedDataType` collects global audio settings and the array of sample trackers【F:WIN32LIB/AUDIO/SOUNDINT.H†L259-L272】.
- Prototypes for core routines such as `Sample_Copy`, `maintenance_callback` and `DigiCallback` appear near the end of the file【F:WIN32LIB/AUDIO/SOUNDINT.H†L266-L277】.

## WIN32LIB/AUDIO/SOUNDLCK.CPP
- `Init_Locked_Data` seeds the global audio structures, locks callback functions and the assembly decompressors in memory【F:WIN32LIB/AUDIO/SOUNDLCK.CPP†L68-L132】.

## WIN32LIB/AUDIO/SOUNDIO.CPP
- Houses most DirectSound logic. `Audio_Init` creates the primary buffer and sets its format before playback begins【F:WIN32LIB/AUDIO/SOUNDIO.CPP†L980-L1060】.
- `Sound_Thread` repeatedly calls `maintenance_callback` to keep secondary buffers full【F:WIN32LIB/AUDIO/SOUNDIO.CPP†L908-L923】.
- Functions like `Play_Sample_Handle` and `Stop_Sample` manage streaming samples and cleanup【F:WIN32LIB/AUDIO/SOUNDIO.CPP†L1620-L1942】【F:WIN32LIB/AUDIO/SOUNDIO.CPP†L1283-L1338】.
- `Attempt_Audio_Restore` and `Convert_HMI_To_Direct_Sound_Volume` handle focus loss and linear-to-log volume conversion【F:WIN32LIB/AUDIO/SOUNDIO.CPP†L1528-L1602】.

## WIN32LIB/AUDIO/SOUNDINT.CPP
- `Sample_Copy` converts compressed audio blocks into raw PCM data used by DirectSound【F:WIN32LIB/AUDIO/SOUNDINT.CPP†L140-L192】.
- `maintenance_callback` services each `SampleTrackerType` by locking buffers and feeding audio to the hardware【F:WIN32LIB/AUDIO/SOUNDINT.CPP†L260-L330】.

## Assembly Modules
- **`AUDUNCMP.ASM`** provides `Decompress_Frame` for Westwood ADPCM decoding【F:WIN32LIB/AUDIO/AUDUNCMP.ASM†L60-L80】.
- **`OLSOSDEC.ASM`** supplies `sosCODECDecompressData` for HMI SOS streams【F:WIN32LIB/AUDIO/OLSOSDEC.ASM†L186-L208】.
- **`SOSCODEC.ASM`** contains an optimized version of the same routine used during debugging【F:WIN32LIB/SRCDEBUG/SOSCODEC.ASM†L178-L208】.
  The new `src/audio_decompress.c` module wraps these assembly routines when
  `ENABLE_ASM` is defined. With assembly disabled it implements the Westwood
  ADPCM and IMA-ADPCM algorithms described in `aud3.txt` directly in C.

## Miniaudio Backend
The legacy HMI timer callbacks were replaced by a portable implementation in `src/miniaudio.c`.  This module drives audio playback and provides simple timer hooks for the game when DirectSound is unavailable.

## CODE/AUDIO.CPP
- Game-facing helpers such as `Sound_Effect` play positional effects with panning and volume scaling【F:CODE/AUDIO.CPP†L326-L469】.
- EVA speech is handled by `Speak`, `Speak_AI`, `Stop_Speaking` and `Is_Speaking` to queue and play voice lines【F:CODE/AUDIO.CPP†L622-L763】.

## CODE/SOUNDDLG.H and CODE/SOUNDDLG.CPP
- Implements the sound options dialog. `SoundControlsClass::Process` draws sliders for music and effect volumes【F:CODE/SOUNDDLG.CPP†L160-L176】.
- `MusicListClass::Draw_Entry` renders track names in the list box【F:CODE/SOUNDDLG.CPP†L415-L435】.

## VQ/VQA32/AUDIO.CPP
- Used during video playback. `VQA_OpenAudio` opens the HMI driver and sets the timer callback【F:VQ/VQA32/AUDIO.CPP†L300-L329】.
- `AudioCallback` streams each frame of audio as the video plays【F:VQ/VQA32/AUDIO.CPP†L952-L975】.

## Public Sound API
- `VQ/INCLUDE/WWLIB32/AUDIO.H` exposes entry points like `Play_Sample` and volume control wrappers for use throughout the game code【F:VQ/INCLUDE/WWLIB32/AUDIO.H†L134-L143】.

