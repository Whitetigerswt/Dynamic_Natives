#include "sampgdk.h"
#include "subhook.h"
#include "Source.h"

#include <string>

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	// Handle sampGDK ticking.
	return sampgdk::ProcessTick();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	// Load SampGDK
	bool load = sampgdk::Load(ppData);

	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];

	InstallAmxHooks();

	// return SampGDK load value
	return load;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	return sampgdk::Unload();
}

cell AMX_NATIVE_CALL isPluginLoadedFunc(AMX* pAmx, cell* pParams)
{
	// Just return true, if we ever get here, the plugin is definitely loaded!
	return 1;
}

AMX_NATIVE_INFO PluginNatives[] =
{
	// Register our natives just how we would a normal plugin.
	{"IsPluginLoaded", isPluginLoadedFunc},
	{ 0, 0 }
};

AMX_NATIVE_INFO* GetPluginNatives()
{
	return PluginNatives;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *pAmx)
{
	// Allow the new loaded script to use our plugin's functions.
	return amx_Register(pAmx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *pAmx)
{
	return AMX_ERR_NONE;
}

static subhook::Hook amx_Register_hook;
static subhook::Hook GetPacketID_hook;

bool bHooksInstalled = false;

AMX_NATIVE GetGravityFunc = NULL;

unsigned long FUNC_GetPacketID = NULL;

cell AMX_NATIVE_CALL SecretProc(AMX* pAmx, cell* pParams)
{
	// If the number of parameters is greater than 0
	if (pParams[0] > 0)
	{
		// Get a list of all natives the plugin registered
		AMX_NATIVE_INFO* natives = GetPluginNatives();

		// gateway function sends the function name as a string as the first parameter always, so get the function we're trying to call
		char* func;
		amx_StrParam(pAmx, pParams[1], func);

		// Remove the first parameter string from the params
		pParams[0] -= 4;
		pParams[1] = pParams[0];

		// Loop through all natives registered
		int i = 0;
		while (natives[i].func > 0)
		{
			// If a native matches the function we're trying to call
			if (!strcmp(natives[i].name, func))
			{
				// Call it, and return 
				return natives[i].func(pAmx, &pParams[1]);
				break;
			}
			i++;
		}
	}

	// If we couldn't find the function, or no parameters were sent, call GetGravity as usual.
	return GetGravityFunc(pAmx, pParams);
}

int AMXAPI HOOK_amx_Register(AMX *amx, AMX_NATIVE_INFO *nativelist, int number)
{
	subhook::ScopedHookRemove remove(&amx_Register_hook);

	int i = 0;

	// Loop through all natives being registered
	while (!bHooksInstalled && nativelist[i].func > 0)
	{
		// If one matches GetGravity
		if (!strcmp(nativelist[i].name, "GetGravity"))
		{
			// Hook it
			bHooksInstalled = true;
			GetGravityFunc = nativelist[i].func;
			nativelist[i].func = SecretProc;
			break;
		}
		i++;
	}
	return amx_Register(amx, nativelist, number);
}

void InstallAmxHooks()
{
	amx_Register_hook.Install((void*)*(unsigned long*)((unsigned long)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
}