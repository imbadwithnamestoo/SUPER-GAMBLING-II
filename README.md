# Super Gambling II

A terminal-based blackjack game where you owe the house 100 grand.

## Building

Requires a C++11 compiler.

```
g++ -std=c++11 -o SuperGambling2 main.cpp src/*.cpp
```

Run the game:

```
./SuperGambling2
```

You can also download the pre-compiled binaries within releases.

## Controls

| Key | Action |
|-----|--------|
| Arrow Keys | Navigate menus |
| Enter | Select / confirm |

## Save File

Your progress is stored in `save.txt` in the working directory. Delete it to reset your progress. The save file is encrypted to discourage tampering.
