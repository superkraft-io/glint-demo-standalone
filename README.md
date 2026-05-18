# glint_demo

A demo application built on top of [glint](https://github.com/superkraft-io/glint) — a C++ UI framework with a DOM-like document tree, CSS layout, and a Skia-based rendering backend.

---

## Requirements

- **Node.js** (v18+)
- **CMake** (3.25+)
- **Windows**: Visual Studio 2022 with the "Desktop development with C++" workload
- **macOS**: Xcode command line tools
- **Linux / WSL2**: Clang 18+, Ninja, `libx11-dev`, `libegl-dev`, `libgl-dev`
- **Python 3** (required by Skia's build system)
- **Git**

---

## Getting Started

### 1. Clone the repo

```sh
git clone <this-repo-url>
cd glint_demo
```

### 2. Init submodules

```sh
git submodule update --init --recursive
```

### 3. Init Skia

#### Option A — Prebuilt libraries (recommended, fast)

Download prebuilt Skia libraries — no Python, no lengthy compile. This only needs to be run once (or again if you switch backends).

```sh
# macOS — Metal (recommended)
node third_party/glint/scripts/init_skia.mjs --prebuilt --backend metal

# Windows — Direct3D 12 (recommended)
node third_party/glint/scripts/init_skia.mjs --prebuilt --backend d3d12

# Linux — OpenGL (EGL + Ganesh)
node third_party/glint/scripts/init_skia.mjs --prebuilt --backend opengl
```

#### Option B — Build from source

If you need a custom configuration, you can build Skia from source instead. This takes significantly longer.

```sh
# macOS — Metal (recommended)
node third_party/glint/scripts/init_skia.mjs --source --backend metal --config Both

# iOS Simulator on macOS — Metal
node third_party/glint/scripts/init_skia.mjs --source --backend metal --target ios-simulator --config Both

# Windows — Direct3D 12 (recommended)
node third_party/glint/scripts/init_skia.mjs --source --backend d3d12 --config Both

# Linux — OpenGL (EGL + Ganesh)
node third_party/glint/scripts/init_skia.mjs --source --backend opengl --config Both
```

**Available backends by platform:**

| Backend | Platform | Description |
|---------|----------|-------------|
| `metal` | macOS, iOS | Metal (GPU) — recommended for Apple platforms |
| `d3d12` | Windows | Direct3D 12 (GPU) — recommended for Windows |
| `opengl` | Windows, Linux | OpenGL (GPU) |
| `dawn` | macOS, Windows | Dawn / WebGPU (GPU) |
| `cpu` | macOS, Windows, Linux | Software CPU renderer |

> `--config Both` builds both Debug and Release Skia libraries, which are required for the respective CMake build configurations. Only applicable for `--source`.

### 4. Configure with CMake

From the `demo/` directory:

**Windows (Visual Studio 2022):**

```sh
cd demo
cmake --preset windows-vs2022
```

**macOS:**

```sh
cd demo
cmake --preset macos
```

**iOS Simulator:**

```sh
cd demo
cmake --preset ios-sim-debug
```

**Linux (CPU renderer):**

```sh
cd demo
cmake --preset linux-debug
```

**Linux (OpenGL GPU renderer — recommended for WSL2/XWayland):**

```sh
cd demo
cmake --preset linux-debug-opengl
```

### 5. Build

**Windows:**

```sh
cmake --build build/windows-vs2022 --config Debug
```

**macOS:**

```sh
cmake --build --preset macos
```

**iOS Simulator:**

```sh
cmake --build --preset ios-sim-debug
```

**Linux:**

```sh
# CPU
cmake --build --preset linux-debug

# OpenGL GPU
cmake --build --preset linux-debug-opengl
```

### 6. Run

**Windows:**

```sh
.\build\windows-vs2022\Debug\glint_demo.exe
```

**macOS:**

```sh
./build/macos/glint_demo
```

**Linux:**

```sh
# CPU
DISPLAY=:0 ./build/linux/glint_demo

# OpenGL GPU
DISPLAY=:0 ./build/linux-opengl/glint_demo
```

> **WSL2 note:** Set `DISPLAY=:0` to route the window through WSLg/XWayland. The OpenGL preset uses EGL + Skia Ganesh for GPU-accelerated rendering via Mesa. The CPU preset falls back to XPutImage (software Skia → X11 shared memory).

---

## VS Code

Open the `glint_demo` folder in VS Code. Launch configurations and build tasks are already set up in `.vscode/`. Use the **Run and Debug** panel to build and launch the demo.

> **Linux / WSL2:** The VS Code launch configurations use `pipeTransport` with `wsl.exe` to build and debug through WSL2. Requires the [C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) and `gdb` installed in WSL (`sudo apt install gdb`). The distribution is assumed to be `Ubuntu-24.04`; edit `.vscode/tasks.json` and `.vscode/launch.json` if yours differs.

---

## License

The **Glint engine** (`third_party/glint`) is a commercial product.

- Free for non-commercial use: https://superkraft.io/license-free
- Commercial use & terms: https://superkraft.io/terms
- Licensing inquiries: **hello@superkraft.io**

The demo code in this repository is provided solely for evaluation purposes.

