# glint_demo

A demo application built on top of [glint](https://github.com/superkraft-io/glint) — a C++ UI framework with a DOM-like document tree, CSS layout, and a Skia-based rendering backend.

---

## Requirements

- **Node.js** (v18+)
- **CMake** (3.25+)
- **Windows**: Visual Studio 2022 with the "Desktop development with C++" workload
- **macOS**: Xcode command line tools
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

### 3. Build Skia

Run the following command from the repo root to download and compile Skia for your chosen render backend. This only needs to be run once (or again if you switch backends).

```sh
node third_party/glint/scripts/init_skia.mjs --source --backend <backend>
```

**Available backends by platform:**

| Backend | Platform | Description |
|---------|----------|-------------|
| `metal` | macOS | Metal (GPU) — recommended for macOS |
| `d3d12` | Windows | Direct3D 12 (GPU) — recommended for Windows |
| `opengl` | Windows | OpenGL (GPU) |
| `dawn` | macOS, Windows | Dawn / WebGPU (GPU) |
| `cpu` | macOS, Windows | Software CPU renderer |

**Examples:**

```sh
# macOS — Metal (recommended)
node third_party/glint/scripts/init_skia.mjs --source --backend metal

# Windows — Direct3D 12 (recommended)
node third_party/glint/scripts/init_skia.mjs --source --backend d3d12
```

> This step takes a while. It only needs to be run once (or again if you switch backends).

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

### 5. Build

**Windows:**

```sh
cmake --build build/windows-vs2022 --config Debug
```

**macOS:**

```sh
cmake --build --preset macos
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

---

## VS Code

Open the `glint_demo` folder in VS Code. Launch configurations and build tasks are already set up in `.vscode/`. Use the **Run and Debug** panel to build and launch the demo.

