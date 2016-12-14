#include <a_samp>

#if !defined GetGravity
	native Float:GetGravity();
#endif

native plugin_gateway_param0(func[]) = GetGravity;

#define IsPluginLoaded() ((_:GetGravity()) != plugin_gateway_param0("IsPluginLoaded"))

public OnFilterScriptInit()
{
	printf("Plugin loaded: %d", IsPluginLoaded());
}
