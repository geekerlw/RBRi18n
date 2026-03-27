
[English](README.md) | [中文](README_zh.md) | [繁體中文](README_zh-Hant.md) | **Português** | [Suomi](README_fi.md) | [Русский](README_ru.md) | [日本語](README_jp.md) | [Magyar](README_hu.md)


# RBRi18n

Um plugin leve de internacionalização (i18n) para **Richard Burns Rally (RBR)**. Ele intercepta a renderização de texto do jogo para carregar traduções e renderizar fontes CJK com o dimensionamento adequado.

![prévia](preview.png)

## Recursos

- Suporte a múltiplos idiomas via configuração `Language=zh|en`
- Atualização automática: baixa os arquivos de tradução mais recentes do GitHub ao iniciar o jogo
- Arquivos de tradução por plugin (`Translation.zh.json`, etc.)
- Família e tamanhos de fonte configuráveis
- Dimensionamento de fonte adaptável à resolução (baseado na resolução nativa 640×480 do RBR)
- Suporte a centralização para telas widescreen/ultrawide

## Instalação

1. Copie `RBRi18n.dll` para o diretório `Plugins` da sua instalação do RBR
2. Crie uma pasta `RBRi18n` no diretório raiz do RBR
3. Os arquivos de tradução são baixados automaticamente no primeiro início

## Instalação Rápida

1. Extraia o arquivo `RRBi18n-v1.x.x.zip` para obter duas pastas: `Plugins` e `RBRi18n`
2. Arraste ambas as pastas e seus conteúdos diretamente para o diretório raiz do jogo RBR
3. O sistema mesclará automaticamente a pasta `Plugins`; substitua os arquivos se solicitado

## Configuração

O idioma padrão é o chinês. Para configurar um idioma diferente, adicione ao `RichardBurnsRally.ini` no diretório raiz do jogo:

```ini
[RBRi18n]
Language=pt

; Configurações de fonte opcionais (valores padrão exibidos)
FontFamily=Arial Unicode MS
FontSizeSmall=7
FontSizeBig=8
FontSizeDebug=6
FontSizeHeading=8
FontSizeMenu=8

; Cores de menu opcionais (hex AARRGGBB ou RRGGBB, valores padrão exibidos)
ColorBackground=FF323232
ColorSelection=FFFF0000
ColorIcon=FFC8C8C8
ColorText=FFFFFFFF
ColorHeading=FFFFFFFF

; Desativar categorias de tradução específicas (separadas por vírgula)
; Disponíveis: cars, stages, menu, options, tuning, rally, weather, tutorial, dailystages, misc
;DisableCategories=tutorial,dailystages
```

## Arquivos de Tradução

Os arquivos de tradução usam formato JSON. Os arquivos são nomeados `{fonte}.{idioma}.json`:
Se você tiver sugestões ou correções para as traduções, faça um fork deste projeto e envie seu arquivo JSON revisado.

```
RBRi18n/
├── RichardBurnsRally.zh.json  # Jogo base (chinês)
├── Weather.zh.json            # Traduções de clima (chinês)
├── Options.zh.json            # Menu de opções (chinês)
├── TuneCar.zh.json            # Ajuste de carro (chinês)
└── ...
```

Exemplo de arquivo de tradução:

```json
{
  "Options": "Opções",
  "Quick Rally": "Rally Rápido"
}
```

Todos os arquivos que corresponderem à extensão de idioma configurada serão carregados e mesclados.

## Compilar a partir do Código Fonte

### Pré-requisitos

- Windows
- Visual Studio com ferramentas C++ (conjunto de ferramentas v143)
- Windows SDK 10.0

### Compilação

1. Abra `RBRi18n.vcxproj` no Visual Studio (ou adicione a uma solução)
2. Selecione **Release | Win32**
3. Compile

Saída: `Release/RBRi18n.dll`

## Créditos

- Endereços de memória e estruturas da API do RBR derivados de [RBRAPI por MIKA-N](https://github.com/mika-n) (licença tipo MIT, ver `RBR/RBRAPI.h`)
- [MinHook](https://github.com/TsudaKageworthy/minhook) para interceptação de funções
- [nlohmann/json](https://github.com/nlohmann/json) para análise de JSON

## Licença

[MIT](LICENSE)
