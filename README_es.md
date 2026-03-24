[English](README.md) | [中文](README_zh.md) | **Español** | [Português](README_pt.md)

# RBRi18n

Un plugin ligero de internacionalización (i18n) para **Richard Burns Rally (RBR)**. Intercepta el renderizado de texto del juego para cargar traducciones y renderizar fuentes CJK con la escala adecuada.

![vista previa](preview.png)

## Características

- Soporte multilingüe mediante la configuración `Language=zh|en`
- Actualización automática: descarga los archivos de traducción más recientes de GitHub al iniciar el juego
- Archivos de traducción por plugin (`Translation.zh.json`, etc.)
- Familia y tamaños de fuente configurables
- Escala de fuente adaptable a la resolución (basada en la resolución nativa 640×480 de RBR)
- Soporte de centrado para pantallas panorámicas y ultrawide

## Instalación

1. Copia `RBRi18n.dll` en el directorio `Plugins` de tu instalación de RBR
2. Crea una carpeta `RBRi18n` en el directorio raíz de RBR
3. Los archivos de traducción se descargan automáticamente en el primer inicio

## Instalación Rápida

1. Extrae el archivo `RRBi18n-v1.x.x.zip` para obtener dos carpetas: `Plugins` y `RBRi18n`
2. Arrastra ambas carpetas y su contenido directamente al directorio raíz del juego RBR
3. El sistema fusionará automáticamente la carpeta `Plugins`; sobrescribe los archivos si se te solicita

## Configuración

Añade lo siguiente a `RichardBurnsRally.ini` en el directorio raíz del juego:

```ini
[RBRi18n]
Language=zh

; Configuración de fuente opcional (se muestran los valores predeterminados)
FontFamily=SimHei
FontSizeSmall=7
FontSizeBig=8
FontSizeDebug=6
FontSizeHeading=8
FontSizeMenu=8
```

## Archivos de Traducción

Los archivos de traducción usan formato JSON. Los archivos se nombran `{fuente}.{idioma}.json`:
Si tienes sugerencias o correcciones para las traducciones, haz un fork de este proyecto y envía tu archivo JSON revisado.

```
RBRi18n/
├── RichardBurnsRally.zh.json  # Juego base (chino)
├── Weather.zh.json            # Traducciones del clima (chino)
├── Options.zh.json            # Menú de opciones (chino)
├── TuneCar.zh.json            # Ajuste de coche (chino)
└── ...
```

Ejemplo de archivo de traducción:

```json
{
  "Options": "Opciones",
  "Quick Rally": "Rally Rápido"
}
```

Todos los archivos que coincidan con la extensión de idioma configurada serán cargados y fusionados.

## Compilar desde el Código Fuente

### Requisitos Previos

- Windows
- Visual Studio con herramientas C++ (conjunto de herramientas v143)
- Windows SDK 10.0

### Compilación

1. Abre `RBRi18n.vcxproj` en Visual Studio (o agrégalo a una solución)
2. Selecciona **Release | Win32**
3. Compila

Salida: `Release/RBRi18n.dll`

## Créditos

- Direcciones de memoria y estructuras de la API de RBR derivadas de [RBRAPI por MIKA-N](https://github.com/mika-n) (licencia tipo MIT, ver `RBR/RBRAPI.h`)
- [MinHook](https://github.com/TsudaKageworthy/minhook) para la interceptación de funciones
- [nlohmann/json](https://github.com/nlohmann/json) para el análisis de JSON

## Licencia

[MIT](LICENSE)
