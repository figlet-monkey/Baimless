#include "MinHook.h"
template<typename T>
class cDetour
{
public:
	explicit cDetour<T>(T target, T detour) : m_target(target), m_detour(detour)
	{
		MH_CreateHook(m_target, m_detour, reinterpret_cast<void**>(&m_trampoline));
	}
	~cDetour()
	{
		MH_DisableHook(m_target);
	}
	T GetTrampoline() const
	{
		return static_cast<T>(m_trampoline);
	}
	bool IsApplied() const
	{
		return m_isEnabled;
	}
	void Apply()
	{
		if (!m_isEnabled)
		{
			m_isEnabled = MH_EnableHook(m_target) == MH_OK;
			if (m_isEnabled)
				memcpy(m_hookBuffer, m_target, sizeof(m_hookBuffer));
		}
	}
	void Remove()
	{
		m_isEnabled = !(m_isEnabled && MH_DisableHook(m_target) == MH_OK);
	}
	void EnsureApply()
	{
		if (memcmp(m_hookBuffer, m_target, sizeof(m_hookBuffer)) != 0)
		{
			DWORD oldProtect;
			VirtualProtect(m_target, sizeof(m_hookBuffer), PAGE_READWRITE, &oldProtect);
			memcpy(m_target, m_hookBuffer, sizeof(m_hookBuffer));
			VirtualProtect(m_target, sizeof(T), oldProtect, &oldProtect);
		}
	}
private:
	T m_trampoline;
	T m_target;
	T m_detour;
	bool m_isEnabled = false;
	char m_hookBuffer[20];

};


class cContext
{
public:
	static cContext& GetInstance();

	template<typename T> cDetour<T>* CreateDetour(T target, T detour)
	{
		auto pDetour = new cDetour<T>(target, detour);
		return pDetour;
	}
	template<typename T> bool        ApplyDetour(T target, T detour, cDetour<T>** ppDetour)
	{
		auto pDetour = CreateDetour(target, detour);
		if (pDetour)
		{
			*ppDetour = pDetour;
			pDetour->Apply();
			return true;
		}
		return false;
	}
	void CloseExit()
	{
		if (!(MH_Uninitialize() == MH_OK))
			TerminateProcess(GetCurrentProcess(), -1);
	}
	cContext() {}
	~cContext() {}
};

bool bInitialized = false;
cContext& cContext::GetInstance()
{
	if (!bInitialized)
		bInitialized = MH_Initialize() == MH_OK;
	static cContext pCtx;
	return pCtx;
}

