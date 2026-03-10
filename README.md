# Project Vortex v0.1.0

一个基于 **C++ + JavaScript** 的本地图像处理软件基础版本（0.1.0）。

- **C++** 负责图像处理核心（打开、保存、旋转、裁切、曝光调整）与模块加载。
- **JavaScript** 负责交互式 UI（运行在本地桌面窗口内）。
- UI 由 Qt WebEngine 加载本地资源（`qrc`），**不是 localhost Web 服务**。

## 0.1.0 已实现能力

- 打开/保存图像（PNG/JPG 等由 Qt 解码支持）
- 顺时针旋转 90°
- 自定义区域裁切
- 简单曝光（亮度）倍率调整
- 可扩展模块机制（动态加载 `modules` 目录下的 `.so/.dll/.dylib`）
- 附带示例模块：`Invert Colors`（颜色反转）

## 技术架构

```text
┌─────────────────────────────────────────────┐
│ Desktop App (Qt Widgets + QWebEngineView)  │
│   ├─ JS UI (frontend/index.html + app.js)   │
│   ├─ QWebChannel                            │
│   └─ C++ ImageController                    │
│        ├─ 图像操作 (QImage)                  │
│        └─ ModuleManager (QLibrary)          │
└─────────────────────────────────────────────┘
```

## 目录结构

```text
.
├── CMakeLists.txt
├── README.md
├── resources.qrc
├── frontend/
│   ├── index.html
│   ├── app.js
│   └── style.css
├── src/
│   ├── main.cpp
│   ├── MainWindow.h/.cpp
│   ├── ImageController.h/.cpp
│   ├── ModuleManager.h/.cpp
│   └── module_api.h
└── modules/
    └── sample_invert_module.cpp
```

## 构建与运行

> 依赖：Qt6（`Widgets`, `WebEngineWidgets`, `WebChannel`）+ CMake + C++17 编译器

```bash
cmake -S . -B build
cmake --build build -j
```

运行主程序：

```bash
./build/vortex
```

安装（可选）：

```bash
cmake --install build --prefix ./dist
```

安装后模块建议放在：

```text
dist/modules/
```

程序会在启动时尝试加载 `applicationDirPath()/modules` 下所有模块。

## 模块开发接口（0.1.0）

模块需要导出函数：

```cpp
extern "C" VORTEX_EXPORT const ModuleDescriptor* register_module();
```

核心结构定义见 `src/module_api.h`。

最小示例（伪代码）：

```cpp
#include "module_api.h"

bool apply(VortexImage* image) {
  // 对 image->data 做原地处理
  return true;
}

const ModuleDescriptor d{
  "your.module.id",
  "Your Module Name",
  "0.1.0",
  &apply
};

extern "C" VORTEX_EXPORT const ModuleDescriptor* register_module() {
  return &d;
}
```

## 注意事项

- 当前 UI 使用“路径输入”方式打开/保存，后续可升级为系统文件对话框。
- 示例模块仅用于演示插件机制，生产环境请加入版本校验、沙箱与错误隔离机制。
