# Tibia 7.72 WASD Movement (Ryndean Version)

This is an enhanced fork of the `tibia-wsad` repo. It allows for a more modern control scheme in Tibia by using **WASD** for movement and handling the chat focus automatically.

## ✨ Key Enhancements
* **Diagonal Movement:** Full support for **Q, E, Z, and C** keys for diagonal steps.
* **Smart NumLock Logic:** The script automatically forces **NumLock OFF** when activated to ensure diagonal keys function correctly without manual setup.
* **Toggle Shortcut:** Toggle the entire script on/off instantly with `CTRL + TAB`.

---

## 📥 Installation (The Easy Way)
1. Go to the [Releases](https://github.com/Ryndean/tibia-wasd-ryndean-version/releases) page.
2. Download the `ddraw.dll`.
3. Place the file into your Tibia folder (where `Tibia.exe` is located).
4. Launch the game.

> **Important:** The file must remain named `ddraw.dll` to be loaded by the game client.

---

## 🛠 Building from Source
You can compile this project using two different methods depending on your preference.

### **Option 1: Visual Studio Code (MSVC Method)**
1. Close VS Code if it is open.
2. Open the **"x86 Native Tools Command Prompt for VS 2022"** from the Start Menu.
3. Navigate to your project folder and type `code .` to launch VS Code.
4. **Open `dllmain.cpp` in the editor** (click it in the file explorer on the left).
5. Press **`Ctrl + Shift + B`**.
6. The `ddraw.dll` will be generated in your project folder.

### **Option 2: Using the Build Script (Linux / MinGW)**
1. Requirements: Ensure you have the i686-w64-mingw32-g++ cross-compiler installed.
   - Ubuntu/Debian: sudo apt update && sudo apt install g++-mingw-w64-i686
2. Open your terminal (or Git Bash).
3. Make the script executable: chmod +x build.sh
4. Run the build: ./build.sh

## 🔧 Technical Methodology
This project utilizes DLL Proxy Loading. By naming our library ddraw.dll, the Tibia client loads our code instead of the system's default DirectDraw library. We then hook the CreateWindowEx and the game's message dispatcher to intercept movement keys before they reach the chat input.

Created and maintained by Ryndean.
