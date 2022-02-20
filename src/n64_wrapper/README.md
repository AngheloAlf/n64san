# N64 Wrapper

Game's abstractions library for sanitizers compatibility

This library provide a simple and opaque way to call any required functionality from a game, without worrying to convert the sanitizer code into an ifdef spaghetti.

## How it works

`N64Wrapper_` function provides implementations of functions declared on the `include/n64_wrapper/` headers, either by calling an internal implementation from the corresponding game (usually in I/O functions), or by implemented that functionallity in the function itself.

## Suported games

Currently only [Ocarina of Time](https://github.com/zeldaret/oot/) is supported out of the box.

[Majora's Mask](https://github.com/zeldaret/oot/) compatibility is planned for a future update (there's no planned date).

There are no plans on me adding support for other games, but I'm open to Pull requests adding support for other games.
