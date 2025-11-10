# UE4-Dumper

Dump offsets, objects and functions for games built with Unreal Engine (UE4 / UE5).

---

## What this does
- Regenerates and dumps all strings using the Global Names pointer (GNames / FNames).  
- Dumps all objects present in the game using Global Names (GNames / FNames) and the Global Object (GUObjectArray).  
- Dumps offsets and function addresses using Global Names & Global Object or Global World (UWorld / GWorld).  
- Dumps Actors/entities present in the Global World to help verify pointer integrity.

Outputs are plain text dumps used as references for reverse engineering and research.

---

## Build (Visual Studio)
This project was developed with Microsoft Visual Studio Insiders. The steps below assume a Windows development environment.

1. Clone the repository or copy the source to a folder on your machine.
2. Open the solution file (`.sln`) in Visual Studio.
3. Select the correct build configuration and target architecture (x86 / x64) to match the target process/libs you will analyze.
4. Build the solution (`Build → Build Solution`).
5. The produced executable and output files will be placed in the project's output directory (typically `UE Dumper\x64\Release`) — see **Output** below.

---

## Offsets & configuration
- Every games require Global pointer/offset adjustments. The primary configuration file is `Offsets.h`.  
- If dumps do not look correct, inspect and update the pre-initialized offsets in `Offsets.h` to match the target binary layout.
- Changing the wrong offsets can lead to incorrect dumps or crashes — make incremental changes and test.

---

## Running & outputs
- Run the executable from anywhere.
- **Output files** (dumps) are written to the same folder as the executable by default.
- Inspect the generated `.txt` files to see GNames, objects, offsets and function addresses.

---

## Troubleshooting
- **No base address found / incorrect results**:  
  - Verify the package/name and library you expect the dumper to read.  
  - Confirm the architecture matches (x86 vs x64).  
  - Re-check offsets in `Offsets.h`.
- **Crashes during scanning**:  
  - Run in a debugger and check call stacks; invalid offsets/pointers are the most common cause.
- **Strings/objects missing**:  
  - Ensure the dumper is pointed at the correct process or memory image and that global pointers are valid.

---

## TODO
- Pattern scanning for automatic pointer discovery.  
- Auto offsets updater.  
- Internal SDK generator for easier downstream usage.

---

## AIDE / Mobile Notes
- This Visual Studio project is **not** directly compatible with AIDE or Android Studio.
- A Android-oriented dumper is maintained separately: `https://github.com/AscarreX/UE4-Mobile-Dumper`.

---

## Important legal & ethical notice
This project is provided for **research, education, debugging, and legitimate reverse-engineering** on software you own or where you have explicit permission. Do **not** use this tool to gain unfair advantages in multiplayer games, to distribute modified binaries, or to bypass any security, licensing, or anti-tamper protections. You are responsible for how you use the software.
