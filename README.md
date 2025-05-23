# Tmux Sprout

Setup:
```sh
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
