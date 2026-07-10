# Requirements
- Yazi
- [Steam API key](https://steamcommunity.com/dev/apikey)

# What is this?
This is a tool to help you identify which proton prefix is which, using symlinks to create folders with the game's name.

# Features
- For Steam games, the program sends a request to Steam in order to retrieve the game name, making the process for these games automatic.
- For non-Steam games, contents of popular gamesave folders are shown, such as all of AppData and Documents. If the user still can't identify the game, yazi will open to let them navigate the prefix freely.

# Running
1. [Download latest release](https://github.com/Marks20125/ProtonPrefixes/releases/latest/download/ProtonPrefixes)
2. ```shell
   chmod +x ProtonPrefixes
   ```
3. ```shell
   ./ProtonPrefixes
   ```

# Building
## Requirements
- CMake
- Curl
- Libpsl
- Ninja

```shell
git clone https://github.com/Marks12520/ProtonPrefixes.git --recursive
cd ProtonPrefixes
cmake -B build -S . -G Ninja
cmake --build build/
```

Binary should be inside `build/`
