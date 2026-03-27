
**English** | [中文](README_zh.md) | [繁體中文](README_zh-Hant.md) | [Português](README_pt.md) | [Suomi](README_fi.md) | [Русский](README_ru.md) | [日本語](README_jp.md) | [Magyar](README_hu.md)


# RBRi18n

A lightweight internationalization (i18n) plugin for **Richard Burns Rally (RBR)**. It hooks the game's text rendering to load translations and render CJK fonts with proper scaling.

![preview](preview.png)

## Features

- Multi-language support via `Language=zh|en` setting
- Auto-update: fetches latest translation files from GitHub on game launch
- Per-plugin translation files (`Translation.zh.json`, etc.)
- Configurable font family, sizes, and menu colors
- Resolution-aware font scaling (based on RBR's native 640×480)
- Widescreen/ultrawide centering support

## Installation

1. Copy `RBRi18n.dll` to your RBR `Plugins` directory
2. Create a `RBRi18n` folder in your RBR root directory
3. Translation files are downloaded automatically on first launch

## Quick Installation

1. Extract the RRBi18n-v1.x.x.zip file archive to get two folders: Plugins and RBRi18n
2. Drag both folders and their contents directly into your RBR game root directory
3. The system will automatically merge the Plugins folder; overwrite any files if prompted

## Configuration

The default language is Chinese. To configure a different language, add to `RichardBurnsRally.ini` in your game root:

```ini
[RBRi18n]
Language=zh

; Optional font settings (defaults shown)
FontFamily=Arial Unicode MS
FontSizeSmall=7
FontSizeBig=8
FontSizeDebug=6
FontSizeHeading=8
FontSizeMenu=8

; Optional menu colors (hex AARRGGBB or RRGGBB, defaults shown)
ColorBackground=FF323232
ColorSelection=FFFF0000
ColorIcon=FFC8C8C8
ColorText=FFFFFFFF
ColorHeading=FFFFFFFF

; Disable specific translation categories (comma-separated)
; Available: cars, stages, menu, options, tuning, rally, weather, tutorial, dailystages, misc
;DisableCategories=tutorial,dailystages
```

## Translation Files


Translation files use JSON format. Files are named `{source}.{lang}.json`:
If you have suggestions or corrections for the translations, please fork this project and submit your revised JSON file.

```
RBRi18n/
├── RichardBurnsRally.zh.json  # Base game (Chinese)
├── Weather.zh.json            # Weather translations (Chinese)
├── Options.zh.json            # Options menu (Chinese)
├── TuneCar.zh.json            # Car tuning (Chinese)
└── ...
```

Example translation file:

```json
{
  "Options": "选项",
  "Quick Rally": "快速拉力赛"
}
```

All files matching the configured language extension will be loaded and merged.

## Build from Source

### Prerequisites

- Windows
- Visual Studio with C++ tools (v143 toolset)
- Windows SDK 10.0

### Build

1. Open `RBRi18n.vcxproj` in Visual Studio (or add to a solution)
2. Select **Release | Win32**
3. Build

Output: `Release/RBRi18n.dll`

## Credits

- RBR API memory addresses and structs derived from [RBRAPI by MIKA-N](https://github.com/mika-n) (MIT-like license, see `RBR/RBRAPI.h`)
- [MinHook](https://github.com/TsudaKageworthy/minhook) for function hooking
- [nlohmann/json](https://github.com/nlohmann/json) for JSON parsing

## License

[MIT](LICENSE)
