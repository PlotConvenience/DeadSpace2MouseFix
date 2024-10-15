#include <cstdio>
#include <MinHook.h>
#include "MinimalDInput8Hook.h"

#pragma comment(lib, "libMinHook.x86.lib")

const uintptr_t GAME_BASE_ADDRESS = (uintptr_t)GetModuleHandleA(NULL);
//const DWORD GAME_BASE_ADDRESS = 0x00400000;

char * CreatePathToSystemDinput8DLL()
{
   char dinput8Path[MAX_PATH];

   GetSystemDirectoryA(dinput8Path, MAX_PATH);
   strcat_s(dinput8Path, "\\dinput8.dll");

   return dinput8Path;
}

//Loads the original dinput8.dll from the system directory.
void InitializeSystemDinput8DLL()
{
   char * systemDinput8Path = CreatePathToSystemDinput8DLL();

   DINPUT8DLL = LoadLibraryA(systemDinput8Path);
}

void InitializeOriginalDirectInput8Create(HMODULE dinputDLLModule)
{
   if(dinputDLLModule != NULL)
      OriginalDirectInput8Create = (DirectInput8Create_t)GetProcAddress(dinputDLLModule, "DirectInput8Create");

   if(OriginalDirectInput8Create != NULL)
      printf("OriginalDirectInput8Create initialized successfully.\n");
   else
      printf("Failed to initialize OriginalDirectInput8Create.\n");
}

void CreateConsole()
{
   AllocConsole();

   FILE * file;

   freopen_s(&file, "CONOUT$", "w", stdout);
}

void InitializeMinHook()
{
   MH_STATUS status = MH_Initialize();

   if(status == MH_OK)
      printf("MinHook initialized successfully.\n");
   else
      printf("MinHook failed to initialize with error: %s\n", MH_StatusToString(status));
}

//typedef void (__thiscall * myFunction)(void *, int, int, int, int,  int, unsigned int);
//myFunction myFunctionOriginal = nullptr;
//myFunction myFunctionAddress = reinterpret_cast<myFunction>(GAME_BASE_ADDRESS + 0x00830290);
//
//void __fastcall myFunctionWrapper(void *This, void * _EDX, int p1, int p2, int p3, int p4, int p5, unsigned int p6)
//{
//   printf("myFunction called.\n");
//   myFunctionOriginal(This, p1, p2, p3, p4, p5, p6);
//   printf("Original myFunction returned.\n");
//}
//
//void HookFunctions1()
//{
//   MH_STATUS status = MH_CreateHook(reinterpret_cast<void**>(myFunctionAddress), &myFunctionWrapper, reinterpret_cast<void**>(&myFunctionOriginal));
//
//   if(status != MH_OK)
//      printf("Failed to create hook: %s\n", MH_StatusToString(status));
//   else
//      printf("Hook created successfully.\n");
//
//   status = MH_EnableHook(reinterpret_cast<void**>(myFunctionAddress));
//
//   if(status != MH_OK)
//      printf("Failed to enable hook: %s\n", MH_StatusToString(status));
//   else
//      printf("Hook enabled successfully.\n");
//}

//typedef unsigned long long (__cdecl * myFunction)(int p1, int p2);
//myFunction myFunctionOriginal = nullptr;
//myFunction myFunctionAddress = reinterpret_cast<myFunction>(GAME_BASE_ADDRESS + 0x0082ca20);
//
//unsigned long long __cdecl myFunctionWrapper(int p1, int p2)
//{
//   printf("myFunction called.\n");
//   return myFunctionOriginal(p1, p2);
//}
//
//void HookFunctions2()
//{
//   MH_STATUS status = MH_CreateHook(reinterpret_cast<void**>(myFunctionAddress), &myFunctionWrapper, reinterpret_cast<void**>(&myFunctionOriginal));
//
//   if(status != MH_OK)
//      printf("Failed to create hook: %s\n", MH_StatusToString(status));
//   else
//      printf("Hook created successfully.\n");
//
//   status = MH_EnableHook(reinterpret_cast<void**>(myFunctionAddress));
//
//   if(status != MH_OK)
//      printf("Failed to enable hook: %s\n", MH_StatusToString(status));
//   else
//      printf("Hook enabled successfully.\n");
//}

//typedef void (__thiscall * myFunction)(void *This, int p1, int p2, char p3, char p4, char p5, int p6, int p7);
//myFunction myFunctionOriginal = nullptr;
//myFunction myFunctionAddress = (myFunction)0x0044e610;
//
//void __fastcall myFunctionWrapper(void *This, int p1, int p2, char p3, char p4, char p5, int p6, int p7)
//{
//   printf("myFunction called.\n");
//   myFunctionOriginal(This, p1, p2, p3, p4, p5, p6, p7);
//}
//
//void HookFunctions2()
//{
//   MH_STATUS status = MH_CreateHook((LPVOID)myFunctionAddress, &myFunctionWrapper, (LPVOID*)&myFunctionOriginal);
//
//   if(status != MH_OK)
//      printf("Failed to create hook: %s\n", MH_StatusToString(status));
//   else
//      printf("Hook created successfully.\n");
//
//   status = MH_EnableHook((LPVOID)myFunctionAddress);
//
//   if(status != MH_OK)
//      printf("Failed to enable hook: %s\n", MH_StatusToString(status));
//   else
//      printf("Hook enabled successfully.\n");
//}

typedef int (__thiscall * WeaponFunction)(int, unsigned int);
WeaponFunction weaponFunctionOriginal = nullptr;
WeaponFunction weaponFunctionAddress = (WeaponFunction)(GAME_BASE_ADDRESS + 0x9bae30);

int WeaponFunctionWrapper(int p1, unsigned int p2)
{
   printf("WeaponFunctionWrapper called.\n");

   return weaponFunctionOriginal(p1, p2);
}

void HookWeaponFunction()
{
   MH_STATUS status = MH_CreateHook(weaponFunctionAddress, WeaponFunctionWrapper, (void **)(&weaponFunctionOriginal));

   if(status == MH_OK)
      printf("Hook created successfully.\n");
   else
      printf("Failed to create hook: %s\n", MH_StatusToString(status));

   status = MH_EnableHook(MH_ALL_HOOKS);

   if(status == MH_OK)
      printf("Hook enabled successfully.\n");
   else
      printf("Failed to enable hook: %s\n", MH_StatusToString(status));
}

void Init()
{
   CreateConsole();
   InitializeSystemDinput8DLL();
   InitializeOriginalDirectInput8Create(DINPUT8DLL);

   //HMODULE module = GetModuleHandleA("deadspace2.exe");
   //const uintptr_t GAME_BASE_ADDRESS = (uintptr_t)module;

   //printf("Base address: %#x\n", GAME_BASE_ADDRESS);
   //
   //int address = 0x1a86770;
   //int offset = 20;
   //int start = address - offset;
   //int end = address + offset;
   //for(int current = start; current < end; current++)
   //   printf("address %#x = %#x\n", current, *((BYTE *)(current)));

   InitializeMinHook();
   //HookFunctions1();
   HookWeaponFunction();
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD  ReasonForCall, LPVOID Reserved)
{
   switch(ReasonForCall)
   {
   case DLL_PROCESS_ATTACH:
      Init();
      break;
   case DLL_THREAD_ATTACH:
      break;
   case DLL_THREAD_DETACH:
      break;
   case DLL_PROCESS_DETACH:
      if(DINPUT8DLL)
         FreeLibrary(DINPUT8DLL);

      MH_Uninitialize();
      break;
   }
   return TRUE;
}