# OpenGlass
A replica of the dead software glass8, also known as the upstream project of [DWMBlurGlass](https://github.com/Maplespe/DWMBlurGlass). This is a fork improving the performance and stability of OpenGlass.

This branch uses raw Direct2D to leverage better performance than the master branch (You can find the master branch [here](https://github.com/ALTaleX531/OpenGlass/tree/master)), however due to it's early nature you may encounter more bugs and crashes. Currently this branch only supports Windows 10 2004-22H2.

You can use DWM Tweaker (Windhawk mod) with this build of OpenGlass! You may find it [here](https://raw.githubusercontent.com/n1d3v/OpenGlass/refs/heads/legacy-optimized/Files/dwmtweaker.cpp) (Thank you Tech Stuff!) (Paste the code from the .cpp file to Windhawk's development environment)
> [!IMPORTANT]  
> This software is intended for advanced users only. If you are a beginner and you do not have deeper knowledge of Windows (such as registry editing etc.) you shouldn't use this. You can use [DWMBlurGlass](https://github.com/Maplespe/DWMBlurGlass) instead.  

> [!WARNING]   
> OpenGlass does not support and is not intended to support Windows Insider Preview, so if you want to use it in these Windows versions please do it at your own risk. It will do nothing if it isn't compatible.

## Demonstration
![demo](https://github.com/user-attachments/assets/faa1ca88-4f95-4287-a78c-2d8171092e73)
> *.msstyles theme used for screenshots: [Aero10 by vaporvance](https://www.deviantart.com/vaporvance/art/Aero10-for-Windows-10-1903-22H2-909711949)*

> *additional software used: [Windhawk](https://windhawk.net/), [Aero Window Manager](https://github.com/Dulappy/aero-window-manager) by @Dulappy, [explorer7](https://github.com/explorer7-team/source) by the explorer7 team*

## Install instructions
Go look at the official `legacy` branch of OpenGlass [here](https://github.com/ALTaleX531/OpenGlass?tab=readme-ov-file#how-to-use-this-software). It is recommended to use the Windhawk method for better overall stability.
### Windhawk method (DWM Tweaker)
> [!IMPORTANT]  
> Make sure OpenGlass is **not** running before doing this, otherwise some stuff can break (Not permament damage)
1. Download both DWM Tweaker (Found at top of this README) and the latest build of OpenGlass here.
2. Extract the build of OpenGlass you have downloaded.
3. Take the `OpenGlass.dll` file and put it in `C:\ProgramData\Windhawk\Engine\ModsWritable` (You may need to create the `ModsWritable` directory).
4. Rename the DLL to `OpenGlassOpt.dll`.
5. Go to `regedit.exe` and locate `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\DWM` and create the DWORD `DisableGlassInjection` and set it to 1.
6. Open Windhawk and click "Create a New mod" and then paste the code from `dwmtweaker.cpp` into there.
7. Compile the mod and after it downloads symbols it should work. You can use the settings from [here](https://raw.githubusercontent.com/n1d3v/OpenGlass/refs/heads/legacy-optimized/Files/settings.reg) to make it look like Windows 7.

> [!NOTE]  
> To save DWM Tweaker and the default settings raw GitHub files right click and click `Save as`. Another way is to copy the file contents and create the file yourself.

## Documentation
The legacy branch can use some of the features of the master branch. Options which are not listed below are not supported in the Legacy branch, vice versa. 

> [!IMPORTANT]  
> Unless specified, the options below are stored in `HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\DWM` and `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\DWM`. OpenGlass prefers to read the settings in the HKCU.

**Colorization settings**

| Key Name | Type | Description | 
| -------- | ---- | ----------- |
| ColorizationColorOverride | DWORD | Overrides the default Windows `ColorizationColor`. The color is in ARGB (i.e 0x6B74B8FC). If this is not set, it'll use the equivalent original registry key. |
| ColorizationAfterglowOveride | DWORD | Overrides the default Windows `ColorizationAfterglow`. The color is in ARGB (i.e 0x6B74B8FC). If this is not set, it'll use the equivalent original registry key.  |
| ColorizationColorBalanceOverride | DWORD | Overrides the default Windows `ColorizationColorBalance`. This key is typically controlled by the intensity slider in Control panel. If you were using the old shader fork previously, this is the equivalent to `og_ColorizationColorBalance`. If this is not set, it'll use the equivalent original registry key. |
| ColorizationAfterglowBalanceOverride | DWORD | Overrides the default Windows `ColorizationAfterglowBalance`. This key is typically controlled by the intensity slider in Control panel. If you were using the old shader fork previously, this is the equivalent to `og_ColorizationColorBalance`. If this is not set, it'll use the equivalent original registry key. |
| ColorizationBlurBalanceOverride | DWORD | Overrides the default Windows `ColorizationBlurBalance`. This key is typically controlled by the intensity slider in Control panel. If you were using the old shader fork previously, this is the equivalent to `og_ColorizationBlurBalance`. If this is not set, it'll use the equivalent original registry key. |

**Glass settings**
| Key Name | Type | Description | 
| -------- | ---- | ----------- |
| GlassType | DWORD | The type of backdrop effect. Currently, only 2 options are supported: 0x0=Basic blur and 0x01=Aero |
| GlassOverrideAccent | DWORD | Overrides surfaces with accent policies with OpenGlass effects, I.E: the taskbar. Set to 1 to enable. |
| EnableGeometryMerging | DWORD | Merges multiple  blur surfaces to render as one. Most notably, this will eliminate the "artifact" near the titlebar edges. <br><br>**!! THIS OPTION MAY HAVE A SEVERE IMPACT ON PERFORMANCE. USE AT YOUR OWN RISK. !!** |
| ColorizationGlassReflectionIntensity | DWORD | Controls the opacity of the glass streaks effect in the window. |
| ColorizationGlassReflectionParallaxIntensity | DWORD | Controls intensity of the parallax effect (I.E when moving the windows side to side) of the glass streaks. | 
| BlurDeviation | DWORD | Controls the radius (intensity) of the gaussian blur effect. |
| TextGlowSize | DWORD | Controls the size of the titlebar text glow effect. |
| RoundRectRadius | DWORD | Controls radius of the blur behind, like a rounded rectangle. Win8 = 0, Win7 = 12 | 
| CustomThemeMsstyle | String | path to msstyle file. | 
| CustomThemeMsstyleUseDefaults | DWORD | Color scheme uses the result from GetThemeDefaults | 
| EnableFullDirty | DWORD | Make the dirty region extend to the entire desktop by using a hack. This will eliminate the "flickering" caused by redrawing due to missing glass safety zones, thus useful for those using a high blur radius. As a price, your entire desktop is rendered instead of just the necessary regions, but the occlusion optimization still works fine. <br><br>**!! THIS OPTION DEFINITELY HAVE A IMPACT ON PERFORMANCE. USE AT YOUR OWN RISK. !!** |

**Extra settings**
| Key Name | Type | Description | 
| -------- | ---- | ----------- |
| DisableGlassInjection | DWORD | Setting this to 1 makes it compatible with Windhawk (DWM Tweaker) (Need to change in HKLM) |

## How to reload the configuration
OpenGlass does not provide any GUI nor commands to explicitly reload the configuration. You can use glass8's UI to refresh it though.
However, it's actually quite easy to do so in C/C++.
```c++
PostMessage(FindWindow(TEXT("Dwm"), nullptr), WM_THEMECHANGED, 0, 0);            // Refresh part of the settings related to theme
PostMessage(FindWindow(TEXT("Dwm"), nullptr), WM_DWMCOLORIZATIONCHANGED, 0, 0);  // Refresh part of the settings related to color/backdrop
```

## Dependencies and References
### [Microsoft Research Detours Package](https://github.com/microsoft/Detours)  
Detours is a software package for monitoring and instrumenting API calls on Windows.  
### [VC-LTL - An elegant way to compile lighter binaries.](https://github.com/Chuyu-Team/VC-LTL5)  
VC-LTL is an open source CRT library based on the MS VCRT that reduce program binary size and say goodbye to Microsoft runtime DLLs, such as msvcr120.dll, api-ms-win-crt-time-l1-1-0.dll and other dependencies.  
### [Windows Implementation Libraries (WIL)](https://github.com/Microsoft/wil)  
The Windows Implementation Libraries (WIL) is a header-only C++ library created to make life easier for developers on Windows through readable type-safe C++ interfaces for common Windows coding patterns. 
### [Interop Compositor](https://blog.adeltax.com/interopcompositor-and-coredispatcher/)
Saved me some decompiling and reverse engineering time thanks to ADeltaX's blog!
### [Win32Acrylic](https://github.com/ALTaleX531/Win32Acrylic)
Win2D sucks!
### [AcrylicEverywhere](https://github.com/ALTaleX531/AcrylicEverywhere)
The predecessor of this project.
### [Loading Visual Styles Per-Application](https://winclassic.net/thread/2178/loading-visual-styles-application)
It is possible for an application to load a style on a per-application basis using an undocumented API.
### Wallpaper
Wallpaper is from the Architectures theme from Windows 7.
