#include "MinimalDInput8Hook.h"
#include <cstdio>

DirectInput8Create_t OriginalDirectInput8Create = nullptr;
HMODULE DINPUT8DLL = nullptr;

DINPUT8_API HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter)
{
   if(OriginalDirectInput8Create)
   {
      printf("DirectInput8Create called successfully.\n");

      return OriginalDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
   }

   printf("Failed to call DirectInput8Create.\n");

   return S_FALSE;
}