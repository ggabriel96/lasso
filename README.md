# lasso

A generic game loop implementation in C++ based on
[Fix Your Timestep!](https://gafferongames.com/post/fix_your_timestep/)

lasso is at early stages of design and development and is subject to
incompatible changes. All feedback is welcome and appreciated!

# Getting started

This project supports the [Meson](https://mesonbuild.com/) build system,
so you may want to install it (although it is not required). A legacy
[`CMakeLists.txt`](CMakeLists.txt) still exists, but is not
*actively* maintained.
A compiler with Concepts TS support (e.g. GCC 6 or above with options
`-std=c++2a` and `-fconcepts`) is required to use lasso. Then, having Meson
installed, run the following commands at the root of this project:

```sh
meson build
cd build && meson install
```

Alternatively, you may just put [`lasso.h`](include/lasso.h) somewhere in
the source tree of your project.

## Using lasso

The `meson install` command above will put (install) it at `/{prefix}/lasso/`
(relative to the default Meson
[paths](https://mesonbuild.com/Builtin-options.html)).
On Linux, that is `/usr/local/include/lasso/`.
Then you just need to have `/usr/local/include/` in your header search path
and you are ready to `#include <lasso/lasso.h>`.
Conversely, if you have somehow put [`lasso.h`](include/lasso.h) in your
source tree, you need to `#include "lasso.h"`.

lasso gains access to your game loop via the C++ concept below:

```cpp
template<typename T> concept bool GameLogic =
requires (T logic,
          LoopStatus const &status,
          high_resolution_duration const &time_delta) {
    { logic.init() } -> void;
    { logic.simulate(status, time_delta) } -> void;
    { logic.render(status, time_delta) } -> void;
    { logic.is_done() } noexcept -> bool;
    { logic.terminate() } -> void;
};
```

That means that you need a `class` or `struct` that implements the
following member functions:

- `void init();`, which is called once right before the loop starts and you
  can use it to initialize anything you need (beyond the constructor of your
  class);
- `void simulate(LoopStatus const &, high_resolution_duration const &);`,
  which is called (possibly many times, see below) in every iteration to
  advance your simulation (input, physics, AI, etc.) since the last call;
- `void render(LoopStatus const &, high_resolution_duration const &);`,
  which is called once in every iteration to render what has been simulated;
- `bool is_done() noexcept;`, which is called once in every iteration to
  determine whether the loop should terminate;
- `void terminate();`, which is called once right after the loop ends and you
  can use it to clean up anything you need (additionally to what will be
  done in the destructor of your class).

Additional member functions may be added in the future.
Examples of classes implementing this concept can be seen in the
[examples folder](example/).

After having implemented your class (let us call it `T`),
running it is as simple as:

```cpp
T t;
# ...
lasso::MainLoop{}.run(t); # or pass T{} directly
```

The constructor of `lasso::MainLoop` has two parameters:

- `unsigned int simulations_per_second`, which determines how many times
  `simulate()` may be called in a second by setting the **fixed simulation
  delta time** (defaults to 30);
- `float fps_smoothing`, which determines how aggressively FPS count changes
  should be smoothed (between 0.0 and 1.0, defaults to 0.75).

## Building the examples

The [Simple and Fast Multimedia Library (SFML)](https://github.com/SFML/SFML)
must be installed to compile and run an [example](example/).
If you want to enable the compilation of the examples, you may run:

```sh
meson build -Dexample=true
cd build && ninja
```

or, after having initially run Meson and while at the `build/` directory:

```sh
meson configure -Dexample=true
ninja
```

In order for an example to locate [`Roboto-Regular.ttf`](example/res/font/) and
hence render text, you must run the compiled executable from either the build
or the [`example/`](example/) directory (or set any of them as the working
directory in your IDE; preferably the build one).

# Third-party

This project thankfully utilizes the third-party resources listed below.
Their licenses are located in the directory [`licenses/`](licenses/).

- [Simple and Fast Multimedia Library (SFML)](https://github.com/SFML/SFML)
- [Google's Roboto Font Family](https://github.com/google/roboto/)