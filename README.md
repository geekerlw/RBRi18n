# RBRi18n

A lightweight internationalization (i18n) plugin for **Richard Burns Rally (RBR)**. It hooks the game's text rendering to load translations and render CJK fonts with proper scaling.

## Features

- Multi-language support via `Language=zh|en` setting
- Per-plugin translation files (`RichardBurnsRally.zh`, `RallySimFans.hu.zh`, `SimRallyCN.zh`, etc.)
- Configurable font family and sizes
- Resolution-aware font scaling (based on RBR's native 640×480)
- Widescreen/ultrawide centering support

## Installation

1. Copy `RBRi18n.dll` to your RBR `Plugins` directory
2. Create a `RBRi18n` folder in your RBR root directory
3. Place translation files (`.zh`, `.en`, etc.) in the `RBRi18n` folder

## Configuration

Add to `RichardBurnsRally.ini` in your game root:

```ini
[RBRi18n]
Language=zh

; Optional font settings (defaults shown)
FontFamily=SimHei
FontSizeSmall=7
FontSizeBig=8
FontSizeDebug=6
FontSizeHeading=8
FontSizeMenu=8
```

## Translation Files

Translation files use INI format with a `[Translations]` section. Files are named `{source}.{lang}`:

```
RBRi18n/
├── RichardBurnsRally.zh    # Base game (Chinese)
├── RichardBurnsRally.en    # Base game (English)
├── RallySimFans.hu.zh      # RSF plugin (Chinese)
├── SimRallyCN.zh           # SimRallyCN plugin (Chinese)
└── ...
```

Example translation file:

```ini
[Translations]
Options=选项
Quick Rally=快速拉力赛
```

All files matching the configured language extension are loaded and merged.

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
- [SimpleIni](https://github.com/brofield/simpleini) for INI parsing

## License

[MIT](LICENSE)
