What's this?
==========
Have you ever been making a SA-MP plugin, and you just got the ever so common, "File or Function not found"? This error is extremely useful, and it is a good thing the server does not continue executing when this error comes up.
However, sometimes it may be useful to include information about what is missing. What file or function is not found? Thus NativeChecker was born. The Native Checker plugin tells you which function was not found. This plugin is a proof-of-concept that the "File or Function not found" error can be completely eliminated.

What's the difference between this and NativeChecker?
==========
This allows you to put your own custom error messages if a plugin is not loaded. Or maybe, you could run your gamemode or filterscript in a less-than-complete mode without an error and recover from the "file or function not found" error.

How might this be useful?
==========
If plugin developers adapt this technique to their plugins, it would allow some pretty great things like:
```
public OnFilterScriptInit()
{
	if(IsPluginLoaded())
	{
		printf("Hey!! You need to download the latest version of plugin [x] at link [y].
		printf("The server will now terminate. Install the plugin to continue.");
		SendRconCommand("exit");
	}
}
```

Which can provide a lot better information to servers than just "File or function not found", or even "Function not found: MapAndreas_FindZ". It allows people to understand why something may have went wrong when they are installing your gamemode, filterscript, or include. This is valuable so that you can make your gamemode or filterscript run with or without plugins.


How does it work?
==========
This plugin hooks the native GetGravity from the AMX list. It uses this native, but it really does not matter which native you use. Any native can be hooked and achieve the same effect, it does not mess up the original "GetGravity" native.

Since PAWN allows us to pass params to natives even if they don't use them, this plugin takes advantage of that. Everytime GetGravity is called, it checks if an extra parameter is attached to it, a string, which is the "real" function name. If the extra param is there, we call that function instead of GetGravity. This allows all plugin natives to be defined by a proxy native which is GetGravity. In reality, you are calling a defined native "GetGravity". Only, if the plugin is loaded, GetGravity is hooked. If the proxy native returns the same value as "GetGravity", then the plugin is not loaded, ASSUMING you are using a native that actually exists.

Other parameters can be added onto natives to have multi-paramed functions. No functionality in plugins will be lost if implemented correctly.

Here's an example include that is using this technique in another plugin: http://samp-ac.com/sampac.inc
