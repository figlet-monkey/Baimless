#pragma once

#if !(defined _M_IX86) && !(defined _M_X64)
#error MinHook supports only x86 and x64 systems.
#endif

#include <windows.h>

// MinHook Error Codes.
typedef enum MH_STATUS
{
	// Unknown error. Should not be returned.
	MH_UNKNOWN = -1,

	// Successful.
	MH_OK = 0,

	// MinHook is already initialized.
	MH_ERROR_ALREADY_INITIALIZED,

	// MinHook is not initialized yet, or already uninitialized.
	MH_ERROR_NOT_INITIALIZED,

	// The hook for the specified target function is already created.
	MH_ERROR_ALREADY_CREATED,

	// The hook for the specified target function is not created yet.
	MH_ERROR_NOT_CREATED,

	// The hook for the specified target function is already enabled.
	MH_ERROR_ENABLED,

	// The hook for the specified target function is not enabled yet, or already
	// disabled.
	MH_ERROR_DISABLED,

	// The specified pointer is invalid. It points the address of non-allocated
	// and/or non-executable region.
	MH_ERROR_NOT_EXECUTABLE,

	// The specified target function cannot be hooked.
	MH_ERROR_UNSUPPORTED_FUNCTION,

	// Failed to allocate memory.
	MH_ERROR_MEMORY_ALLOC,

	// Failed to change the memory protection.
	MH_ERROR_MEMORY_PROTECT,

	// The specified module is not loaded.
	MH_ERROR_MODULE_NOT_FOUND,

	// The specified function is not found.
	MH_ERROR_FUNCTION_NOT_FOUND
}
MH_STATUS;

// Can be passed as a parameter to MH_EnableHook, MH_DisableHook,
// MH_QueueEnableHook or MH_QueueDisableHook.
#define MH_ALL_HOOKS NULL

#ifdef __cplusplus
extern "C" {
#endif

	// Initialize the MinHook library. You must call this function EXACTLY ONCE
	// at the beginning of your program.
	MH_STATUS WINAPI MH_Initialize(VOID);

	// Uninitialize the MinHook library. You must call this function EXACTLY
	// ONCE at the end of your program.
	MH_STATUS WINAPI MH_Uninitialize(VOID);

	// Creates a Hook for the specified target function, in disabled state.
	// Parameters:
	//   pTarget    [in]  A pointer to the target function, which will be
	//                    overridden by the detour function.
	//   pDetour    [in]  A pointer to the detour function, which will override
	//                    the target function.
	//   ppOriginal [out] A pointer to the trampoline function, which will be
	//                    used to call the original target function.
	//                    This parameter can be NULL.
	MH_STATUS WINAPI MH_CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal);

	// Creates a Hook for the specified API function, in disabled state.
	// Parameters:
	//   pszModule  [in]  A pointer to the loaded module name which contains the
	//                    target function.
	//   pszTarget  [in]  A pointer to the target function name, which will be
	//                    overridden by the detour function.
	//   pDetour    [in]  A pointer to the detour function, which will override
	//                    the target function.
	//   ppOriginal [out] A pointer to the trampoline function, which will be
	//                    used to call the original target function.
	//                    This parameter can be NULL.
	MH_STATUS WINAPI MH_CreateHookApi(
		LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal);

	// Creates a Hook for the specified API function, in disabled state.
	// Parameters:
	//   pszModule  [in]  A pointer to the loaded module name which contains the
	//                    target function.
	//   pszTarget  [in]  A pointer to the target function name, which will be
	//                    overridden by the detour function.
	//   pDetour    [in]  A pointer to the detour function, which will override
	//                    the target function.
	//   ppOriginal [out] A pointer to the trampoline function, which will be
	//                    used to call the original target function.
	//                    This parameter can be NULL.
	//   ppTarget   [out] A pointer to the target function, which will be used
	//                    with other functions.
	//                    This parameter can be NULL.
	MH_STATUS WINAPI MH_CreateHookApiEx(
		LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal, LPVOID *ppTarget);

	// Removes an already created hook.
	// Parameters:
	//   pTarget [in] A pointer to the target function.
	MH_STATUS WINAPI MH_RemoveHook(LPVOID pTarget);

	// Enables an already created hook.
	// Parameters:
	//   pTarget [in] A pointer to the target function.
	//                If this parameter is MH_ALL_HOOKS, all created hooks are
	//                enabled in one go.
	MH_STATUS WINAPI MH_EnableHook(LPVOID pTarget);

	// Disables an already created hook.
	// Parameters:
	//   pTarget [in] A pointer to the target function.
	//                If this parameter is MH_ALL_HOOKS, all created hooks are
	//                disabled in one go.
	MH_STATUS WINAPI MH_DisableHook(LPVOID pTarget);

	// Queues to enable an already created hook.
	// Parameters:
	//   pTarget [in] A pointer to the target function.
	//                If this parameter is MH_ALL_HOOKS, all created hooks are
	//                queued to be enabled.
	MH_STATUS WINAPI MH_QueueEnableHook(LPVOID pTarget);

	// Queues to disable an already created hook.
	// Parameters:
	//   pTarget [in] A pointer to the target function.
	//                If this parameter is MH_ALL_HOOKS, all created hooks are
	//                queued to be disabled.
	MH_STATUS WINAPI MH_QueueDisableHook(LPVOID pTarget);

	// Applies all queued changes in one go.
	MH_STATUS WINAPI MH_ApplyQueued(VOID);

	// Translates the MH_STATUS to its name as a string.
	const char * WINAPI MH_StatusToString(MH_STATUS status);

#ifdef __cplusplus
}
#endif

