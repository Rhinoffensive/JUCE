/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"

#include "../utility/juce_CheckSettingMacros.h"

#if JucePlugin_Build_RTAS

 #include "juce_RTAS_DigiCode_Header.h"

 /*
    This file is used to include and build the required digidesign CPP files without your project
    needing to reference the files directly. Because these files will be found via your include path,
    this means that the project doesn't have to change to cope with people's SDKs being in different
    locations.

    Important note on Windows: In your project settings for the three juce_RTAS_DigiCode.cpp files and
    the juce_RTAS_Wrapper.cpp file, you need to set the calling convention to "__stdcall".
    If you don't do this, you'll get some unresolved externals and will spend a long time wondering what's
    going on... All the other files in your project can be set to use the normal __cdecl convention.

    If you get an error building the includes statements below, check your paths - there's a full
    list of the necessary Digidesign paths in juce_RTAS_Wrapper.cpp
 */

 #if WINDOWS_VERSION
  #undef _UNICODE
  #undef UNICODE

  #define DllMain DllMainRTAS
  #include <DLLMain.cpp>
  #undef DllMain
  #include <DefaultSwap.cpp>

 #else
  #include <PlugInInitialize.cpp>
  #include <Dispatcher.cpp>
 #endif

 #else

 #if _MSC_VER
  short __stdcall NewPlugIn (void*)                          { return 0; }
  short __stdcall _PI_GetRoutineDescriptor (long, void*)     { return 0; }
 #endif

#endif


//==============================================================================
#if _MSC_VER
 #if ! JucePlugin_Build_RTAS
  #include <windows.h>
 #endif

 #if JucePlugin_Build_VST
  extern void __stdcall DllMainVST (HINSTANCE, DWORD);
 #endif

 #if JucePlugin_Build_AAX
  extern void __stdcall DllMainAAX (HINSTANCE, DWORD);
 #endif

  extern "C" BOOL WINAPI DllMain (HINSTANCE instance, DWORD reason, LPVOID reserved)
  {
      (void) reserved;

     #if JucePlugin_Build_VST
      DllMainVST (instance, reason);
     #endif

     #if JucePlugin_Build_AAX
      DllMainAAX (instance, reason);
     #endif

     #if JucePlugin_Build_RTAS
      if (GetModuleHandleA ("DAE.DLL") != 0)
         return DllMainRTAS (instance, reason, reserved);
     #endif

      return TRUE;
  }
#endif
