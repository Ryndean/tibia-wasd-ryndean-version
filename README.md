# Tibia WASD Movement (Ryndean Version)

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

### **Option 1: Visual Studio Code (Recommended for Windows)**
This is the easiest method if you are using VS Code:
1. Open the project folder in VS Code.
2. Ensure you have **MinGW-w64** (g++) installed and added to your Windows PATH.
3. Press **`Ctrl + Shift + B`**.
4. The `ddraw.dll` will be generated automatically in the root folder.

### **Option 2: Using the Build Script (CLI / Linux / Automation)**
If you prefer the command line or are cross-compiling from Linux:
1. Open your terminal (Git Bash is recommended on Windows).
2. Run the following command:
   ```bash
   ./build.sh
3. The script will verify your compiler and link the necessary Windows libraries to create the ddraw.dll.

## 🔧 Technical Methodology
This project utilizes DLL Proxy Loading. By naming our library ddraw.dll, the Tibia client loads our code instead of the system's default DirectDraw library. We then hook the CreateWindowEx and the game's message dispatcher to intercept movement keys before they reach the chat input.

Created and maintained by Ryndean.
