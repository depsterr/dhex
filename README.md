# dhex - depsterr hex

A simple hex "editor"

## Installation and usage

#### Installation

```shell
git clone https://github.com/depsterr/dhex.git
cd dhex
sudo make install
```

#### Usage

Basic usage

```
dhex <file> [ options ]
```

flags:

```
-h  --help      displays this message

-n  --new       creates a new file instead of editing an existing one

-o  --output    will output to a filename corresponding to the next arg

-c  --columns   changes the amount of columns per row in the hexfile to the next arg

-e  --editor    will open the hexfile with the command given as the next arg

-v  --verbose   outputs progress while writing files, slows down the process a bit
```
