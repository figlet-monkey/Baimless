#pragma once
#include <Psapi.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <vector>
#include "..\SDK\IVEngineClient.h"
#pragma warning( disable : 4244 )
#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
static HANDLE _out = NULL, _old_out = NULL;
static HANDLE _err = NULL, _old_err = NULL;
static HANDLE _in = NULL, _old_in = NULL;
#define INRANGE(x,a,b)    (x >= a && x <= b)
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))
class Utils
{
public:
    static BSTR ConvertStringToBSTR_UTF8(const char* szBuff)
    {
        if (!szBuff) return NULL;
        DWORD cwch;
        BSTR wsOut(NULL);

        if (cwch = MultiByteToWideChar(CP_UTF8, 0, szBuff, -1, NULL, 0))
        {
            cwch--;
            wsOut = SysAllocStringLen(NULL, cwch);
            if (wsOut)
            {
                if (!MultiByteToWideChar(CP_UTF8, 0, szBuff, -1, wsOut, cwch))
                {
                    if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError())
                        return wsOut;

                    SysFreeString(wsOut);
                    wsOut = NULL;
                }
            }

        };

        return wsOut;
    }
    typedef void(__cdecl* MsgFn)(const char* msg, va_list);
    static void MsgX(const char* msg, ...)
    {
        if (msg == nullptr)
            return; //If no string was passed, or it was null then don't do anything
        static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); 	char buffer[989];
        va_list list;
        va_start(list, msg);
        vsprintf(buffer, msg, list);
        va_end(list);
        fn(buffer, list); //Calls the function, we got the address above.
    }
    static std::uintptr_t Scan(const std::string& image_name, const std::string& signature)
    {
        auto image = GetModuleHandleA(image_name.c_str());

        if (!image)
        {
#ifdef DEBUG
            Win32::Error("GetModuleHandleA failed (Horizon::Memory::%s)", __FUNCTION__);
#endif // DEBUG
            return 0u;
        }

        auto image_base = (std::uintptr_t)(image);
        auto image_dos_hdr = (IMAGE_DOS_HEADER*)(image_base);

        if (image_dos_hdr->e_magic != IMAGE_DOS_SIGNATURE)
        {
#ifdef DEBUG
            Win32::Error("IMAGE_DOS_HEADER::e_magic is invalid (Horizon::Memory::%s)", __FUNCTION__);
#endif // DEBUG
            return 0u;
        }

        auto image_nt_hdrs = (IMAGE_NT_HEADERS*)(image_base + image_dos_hdr->e_lfanew);

        if (image_nt_hdrs->Signature != IMAGE_NT_SIGNATURE)
        {
#ifdef DEBUG
            Win32::Error("IMAGE_NT_HEADERS::Signature is invalid (Horizon::Memory::%s)", __FUNCTION__);
#endif // DEBUG
            return 0u;
        }

        auto scan_begin = (std::uint8_t*)(image_base);
        auto scan_end = (std::uint8_t*)(image_base + image_nt_hdrs->OptionalHeader.SizeOfImage);

        std::uint8_t* scan_result = nullptr;
        std::uint8_t* scan_data = (std::uint8_t*)(signature.c_str());

        for (auto current = scan_begin; current < scan_end; current++)
        {
            if (*(std::uint8_t*)scan_data == '\?' || *current == GET_BYTE(scan_data))
            {
                if (!scan_result)
                    scan_result = current;

                if (!scan_data[2])
                    return (std::uintptr_t)(scan_result);

                scan_data += (*(std::uint16_t*)scan_data == '\?\?' || *(std::uint8_t*)scan_data != '\?') ? 3 : 2;

                if (!*scan_data)
                    return (std::uintptr_t)(scan_result);
            }
            else if (scan_result)
            {
                current = scan_result;
                scan_data = (std::uint8_t*)(signature.c_str());
                scan_result = nullptr;
            }
        }

#ifdef DEBUG
        Win32::Warning("Signature '%s' in module '%s' not found (Horizon::Memory::%s)", signature.c_str(), image_name.c_str(), __FUNCTION__);
#endif // DEBUG

        return 0u;
    }
    static std::uintptr_t findPattern(const wchar_t* module, const char* pattern, size_t offset = 0)
    {
        static auto id = 0;
        ++id;

        if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(module), &moduleInfo, sizeof(moduleInfo))) {
            auto start = static_cast<const char*>(moduleInfo.lpBaseOfDll);
            const auto end = start + moduleInfo.SizeOfImage;

            auto first = start;
            auto second = pattern;

            while (first < end && *second) {
                if (*first == *second || *second == '?') {
                    ++first;
                    ++second;
                }
                else {
                    first = ++start;
                    second = pattern;
                }
            }

            if (!*second)
                return reinterpret_cast<std::uintptr_t>(const_cast<char*>(start) + offset);
        }
        MessageBoxA(NULL, ("Failed to find pattern #" + std::to_string(id) + '!').c_str(), "Osiris", MB_OK | MB_ICONWARNING);
        return 0;
    }
    static std::uint8_t* PatternScan(void* module, const char* signature)
    {
        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector<int>{};
            auto start = const_cast<char*>(pattern);
            auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                }
                else {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        auto dosHeader = (PIMAGE_DOS_HEADER)module;
        auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

        auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = pattern_to_byte(signature);
        auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

        auto s = patternBytes.size();
        auto d = patternBytes.data();

        for (auto i = 0ul; i < sizeOfImage - s; ++i) {
            bool found = true;
            for (auto j = 0ul; j < s; ++j) {
                if (scanBytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return &scanBytes[i];
            }
        }
        return nullptr;
    }
    static std::string wstring_to_string(const std::wstring& ws)
    {
        const std::string s(ws.begin(), ws.end());
        return s;
    }
    static std::wstring string_to_wstring(const std::string& s)
    {
        const std::wstring ws(s.begin(), s.end());
        return ws;
    }
    static std::string hardware_id()
    {
        HW_PROFILE_INFO hwProfileInfo;
        if (GetCurrentHwProfile(&hwProfileInfo)) {
            std::string ws(hwProfileInfo.szHwProfileGuid); 
            //std::string str(ws.begin(), ws.end());
            return ws;
        }
    }
    static void AttachConsole()
    {
        _old_out = GetStdHandle(STD_OUTPUT_HANDLE);
        _old_err = GetStdHandle(STD_ERROR_HANDLE);
        _old_in = GetStdHandle(STD_INPUT_HANDLE);

        ::AllocConsole() && ::AttachConsole(GetCurrentProcessId());

        _out = GetStdHandle(STD_OUTPUT_HANDLE);
        _err = GetStdHandle(STD_ERROR_HANDLE);
        _in = GetStdHandle(STD_INPUT_HANDLE);

        SetConsoleMode(_out,
            ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

        SetConsoleMode(_in,
            ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
            ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
    }

    /*
     * @brief Detach console
     *
     * Detach and destroy the attached console
     */
    static void DetachConsole()
    {
        if (_out && _err && _in) {
            FreeConsole();

            if (_old_out)
                SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
            if (_old_err)
                SetStdHandle(STD_ERROR_HANDLE, _old_err);
            if (_old_in)
                SetStdHandle(STD_INPUT_HANDLE, _old_in);
        }
    }

    /*
     * @brief Print to console
     *
     * Replacement to printf that works with the newly created console
     */
    static bool ConsolePrint(const char* fmt, ...)
    {
        if (!_out)
            return false;

        char buf[1024];
        va_list va;

        va_start(va, fmt);
        _vsnprintf_s(buf, 1024, fmt, va);
        va_end(va);

        return !!WriteConsoleA(_out, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
    }

    /*
     * @brief Blocks execution until a key is pressed on the console window
     *
     */
    static char ConsoleReadKey()
    {
        if (!_in)
            return false;

        auto key = char{ 0 };
        auto keysread = DWORD{ 0 };

        ReadConsoleA(_in, &key, 1, &keysread, nullptr);

        return key;
    }

    template<unsigned int IIdx, typename TRet, typename ... TArgs>
    static auto CallVFunc(void* thisptr, TArgs ... argList) -> TRet
    {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

	template <typename t>
	static t GetVFunc(void* class_pointer, size_t index) {
		return (*(t**)class_pointer)[index];
	}


	static DWORD FindSig(DWORD dwAddress, DWORD dwLength, const char* szPattern)
	{
		if (!dwAddress || !dwLength || !szPattern)
			return 0;

		const char* pat = szPattern;
		DWORD firstMatch = NULL;

		for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else pat += 2;
			}
			else
			{
				pat = szPattern;
				firstMatch = 0;
			}
		}

		return 0;
	}
	static DWORD FindSignature(const char* szModuleName, const char* PatternName, char* szPattern)
	{
		HMODULE hModule = GetModuleHandleA(szModuleName);
		PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
		PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hModule) + pDOSHeader->e_lfanew);

		DWORD ret = FindSig(((DWORD)hModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);;

		return ret;
	}
    static uintptr_t FindSignature(const char* szModule, const char* szSignature)
    {
        const char* pat = szSignature;
        DWORD firstMatch = 0;
        DWORD rangeStart = (DWORD)GetModuleHandleA(szModule);
        MODULEINFO miModInfo;
        GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
        DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
        for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
        {
            if (!*pat)
                return firstMatch;

            if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GET_BYTE(pat))
            {
                if (!firstMatch)
                    firstMatch = pCur;

                if (!pat[2])
                    return firstMatch;

                if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
                    pat += 3;

                else
                    pat += 2;
            }
            else
            {
                pat = szSignature;
                firstMatch = 0;
            }
        }
        return NULL;
    }

	static DWORD FindPattern(const char* module_name, const BYTE* mask, const char* mask_string)
	{
		/// Get module address
		const unsigned int module_address = reinterpret_cast<unsigned int>(GetModuleHandle(module_name));

		/// Get module information to the size
		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(module_address), &module_info, sizeof(MODULEINFO));

		auto IsCorrectMask = [](const unsigned char* data, const unsigned char* mask, const char* mask_string) -> bool
		{
			for (; *mask_string; ++mask_string, ++mask, ++data)
				if (*mask_string == 'x' && *mask != *data)
					return false;

			return (*mask_string) == 0;
		};

		/// Iterate until we find a matching mask
		for (unsigned int c = 0; c < module_info.SizeOfImage; c += 1)
		{
			/// does it match?
			if (IsCorrectMask(reinterpret_cast<unsigned char*>(module_address + c), mask, mask_string))
				return (module_address + c);
		}

		return 0;
	}

    /**
    *   GetCurrentSystemTime - Gets actual system time
    *   @timeInfo: Reference to your own tm variable, gets modified.
    */
    static void GetCurrentSystemTime(tm& timeInfo)
    {
        const std::chrono::system_clock::time_point systemNow = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(systemNow);
        localtime_s(&timeInfo, &now_c); // using localtime_s as std::localtime is not thread-safe.
    };


    /**
    *   Log - Logs specified message to debug console if in debug mode.
    *   Format: [HH:MM:SS] str
    *   @str: Debug message to be shown.
    */
    static void Log(const std::string& str, ...)
    {
#ifdef _DEBUG
        tm timeInfo { };
        Utils::GetCurrentSystemTime(timeInfo);

        std::stringstream ssTime; // Temp stringstream to keep things clean
        ssTime << "[" << std::put_time(&timeInfo, "%T") << "] " << str << std::endl;

        std::cout << ssTime.str();
#endif // _DEBUG
    };



    /**
    *   Log(HRESULT) - Sets up an error message when you pass HRESULT
    *   Message contains error code only, no specific information
    */
    static void Log(const HRESULT hr)
    {
#ifdef _DEBUG
        std::stringstream strFormatted;
        strFormatted << "Operation failed, error code = 0x" << std::hex << hr;

        Utils::Log(strFormatted.str());
#endif // _DEBUG
    };



    /**
    *   WorldToScreen - Converts game coordinates to screen
    *   @origin: 3D coordinates to be converted
    *   @screen: Viewport coordinates to which we will convert
    */
    static bool screen_transform(const Vector& in, Vector& out)
    {
        static auto& w2sMatrix = g_pEngine->WorldToScreenMatrix();

        out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
        out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
        out.z = 0.0f;

        float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

        if (w < 0.001f) {
            out.x *= 100000;
            out.y *= 100000;
            return false;
        }

        out.x /= w;
        out.y /= w;

        return true;
    }
    static bool WorldToScreen(const Vector& in, Vector& out)
    {
        if (screen_transform(in, out)) {
            int w, h;
            g_pEngine->GetScreenSize(w, h);

            out.x = (w / 2.0f) + (out.x * w) / 2.0f;
            out.y = (h / 2.0f) - (out.y * h) / 2.0f;

            return true;
        }
        return false;
    }
    static bool WorldToScreen(const Vector &origin, Vector2D &screen)
    {
        const auto screenTransform = [&origin, &screen]() -> bool
        {
            static std::uintptr_t pViewMatrix;
            if (!pViewMatrix)
            {
                pViewMatrix = static_cast<std::uintptr_t>(Utils::FindSignature("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
                pViewMatrix += 3;
                pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
                pViewMatrix += 176;
            }

            const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
            screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
            screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];

            float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

            if (w < 0.001f)
            {
                screen.x *= 100000;
                screen.y *= 100000;
                return true;
            }

            float invw = 1.f / w;
            screen.x *= invw;
            screen.y *= invw;

            return false;
        };

        if (!screenTransform())
        {
            int iScreenWidth, iScreenHeight;
            g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

            screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
            screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

            return true;
        }
        return false;
    }
};
