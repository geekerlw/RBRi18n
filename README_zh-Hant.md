[English](README.md) | [中文](README_zh.md) | **繁體中文** | [Português](README_pt.md) | [Suomi](README_fi.md) | [Русский](README_ru.md) | [日本語](README_jp.md)

# RBRi18n

一個輕量級的 **Richard Burns Rally (RBR)** 國際化 (i18n) 外掛程式。它透過攔截遊戲的文字渲染功能來載入翻譯，並以適當的縮放比例渲染 CJK 字型。

![預覽](preview.png)

## 功能特性

- 透過 `Language=zh|en` 設定支援多語言
- 自動更新：遊戲啟動時自動從 GitHub 取得最新翻譯檔案
- 按外掛程式劃分的翻譯檔案（如 `Translation.zh.json` 等）
- 可設定的字型系列和大小
- 解析度自適應字型縮放（基於 RBR 原生 640×480）
- 支援寬螢幕/超寬螢幕置中顯示

## 安裝方法

1. 將 `RBRi18n.dll` 複製到 RBR 的 `Plugins` 目錄
2. 在 RBR 根目錄建立 `RBRi18n` 資料夾
3. 翻譯檔案將在首次啟動時自動下載

## 快速安裝方法

1. 解壓縮 RRBi18n-v1.x.x.zip 檔案會產生兩個資料夾：Plugins 和 RBRi18n
2. 將產生的兩個資料夾及其內容直接拖曳到 RBR 遊戲根目錄
3. 系統會自動合併 Plugins 資料夾，如有重複檔案直接替換即可

## 設定說明

目前預設為中文，如需設定其他語言，請在遊戲根目錄的 `RichardBurnsRally.ini` 中新增：

```ini
[RBRi18n]
Language=zh-Hant

; 可選字型設定（顯示的是預設值）
FontFamily=SimHei
FontSizeSmall=7
FontSizeBig=8
FontSizeDebug=6
FontSizeHeading=8
FontSizeMenu=8
```

## 翻譯檔案

翻譯檔案使用 JSON 格式。檔案命名為 `{source}.{lang}.json`：
如果您對翻譯內容有任何建議或修正意見，請 Fork 此專案並提交您修正後的 JSON 檔案。

```
RBRi18n/
├── RichardBurnsRally.zh.json  # 基礎遊戲（中文）
├── Weather.zh.json            # 天氣（中文）
├── Options.zh.json            # 選項選單（中文）
├── TuneCar.zh.json            # 車輛調校（中文）
└── ...
```

翻譯檔案範例：

```json
{
  "Options": "選項",
  "Quick Rally": "快速拉力賽"
}
```

所有符合設定語言副檔名的檔案都會被載入並合併。

## 從原始碼建置

### 前置要求

- Windows
- 帶 C++ 工具的 Visual Studio（v143 工具集）
- Windows SDK 10.0

### 建置步驟

1. 在 Visual Studio 中開啟 `RBRi18n.vcxproj`（或新增至解決方案中）
2. 選擇 **Release | Win32**
3. 建置

輸出：`Release/RBRi18n.dll`

## 致謝

- RBR API 記憶體位址和結構來源於 [MIKA-N 的 RBRAPI](https://github.com/mika-n)（類 MIT 授權條款，詳見 `RBR/RBRAPI.h`）
- [MinHook](https://github.com/TsudaKageworthy/minhook) 用於函式攔截
- [nlohmann/json](https://github.com/nlohmann/json) 用於 JSON 解析

## 授權條款

[MIT](LICENSE)
