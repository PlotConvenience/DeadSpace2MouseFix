#pragma once
#include <unknwn.h>

#ifdef DLL_EXPORT
#define DINPUT8_API __declspec(dllexport)
#else
#define DINPUT8_API __declspec(dllimport)
#endif

typedef HRESULT(*DirectInput8Create_t)(
   HINSTANCE hinst,
   DWORD dwVersion,
   REFIID riidltf,
   LPVOID * ppvOut,
   LPUNKNOWN punkOuter);

extern DirectInput8Create_t OriginalDirectInput8Create;
extern HMODULE DINPUT8DLL;

extern "C"
{
   DINPUT8_API HRESULT DirectInput8Create(
      HINSTANCE hinst,
      DWORD dwVersion,
      REFIID riidltf,
      LPVOID * ppvOut,
      LPUNKNOWN punkOuter
   );
}