What's this?
==========
Have you ever been making a SA-MP plugin, and you just got the ever so common, "File or Function not found"? This error is extremely useful, and it is a good thing the server does not continue executing when this error comes up.
However, sometimes it may be useful to include information about what is missing. What file or function is not found? Thus NativeChecker was born. The Native Checker plugin tells you which function was not found.

How can plugin developers include more information natively in their scripts to avoid this error?
==========
Well, up until now, you couldn't. However, I was working on a plugin and decided to investigate a little further to see if this could be avoided in a way. This plugin is a fork of SAMP_AC_v2 with just the file or function not found errors fixed up in an example script.

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

Which can provide a lot better information to servers than just "File or function not found", or even "Function not found: MapAndreas_FindZ".