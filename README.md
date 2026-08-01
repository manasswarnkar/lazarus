# Lazarus

A C++23 game engine built from scratch, focused on learning industry-standard
engine architecture and coding patterns.

## Status

Actively in development. Core foundational systems are in place; windowing
and rendering are in progress.

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

### Planned

- **Window** — RAII wrapper around a GLFW window and OpenGL context (next up).

See the full [roadmap](#roadmap) below for everything after that.

## Building

### Requirements

- CMake 3.20+
- A C++23 compiler (GCC or Clang recommended)
- Git (for fetching dependencies via `FetchContent`)

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
├── include/                    # Third-party / vendored headers
├── src/
│   ├── Engine/
│   │   ├── CMakeLists.txt      # Engine static library target
│   │   └── Core/               # Foundational, dependency-free systems
│   │       ├── Logger.h / .cpp
│   │       ├── Timer.h / .cpp
│   │       ├── Assert.h
│   │       └── LinearAllocator.h / .cpp
│   └── Main.cpp                 # Application entry point
├── tests/
│   ├── CMakeLists.txt           # GoogleTest-based test suite
│   ├── TimerTest.cpp
│   ├── AssertTest.cpp
│   └── LinearAllocatorTest.cpp
└── third_party/                 # Vendored / fetched dependencies
```

> `Platform/Window` (GLFW-based windowing) is planned next and not yet
> present in the tree — see [Roadmap](#roadmap).

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

## Testing

Tests are written with [GoogleTest](https://github.com/google/googletest),
fetched automatically via CMake's `FetchContent` — no manual installation
required. Each subsystem has a corresponding `<Subsystem>Test.cpp` file in
`tests/`.

## Roadmap

High-level phased plan, roughly in dependency order:

- **Phase 0 — Foundation:** Logger ✅, Timer ✅, Assert ✅, LinearAllocator ✅,
  Config system
- **Phase 1 — Platform & Windowing:** Window (next), Input, Event system
- **Phase 2 — Rendering Foundation:** Graphics API abstraction, renderer
  core, shaders, buffers/textures, camera, mesh loading, materials
- **Phase 3 — Core Architecture:** ECS, scene/world management, resource
  manager, serialization
- **Phase 4 — Gameplay Systems:** Scripting, physics, audio, animation, UI
- **Phase 5 — Tooling:** Editor, profiler, hot-reloading, build pipeline
- **Phase 6 — Polish & Performance:** Job system, multithreaded rendering,
  memory profiling

## License

See [LICENSE](LICENSE).