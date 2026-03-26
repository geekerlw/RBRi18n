
[English](README.md) | **中文** | [繁體中文](README_zh-Hant.md) | [Português](README_pt.md) | [Suomi](README_fi.md) | [Русский](README_ru.md) | [日本語](README_jp.md)

# RBRi18n

一个轻量级的 **Richard Burns Rally (RBR)** 国际化 (i18n) 插件。它通过钩取游戏的文本渲染功能来加载翻译，并以适当的缩放比例渲染 CJK 字体。

![预览](preview.png)

## 功能特性

- 通过 `Language=zh|en` 设置支持多语言
- 自动更新：游戏启动时自动从 GitHub 获取最新翻译文件
- 按插件划分的翻译文件（如 `Translation.zh.json` 等）
- 可配置的字体系列和大小
- 分辨率自适应字体缩放（基于 RBR 原生 640×480）
- 支持宽屏/超宽屏居中显示

## 安装方法

1. 将 `RBRi18n.dll` 复制到 RBR 的 `Plugins` 目录
2. 在 RBR 根目录创建 `RBRi18n` 文件夹
3. 翻译文件将在首次启动时自动下载

## 快速安装方法
1. 解压缩RRBi18n-v1.x.x.zip文件会生成的两个文件夹Plugins 和 RBRi18n
2. 将生成的两个文件夹以及内容直接拖到RBR游戏根目录
3. 系统会自动合并Plugins文件夹，如果有重复的替换即可

## 配置说明

现在默认是中文，如果需要配置其他语言
在游戏根目录的 `RichardBurnsRally.ini` 中添加：

```ini
[RBRi18n]
Language=zh

; 可选字体设置（显示的是默认值）
FontFamily=SimHei
FontSizeSmall=7
FontSizeBig=8
FontSizeDebug=6
FontSizeHeading=8
FontSizeMenu=8

; 可选菜单颜色（十六进制 AARRGGBB 或 RRGGBB，显示的是默认值）
ColorBackground=FF323232
ColorSelection=FFFF0000
ColorIcon=FFC8C8C8
ColorText=FFFFFFFF
ColorHeading=FFFFFFFF
```

## 翻译文件

翻译文件使用 JSON 格式。文件命名为 `{source}.{lang}.json`：
如果你对翻译内容有任何异议和改进意见，请Fork这个项目并提交你的校正后JSON文件

```
RBRi18n/
├── RichardBurnsRally.zh.json  # 基础游戏（中文）
├── Weather.zh.json            # 天气（中文）
├── Options.zh.json            # 选项菜单（中文）
├── TuneCar.zh.json            # 调校菜单（中文）
└── ...
```

翻译文件示例：

```json
{
	"Options": "选项",
	"Quick Rally": "快速拉力赛"
}
```


所有匹配配置语言扩展名的文件都会被加载并合并。

## 从源码构建

### 前置要求

- Windows
- 带 C++ 工具的 Visual Studio（v143 工具集）
- Windows SDK 10.0

### 构建步骤

1. 在 Visual Studio 中打开 `RBRi18n.vcxproj`（或添加到解决方案中）
2. 选择 **Release | Win32**
3. 构建

输出：`Release/RBRi18n.dll`

## 致谢

- RBR API 内存地址和结构来源于 [MIKA-N 的 RBRAPI](https://github.com/mika-n)（类 MIT 许可证，详见 `RBR/RBRAPI.h`）
- [MinHook](https://github.com/TsudaKageworthy/minhook) 用于函数钩取
- [SimpleIni](https://github.com/brofield/simpleini) 用于 INI 解析

## 许可证

[MIT](LICENSE)
