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

### 3. Init Skia

#### Option A — Prebuilt libraries (recommended, fast)

Download prebuilt Skia libraries — no Python, no lengthy compile. This only needs to be run once (or again if you switch backends).

```sh
# macOS — Metal (recommended)
node third_party/glint/scripts/init_skia.mjs --prebuilt --backend metal

# Windows — Direct3D 12 (recommended)
node third_party/glint/scripts/init_skia.mjs --prebuilt --backend d3d12
```

#### Option B — Build from source

If you need a custom configuration, you can build Skia from source instead. This takes significantly longer.

```sh
# macOS — Metal (recommended)
node third_party/glint/scripts/init_skia.mjs --source --backend metal --config Both

# Windows — Direct3D 12 (recommended)
node third_party/glint/scripts/init_skia.mjs --source --backend d3d12 --config Both
```

**Available backends by platform:**

| Backend | Platform | Description |
|---------|----------|-------------|
| `metal` | macOS | Metal (GPU) — recommended for macOS |
| `d3d12` | Windows | Direct3D 12 (GPU) — recommended for Windows |
| `opengl` | Windows | OpenGL (GPU) |
| `dawn` | macOS, Windows | Dawn / WebGPU (GPU) |
| `cpu` | macOS, Windows | Software CPU renderer |

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

---

## License

The **Glint engine** (`third_party/glint`) is a commercial product.

- Free for non-commercial use: https://superkraft.io/license-free
- Commercial use & terms: https://superkraft.io/terms
- Licensing inquiries: **hello@superkraft.io**

The demo code in this repository is provided solely for evaluation purposes.

