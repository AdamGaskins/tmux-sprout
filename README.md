# Tmux Sprout

<div align="center">
    <img width="720px" src="assets/screenshot.png" />
</div>

A simple C script to switch and hydrate your tmux sessions. Inspired by [Primeagen's tmux-sessionizer](https://github.com/ThePrimeagen/tmux-sessionizer). Encapsulates all the functionality of creating sessions, attaching if they exist, etc. so your `tmux-sessionizer` essentially just becomes where you specify what folders you want to include:

```sh
selected=$(find ~/path1 ~/path2/path3 -mindepth 1 -maxdepth 1 -type d | fzf)

if [[ -z $selected ]]; then
    exit 0
fi

tmux-sprout "$selected"
```

## How it works

When you call `tmux-sprout [path]`, a tmux session will be opened to the specified path. If the session doesn't already exist it will be created, otherwise it will be open.

Then, by writing a simple config file, you can tell tmux-sprout how to set up the session when it is created. This saves me a few seconds from needing to open neovim, running `npm run dev`, and then opening another shell tab to use for git.

## Setup

No binaries are provided. Compiling with [Meson](https://mesonbuild.com/SimpleStart.html) is very easy.

Setup:
```sh
git clone https://github.com/AdamGaskins/tmux-sprout.git tmux-sprout
cd tmux-sprout
meson setup buildDir
```

Compile:
```sh
meson compile -C buildDir
```

Run:
```sh
./buildDir/tmux-sprout
```

Link:
```sh
sudo ln -sf $(realpath ./buildDir/tmux-sprout) /usr/local/bin/tmux-sprout
```

## Configuration Example

Configuration is looked for at:
* `$XDG_CONFIG_DIR\tmux-sprout.toml`
* `$HOME\tmux-sprout.toml`
* `tmux-sprout.toml`

```toml
# Create a new entry in the projects array for each project
[[projects]]
name = "Dotfiles"                   # What the TMUX session should be named
folder = "/Users/adam/.dotfiles"    # the full path to the folder
tabs.dotfiles = "nvim"              # tabs, and the commands to initialize them
tabs.shell = ""                     # blank to run no command

[[projects]]
name = "TMUX Sprout"
folder = "lang/tmux-sprout"         # if a partial path is specified, it will match the end (e.g. a folder named "tmux-sprout" in a folder named "lang") 
tabs.nvim = "nvim"
tabs.shell = ""
```
