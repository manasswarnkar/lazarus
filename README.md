# Lazarus

A C++23 game engine built from scratch, focused on learning industry-standard
engine architecture and coding patterns.

## Status

Phase 0 (Foundation) and Phase 1 (Platform & Windowing) complete. Phase 2
(Rendering Foundation) mostly complete — a textured, materialed, camera-driven
2D pipeline is working end to end. Mesh loading is the remaining Phase 2 item.

## Features

### Implemented

- **Logger** — Static, leveled logging (`Debug`, `Info`, `Warning`, `Error`).
  Zero dependencies; sits at the bottom of the engine's dependency graph.
- **Timer** — `Stopwatch` for one-off interval timing (profiling, load times)
  and `Time` for engine-wide per-frame delta time, total elapsed time, and
  frame counting. Built on `std::chrono::steady_clock` for monotonic timing.
- **Assert** — `ENGINE_ASSERT` (debug-only, compiled out under `NDEBUG`) and
  `ENGINE_VERIFY` (always evaluated, always checked) macros. Breaks into the
  debugger on failure via a platform-specific debug-break intrinsic. Fully
  decoupled from Logger to avoid circular dependencies — writes directly to
  `stderr`.
- **LinearAllocator** — Bump-pointer allocator for fast, contiguous scratch
  allocations with O(1) bulk reset. Alignment-aware; move-only.
- **Window** — RAII wrapper around a GLFW window and OpenGL 4.5 core context.
  Handles resize (framebuffer + window-size callbacks), correct HiDPI
  viewport sizing, and clean GLFW init/terminate across multiple windows.
- **Events** — Dispatcher-based event system (`Event`, `EventDispatcher`,
  category/type flags). `WindowResizeEvent`, `WindowCloseEvent`,
  `Key*Event`, `Mouse*Event` all flow through a single `EventCallbackFn`
  set on `Window`.
- **Input** — Static, polling-friendly key/mouse state (`Input::IsKeyPressed`,
  `Input::GetMousePosition`, etc.), populated by `Window`'s GLFW callbacks.
  No GLFW dependency of its own — pure cached state.
- **UUID** — 64-bit randomly-generated identifier (Mersenne Twister-backed),
  with `std::hash` support for use as a map/set key. Negligible collision
  risk for any realistic ID count within an engine session.
- **Random** — Seedable static RNG wrapper (`Float`, `Int`, `Bool`).
  Auto-seeded via `std::random_device` on first use; explicit `Seed()`
  for reproducible sequences (tests, deterministic generation/replay).
- **FileSystem** — Thin `std::filesystem` wrapper: whole-file read/write
  (text and binary), existence/directory checks, path component helpers.
  Read failures return `std::nullopt` rather than throwing — expected,
  recoverable conditions, not asserted.
- **Renderer** — A minimal OpenGL 4.5 core-profile rendering stack:
  - `VertexBuffer` / `IndexBuffer` / `BufferLayout` — RAII GPU buffer
    wrappers; layout math (stride/offset calculation) is pure and tested.
  - `VertexArray` — binds buffers + layout into a drawable GPU object.
  - `Shader` — compile/link from source or from `.vert`/`.frag` files on
    disk (via `FileSystem`); uniform-setting helpers (`SetInt`, `SetFloat4`,
    `SetMat4`).
  - `RenderCommand` — thin static wrapper isolating raw GL calls
    (clear, draw) from the rest of the renderer.
  - `Renderer` — `BeginScene` / `Submit` / `EndScene`; applies the active
    camera's view-projection matrix to each draw.
  - `Texture2D` — Image loading via `stb_image` (PNG/JPG/etc.), GPU
    upload with mipmaps, bindable to a texture unit slot.
  - `Material` — Bundles a `Shader` with texture bindings and a tint
    color uniform; re-applies all state every `Bind()` call so runtime
    tweaking (e.g. from a future editor) needs no extra plumbing.
- **OrthographicCamera** / **OrthographicCameraController** — 2D camera
  with position/rotation, view-projection matrix (tested). Controller
  drives it from `Input` (WASD movement, optional Q/E rotation) scaled by
  `Time::DeltaTime()` for frame-rate-independent motion.
- **Locked-aspect-ratio viewport** — `Window::LockAspectRatio` maintains a
  fixed rendering aspect ratio via letterboxing/pillarboxing on resize,
  rather than stretching or reflowing the camera's view. Viewport math
  (`Core::ComputeLetterboxedViewport`) is pure and fully tested.

### Planned

See the full [roadmap](#roadmap) below.

## Building

### Requirements

- CMake 3.20+
- A C++23 **and** C compiler (GCC or Clang recommended) — GLFW/GLAD are C
  libraries
- Git (for fetching dependencies via `FetchContent`: GoogleTest, GLFW, GLAD,
  glm, stb)
- On Linux: X11/Wayland development packages for GLFW (auto-detected by
  CMake at configure time)

### Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Run

```bash
./build/MyProject
```

### Run tests

```bash
./build/tests/EngineTests
```

or, via CTest:

```bash
ctest --test-dir build --output-on-failure
```

## Project structure

```
.
├── CMakeLists.txt              # Top-level build configuration
├── assets/
│   ├── shaders/                 # .vert / .frag source, loaded at runtime
│   └── textures/                 # Images loaded via Texture2D
├── include/                    # Third-party / vendored headers
├── src/
│   ├── Engine/
│   │   ├── CMakeLists.txt      # Engine static library target (links GLFW,
│   │   │                        # GLAD, glm, stb)
│   │   ├── Core/                # Foundational, dependency-free systems
│   │   │   ├── Logger.h / .cpp
│   │   │   ├── Timer.h / .cpp
│   │   │   ├── Assert.h
│   │   │   ├── LinearAllocator.h / .cpp
│   │   │   ├── ViewportMath.h / .cpp
│   │   │   ├── UUID.h / .cpp
│   │   │   ├── Random.h / .cpp
│   │   │   └── FileSystem.h / .cpp
│   │   ├── Platform/             # OS / windowing abstraction
│   │   │   └── Window.h / .cpp
│   │   ├── Events/                # Event types + dispatcher
│   │   │   ├── Event.h
│   │   │   ├── ApplicationEvent.h
│   │   │   ├── KeyEvent.h
│   │   │   └── MouseEvent.h
│   │   ├── Input/                 # Polling-based input state
│   │   │   ├── KeyCodes.h
│   │   │   ├── MouseCodes.h
│   │   │   └── Input.h / .cpp
│   │   └── Renderer/               # OpenGL rendering stack
│   │       ├── Buffer.h / .cpp
│   │       ├── VertexArray.h / .cpp
│   │       ├── Shader.h / .cpp
│   │       ├── Texture.h / .cpp
│   │       ├── Material.h / .cpp
│   │       ├── RenderCommand.h / .cpp
│   │       ├── Renderer.h / .cpp
│   │       ├── OrthographicCamera.h / .cpp
│   │       └── OrthographicCameraController.h / .cpp
│   └── Main.cpp                   # Application entry point
├── tests/
│   ├── CMakeLists.txt             # GoogleTest-based test suite
│   ├── TimerTest.cpp
│   ├── AssertTest.cpp
│   ├── LinearAllocatorTest.cpp
│   ├── EventTest.cpp
│   ├── InputTest.cpp
│   ├── BufferLayoutTest.cpp
│   ├── ViewportMathTest.cpp
│   ├── OrthographicCameraTest.cpp
│   ├── UUIDTest.cpp
│   ├── RandomTest.cpp
│   └── FileSystemTest.cpp
└── third_party/                   # Vendored / fetched dependencies
```

> `Material`, `Shader`, `Texture2D`, `VertexArray`, etc. depend on a live
> OpenGL context and are verified visually rather than unit tested — see
> [Testing](#testing).

## Design principles

- **Dependency direction matters.** Core systems (`Logger`, `Assert`) are
  kept free of dependencies on each other where possible, to avoid circular
  includes and keep foundational code usable from anywhere in the engine.
- **Static, industry-conventional API style.** Subsystems that are
  conceptually singletons (`Logger`, `Time`) expose static methods rather
  than requiring an instance; resource-owning types (`LinearAllocator`,
  `Window`) are RAII classes.
- **Every subsystem is unit tested** using GoogleTest, including edge cases
  like death tests for assertion failures.
- **Explicit over implicit** in CMake: source files are listed explicitly
  rather than globbed, to avoid stale build issues.
- **Renderable state stays plain and is re-applied every frame** (e.g.
  `Material::Bind()` re-uploads its tint/textures each call rather than
  once at creation). Costs little now and means a future editor can mutate
  a `Material`'s fields directly with no extra "apply changes" plumbing.

## Testing

Tests are written with [GoogleTest](https://github.com/google/googletest),
fetched automatically via CMake's `FetchContent` — no manual installation
required. Each subsystem has a corresponding `<Subsystem>Test.cpp` file in
`tests/`.

Pure-logic pieces of GL-dependent classes are pulled out and tested in
isolation where possible (e.g. `BufferLayout`'s stride/offset math,
`OrthographicCamera`'s matrix math, `Core::ComputeLetterboxedViewport`).
Classes that require a live OpenGL context to do anything meaningful
(`Shader`, `Texture2D`, `Material`, `VertexArray`) are verified manually /
visually instead, which is standard practice for this layer.

## Roadmap

High-level phased plan, roughly in dependency order:

- **Phase 0 — Foundation:** Logger ✅, Timer ✅, Assert ✅, LinearAllocator ✅,
  Config system (deferred)
- **Phase 1 — Platform & Windowing:** Window ✅, Events ✅, Input ✅
- **Phase 2 — Rendering Foundation (next):** buffer/shader wrapper classes,
  minimal `Renderer` (`BeginScene`/`Submit`/`EndScene`), first triangle,
  camera (orthographic first), textures, materials, mesh loading
- **Phase 3 — Core Architecture:** ECS, scene/world management, resource
  manager, serialization
- **Phase 4 — Gameplay Systems:** Scripting, physics, audio, animation, UI
- **Phase 5 — Tooling:** Editor, profiler, hot-reloading, build pipeline
- **Phase 6 — Polish & Performance:** Job system, multithreaded rendering,
  memory profiling

## License

See [LICENSE](LICENSE).