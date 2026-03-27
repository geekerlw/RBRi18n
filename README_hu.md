[English](README.md) | [中文](README_zh.md) | [繁體中文](README_zh-Hant.md) | [Português](README_pt.md) | [Suomi](README_fi.md) | [Русский](README_ru.md) | [日本語](README_jp.md) | **Magyar**

# RBRi18n

Egy könnyűsúlyú nemzetköziesítési (i18n) bővítmény a **Richard Burns Rally (RBR)** játékhoz. A játék szövegmegjelenítését módosítva tölti be a fordításokat, és megfelelő méretezéssel jeleníti meg a CJK betűtípusokat.

![előnézet](preview.png)

## Funkciók

- Többnyelvű támogatás a `Language=zh|en` beállítással
- Automatikus frissítés: a játék indításakor letölti a legújabb fordítási fájlokat a GitHubról
- Bővítményenkénti fordítási fájlok (`Translation.zh.json` stb.)
- Beállítható betűtípuscsalád és -méretek
- Felbontásfüggő betűméretezés (az RBR natív 640×480-as felbontása alapján)
- Szélesvásznú/ultraszéles középre igazítás támogatása

## Telepítés

1. Másold a `RBRi18n.dll` fájlt az RBR `Plugins` könyvtárába
2. Hozz létre egy `RBRi18n` mappát az RBR gyökérkönyvtárában
3. A fordítási fájlok automatikusan letöltődnek az első indításkor

## Gyorstelepítés

1. Csomagold ki az `RRBi18n-v1.x.x.zip` fájlt, amely két mappát tartalmaz: `Plugins` és `RBRi18n`
2. Húzd mindkét mappát és tartalmukat közvetlenül az RBR játék gyökérkönyvtárába
3. A rendszer automatikusan egyesíti a `Plugins` mappát; ha kéri, írd felül a fájlokat

## Beállítás

Az alapértelmezett nyelv a kínai. Más nyelv beállításához add hozzá a következőt a játék gyökérkönyvtárában található `RichardBurnsRally.ini` fájlhoz:

```ini
[RBRi18n]
Language=hu

; Opcionális betűtípus-beállítások (alapértelmezett értékek)
FontFamily=Arial Unicode MS
FontSizeSmall=7
FontSizeBig=8
FontSizeDebug=6
FontSizeHeading=8
FontSizeMenu=8
```

## Fordítási fájlok

A fordítási fájlok JSON formátumot használnak. A fájlok neve: `{forrás}.{nyelv}.json`:
Ha javaslataid vagy javításaid vannak a fordításokhoz, kérjük, forkold ezt a projektet és küldd be a javított JSON fájlodat.

```
RBRi18n/
├── RichardBurnsRally.zh.json  # Alapjáték (kínai)
├── Weather.zh.json            # Időjárás fordítások (kínai)
├── Options.zh.json            # Beállítások menü (kínai)
├── TuneCar.zh.json            # Autóhangolás (kínai)
└── ...
```

Példa fordítási fájl:

```json
{
  "Options": "Beállítások",
  "Quick Rally": "Gyors rali"
}
```

Az összes, a beállított nyelvi kiterjesztésnek megfelelő fájl betöltődik és egyesítésre kerül.

## Fordítás forráskódból

### Előfeltételek

- Windows
- Visual Studio C++ eszközökkel (v143 eszközkészlet)
- Windows SDK 10.0

### Fordítás

1. Nyisd meg a `RBRi18n.vcxproj` fájlt a Visual Studióban (vagy add hozzá egy megoldáshoz)
2. Válaszd a **Release | Win32** konfigurációt
3. Fordítsd le

Kimenet: `Release/RBRi18n.dll`

## Köszönetnyilvánítás

- Az RBR API memóriacímek és struktúrák a [MIKA-N RBRAPI](https://github.com/mika-n) projektből származnak (MIT-szerű licenc, lásd `RBR/RBRAPI.h`)
- [MinHook](https://github.com/TsudaKageworthy/minhook) a függvényhookoláshoz
- [nlohmann/json](https://github.com/nlohmann/json) a JSON feldolgozáshoz

## Licenc

[MIT](LICENSE)
