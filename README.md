# lasso
A generic game loop implementation in C++ based on
[Fix Your Timestep!](https://gafferongames.com/post/fix_your_timestep/)

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

The `meson install` command above will put (install) it at `/{prefix}/lasso/`
(relative to the default Meson
[paths](https://mesonbuild.com/Builtin-options.html)).
On Linux, that is `/usr/local/include/lasso/`.
Then you just need to have `/usr/local/include/` in your header search path
and you are ready to `#include <lasso/lasso.h>`.
Alternatively, you may just put [`lasso.h`](include/lasso.h) somewhere in
the source tree of your project.

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