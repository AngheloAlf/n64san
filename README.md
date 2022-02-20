# N64 San

Sanitizer runtime libraries for N64 decompiled games

Most of the sanitizer specific code has been ported/adapted from the [llvm-project repository](https://github.com/llvm/llvm-project/tree/main/compiler-rt).

## Supported sanitizers

Currently only a subset of the Undefined Behaviour Sanitizer (ubsan) runtime has been successfully ported.

For more info see the [N64 UBSan readme](#src/n64_ubsan/README.md).

## N64Wrapper

N64Wrapper is a small library designed to abstract away interactions from the sanitizers to any game-specific required function, like I/O functions.

For more info see the [N64Wrapper readme](#src/n64_wrapper/README.md).

## Building

Currently only building on Linux has been tested.

### Dependencies

The build process has the following package requirements:

- build-essential
- binutils-mips-linux-gnu
- gcc-mips-linux-gnu

Under Debian / Ubuntu distros you can install them with the following commands:

```sh
sudo apt update
sudo apt install build-essential binutils-mips-linux-gnu gcc-mips-linux-gnu
```

Other distrubutions may work but they has not been tested.
Other gcc-based compilers may work but they has not been tested.
The clang suite may work but it has not been tested.

### Build

After the repo has been cloned and and the dependencies are installed, simply run

```sh
make
```

This will compile every currently supported sanitizer runtime and the N64Wrapper library into `.a` files that can be linked with a N64 decompiled game which has been rebuilt with sanitizers enabled.

<!-- TODO: document current Makefile targets and variables -->

## Usage

Now you can rebuild your game enabling your desired sanitizers and link the compiled runtime libraries and the N64Wrapper library (probably by adding them to your linker script). Linking this library to the `boot` segment of your game is recommended.

The produced libraries should be a drop-in solution, without requiring to modify the original codebase, but there can be some exceptions. See [Usage considerations](#usage-considerations).

If you are getting "undefined reference to `__ubsan_`... (or any other supported sanitizer)" errors, you may need to add the `--whole-archive` flag to your linker invocation.

Note some sanitizers are not compatible with each other. Please consult the documentation of your compiler.

### Usage considerations

IMPORTANT: When compiling your game with any sanitizer option enabled, the compiler will produce a lot of extra instructions (that's where the runtime checks come from) which may slow down the game's runtime and will increase the ROM size, probably shifting the final built ROM.

If multiples sanitizer options are enabled when building many files, the size of the produced game may increase to the point the game may run out of RAM during execution, leading to missing entities, crashing during normal play or crashing during the boot sequence, among other problems. Other not RAM-related problems may arise too. It may be possible to circumvent those problems by modifying the game's codebase, like for example increasing the heap, but this is not guaranteed to always work.

It is recommended to not use every possible sanitizer option available at the same time and/or not sanitize the whole codebase of your game.
