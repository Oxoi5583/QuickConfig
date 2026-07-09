# QuickConfig

QuickConfig is a small C++20 configuration library for loading, reading, updating, and saving simple `.qc` config files.

It was built as a practice and personal-development project, with a compact API around namespaced config values.

## Features

- Loads every `.qc` file from a config directory.
- Supports namespaced config sets.
- Supports number, string, and boolean values.
- Provides typed `get()` and `set()` overloads.
- Saves changed values back to the original config files.
- Allows `//` comments in config files.

## Project Layout

```text
.
├── include/
│   └── QuickConfig.h
├── src/
│   └── QuickConfig.cpp
├── test/
│   └── test.cpp
├── bin/
│   └── config/
│       ├── test_config.qc
│       └── test_config2.qc
├── CMakeLists.txt
├── compile.bat
└── run.bat
```

## Requirements

- CMake 3.30 or newer
- A C++20 compiler
- `ccache`, because the current `CMakeLists.txt` configures it as the compiler launcher

## Build and Run

On Windows, use the included batch script:

```bat
compile.bat
```

The script configures the project, builds it, and runs the test executable from `bin/test.exe`.

You can also run the steps manually:

```bat
cmake -S . -B build --fresh
cmake --build build
bin\test.exe
```

Build output is written to:

- `bin/` for the test executable
- `lib/` for the QuickConfig library

## Config File Format

QuickConfig reads files with the `.qc` extension. Each file starts with a namespace header:

```qc
[QuickConfig::<namespace>]
```

Config values use this format:

```qc
<type>@<key>::<value>
```

Supported types:

| Type | Meaning | C++ value type |
| --- | --- | --- |
| `n` | Number | `double` |
| `s` | String | `std::string` |
| `b` | Boolean | `bool` |

Example:

```qc
[QuickConfig::App]
n@window_width::1280
n@window_height::720
s@title::QuickConfig Demo
b@fullscreen::false
```

Comments are supported with `//`:

```qc
// This line is ignored
n@volume::0.8
```

Notes:

- A `.qc` file has one namespace header.
- The same namespace can appear in multiple `.qc` files.
- Keys must be unique inside the same namespace.
- Boolean values accept `true`, `false`, `t`, `f`, `yes`, `no`, `y`, and `n`.

## Basic Usage

Create a config directory, then place one or more `.qc` files inside it.

For example, if the executable runs from `bin/`, this project uses:

```text
bin/
└── config/
    └── test_config.qc
```

Include the header:

```cpp
#include <QuickConfig.h>
```

Load a config path:

```cpp
QuickConfig::QC_Path path;
path.go_to_child("config");

QC_Server->add_config_path(path);
```

Read values:

```cpp
std::string title;
double width;
bool fullscreen;

QC_Server->get("App", "title", title);
QC_Server->get("App", "window_width", width);
QC_Server->get("App", "fullscreen", fullscreen);
```

Update and save values:

```cpp
QC_Server->set("App", "title", "Updated Title");
QC_Server->set("App", "window_width", 1920);
QC_Server->set("App", "fullscreen", true);

QC_Server->save();
```

Reload configs:

```cpp
QC_Server->init();
QC_Server->add_config_path(path);
```

## API Overview

| API | Description |
| --- | --- |
| `QuickConfig::QC_Path()` | Creates a path at the current working directory. |
| `QuickConfig::QC_Path(path)` | Creates a path from the current working directory plus `path`. |
| `QC_Path::go_to_child(path)` | Moves the path into a child directory. |
| `QC_Path::go_to_parent()` | Moves the path to its parent directory. |
| `QC_Path::to_string()` | Returns the path as a string. |
| `QC_Server->add_config_path(path)` | Loads all `.qc` files in the given directory. |
| `QC_Server->get(namespace, key, value)` | Reads a value into a typed output variable. |
| `QC_Server->set(namespace, key, value)` | Updates a loaded value. |
| `QC_Server->get_str(namespace, key)` | Returns a config value as a string. |
| `QC_Server->save()` | Writes changed values back to the loaded config files. |
| `QC_Server->init()` | Clears loaded config state and file cache. |

`QC_Server` is a macro for:

```cpp
QuickConfig::QuickConfigServer::get_instance()
```

## Using QuickConfig in Another Project

Add `include/` to your include path and link against the built `QuickConfig` library.

With CMake, the local project already builds:

```cmake
add_library(QuickConfig src/QuickConfig.cpp)
target_include_directories(QuickConfig PUBLIC include)
```

Then link your executable to `QuickConfig`.

## Current Limitations

- Config loading scans only the given directory, not nested directories.
- `set()` can update existing keys only; it does not create new config entries.
- Type mismatches and invalid config formats terminate the program with an error.
- String values are trimmed, so leading and trailing spaces are not preserved.
- Because `//` starts a comment, string values should not contain raw `//`.

## Example

See [`test/test.cpp`](test/test.cpp) and the sample configs in [`bin/config/`](bin/config/).
