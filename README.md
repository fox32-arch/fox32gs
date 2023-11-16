# fox32gs

![fox32 logo](docs/logos/fox32-circle.png)  
(logo by [ZenithNeko](https://zencorner.xyz/contacts.html))

**fox32gs** is a fantasy game console based around the [fox32](https://github.com/fox32-arch/fox32) architecture.

## Getting Started

**Note: This software is still very much in an early stage, and is currently more focused towards developers rather than end-users.**

Prebuilt Linux binaries of the latest commit are available on the [GitHub Actions page](https://github.com/fox32-arch/fox32gs/actions).

### Building

Simply run `make`. The resulting binary will be saved as `fox32gs`. Optionally you may build for a different target with `make TARGET=<target>`, see the Makefile for details.

### Usage

The following arguments are valid:
- `--cd <file>`: mount the specified file as a CD-ROM
- `--card <file>`: specify a memory card image to use
- `--rom <file>`: use the specified file as the boot ROM. if this argument is not specified then the embedded copy of **libgs** is used
- `--debug`: print a disassembly of each instruction as it runs

## License
This project is licensed under the [MIT license](LICENSE).
