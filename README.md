# Hello GDExtensions
This repository contains samples and experiments using Godot 4 + GDExtension


## Articles
1. [Hello GDExtension](1.hello-c/README.md)


## Running the samples
For now, samples solely print their output to the console.

To run the samples, simply run Godot 4 in the root of this repository.
For example with Godot 4 in PATH with the name `godot4`:

```sh
# --quit makes Godot quit after the first frame
# --headless runs Godot in headless mode, so it doesn't render a window
godot4 --quit --headless 
```

You may also run `make run`, which does the same as the command before.
Set the variable `GODOT_BIN` to the path of the Godot 4 executable, if its name differs from `godot4`:

```sh
# Without the variable, GODOT_BIN defaults to `godot4`
make run
# You can pass the variable directly to make
make run GODOT_BIN=my-godot-4.exe
# Or define an environment variable
export GODOT_BIN=my-godot-4.exe
make run
```
