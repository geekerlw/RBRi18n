[English](README.md) | [中文](README_zh.md) | [繁體中文](README_zh-Hant.md) | [Português](README_pt.md) | [Suomi](README_fi.md) | [Русский](README_ru.md) | **日本語**

# RBRi18n

**Richard Burns Rally (RBR)** 向けの軽量な国際化 (i18n) プラグインです。ゲームのテキストレンダリングをフックし、翻訳を読み込み、適切なスケーリングで CJK フォントを描画します。

![プレビュー](preview.png)

## 機能

- `Language=zh|en` 設定による多言語サポート
- 自動更新：ゲーム起動時に GitHub から最新の翻訳ファイルを取得
- プラグインごとの翻訳ファイル（`Translation.zh.json` など）
- フォントファミリーとサイズの設定が可能
- 解像度に応じたフォントスケーリング（RBR のネイティブ解像度 640×480 基準）
- ワイドスクリーン/ウルトラワイドのセンタリングサポート

## インストール

1. `RBRi18n.dll` を RBR の `Plugins` ディレクトリにコピー
2. RBR のルートディレクトリに `RBRi18n` フォルダを作成
3. 翻訳ファイルは初回起動時に自動ダウンロードされます

## クイックインストール

1. `RRBi18n-v1.x.x.zip` ファイルを解凍して、`Plugins` と `RBRi18n` の2つのフォルダを取得
2. 両方のフォルダとその中身を RBR ゲームのルートディレクトリに直接ドラッグ
3. システムが自動的に `Plugins` フォルダを統合します。ファイルの上書きを求められた場合は上書きしてください

## 設定

デフォルト言語は中国語です。別の言語を設定するには、ゲームルートディレクトリの `RichardBurnsRally.ini` に以下を追加してください：

```ini
[RBRi18n]
Language=jp

; オプションのフォント設定（デフォルト値を表示）
FontFamily=SimHei
FontSizeSmall=7
FontSizeBig=8
FontSizeDebug=6
FontSizeHeading=8
FontSizeMenu=8

; オプションのメニュー色（16進数 AARRGGBB または RRGGBB、デフォルト値を表示）
ColorBackground=FF323232
ColorSelection=FFFF0000
ColorIcon=FFC8C8C8
ColorText=FFFFFFFF
ColorHeading=FFFFFFFF
```

## 翻訳ファイル

翻訳ファイルは JSON 形式を使用します。ファイルは `{ソース}.{言語}.json` の形式で命名されます：
翻訳に対する提案や修正がある場合は、このプロジェクトをフォークして修正済みの JSON ファイルを提出してください。

```
RBRi18n/
├── RichardBurnsRally.zh.json  # ベースゲーム（中国語）
├── Weather.zh.json            # 天気の翻訳（中国語）
├── Options.zh.json            # オプションメニュー（中国語）
├── TuneCar.zh.json            # 車のチューニング（中国語）
└── ...
```

翻訳ファイルの例：

```json
{
  "Options": "オプション",
  "Quick Rally": "クイックラリー"
}
```

設定された言語拡張子に一致するすべてのファイルが読み込まれ、統合されます。

## ソースからビルド

### 前提条件

- Windows
- C++ ツール付き Visual Studio（v143 ツールセット）
- Windows SDK 10.0

### ビルド

1. Visual Studio で `RBRi18n.vcxproj` を開く（またはソリューションに追加）
2. **Release | Win32** を選択
3. ビルド

出力：`Release/RBRi18n.dll`

## クレジット

- RBR API のメモリアドレスと構造体は [MIKA-N の RBRAPI](https://github.com/mika-n) から派生（MIT 系ライセンス、`RBR/RBRAPI.h` を参照）
- [MinHook](https://github.com/TsudaKageworthy/minhook) - 関数フック用
- [nlohmann/json](https://github.com/nlohmann/json) - JSON パース用

## ライセンス

[MIT](LICENSE)
