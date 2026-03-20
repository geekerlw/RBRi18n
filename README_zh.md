[English](README.md) | **中文**

# RBRi18n（中文说明）

一个轻量级的 **Richard Burns Rally (RBR)** 国际化 (i18n) 插件。它通过钩取游戏的文本渲染功能来加载翻译并以适当的缩放比例渲染 CJK 字体。

![预览](preview.png)

## 功能特性

- 通过 `Language=zh|en` 设置支持多语言
- 按插件划分的翻译文件（`RichardBurnsRally.zh`、`RallySimFans.hu.zh`、`SimRallyCN.zh` 等）
- 可配置的字体系列和大小
- 分辨率自适应字体缩放（基于 RBR 原生 640×480 分辨率）
- 支持宽屏/超宽屏居中显示

## 安装方法

1. 将 `RBRi18n.dll` 复制到 RBR 的 `Plugins` 目录
2. 在 RBR 根目录创建 `RBRi18n` 文件夹
3. 将翻译文件（`.zh`、`.en` 等）放入 `RBRi18n` 文件夹

## 配置说明

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
```

## 翻译文件

翻译文件使用 INI 格式，包含 `[Translations]` 节。文件命名格式为 `{来源}.{语言}`：

```
RBRi18n/
├── RichardBurnsRally.zh    # 基础游戏（中文）
├── RichardBurnsRally.en    # 基础游戏（英文）
├── RallySimFans.hu.zh      # RSF 插件（中文）
├── SimRallyCN.zh           # SimRallyCN 插件（中文）
└── ...
```

翻译文件示例：

```ini
[Translations]
Options=选项
Quick Rally=快速拉力赛
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
