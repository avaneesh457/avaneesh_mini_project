# 2D ASCII Graphics Editor

A lightweight, menu-driven 2D vector graphics editor that renders output onto an ASCII character grid (`80` columns by `25` rows) using `*` for drawing elements and `_` for the background.

This repository contains two equivalent implementations of the editor:
1. **C Language Version (`main.c`)**: High-performance, portable standard C code designed to compile and run natively on any platform.
2. **PowerShell Version (`editor.ps1`)**: Script equivalent designed to run immediately in a Windows PowerShell console window without requiring any compilers or build tools.

---

## Features

- **Vector-style Shape Database**: Rather than drawing directly to the pixels (which makes shapes impossible to remove), the program maintains an internal list of active shapes. Removing an object clears the grid and re-renders the remaining shapes cleanly.
- **Display on Demand**: The grid coordinates and rendering canvas are only printed when you choose option `1` (Display Picture). Adding or deleting objects outputs a clean success feedback message and keeps the terminal clean.
- **Advanced Rasterization Algorithms**:
  - **Lines**: Uses **Bresenham's Line Algorithm** to accurately plot pixels for any slope using integer-only math.
  - **Circles**: Uses the **Midpoint Circle Algorithm** to render perfect rings around a center point.
  - **Rectangles**: Renders rectangular boundaries with precise dimension guides.
  - **Triangles**: Draws three interconnected lines between three vertices.
- **Grid Coordinate Visualizer**: The canvas is surrounded by a coordinate ruler (0–79 on the top, 0–24 on the side) to make plotting easy.
- **Robust Input Handling**: Custom integer parser that prevents console crashes, memory overflows, or infinite loops if characters are typed into numeric prompts.

---

## File Structure

- **`main.c`**: The C language source code implementing the editor menus, shapes list, and rendering logic.
- **`editor.ps1`**: A native Windows PowerShell implementation of the exact same menu loop and drawing algorithms.
- **`README.md`**: This guide.

---

## How to Run the PowerShell Version (Instant Terminal Run)

If you do not have a C compiler installed on Windows, you can run the editor immediately using the PowerShell version:

1. Open PowerShell.
2. Navigate to this directory:
   ```powershell
   cd C:\Users\playz\.gemini\antigravity-ide\scratch\graphics_editor
   ```
3. Run the script:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\editor.ps1
   ```

---

## How to Run the C Version (main.c)

To build and run the C version, you will need a C compiler (like `clang` or `gcc`) installed.

### Step 1: Install a Compiler
If you don't have one, run the following in PowerShell:
```powershell
winget install LLVM.LLVM
```
*(Make sure to restart your IDE or terminal window after the installation finishes so it registers `clang`).*

### Step 2: Compile
Run this command to compile the C code:
```powershell
clang -o editor.exe main.c
```

### Step 3: Run
Run the executable in your terminal:
```powershell
.\editor.exe
```

---

## Menu Options

1. **Display Picture**: Renders the active shapes on the grid and shows the current picture.
2. **Add a Line**: Prompts for start `(x1, y1)` and end `(x2, y2)` coordinates.
3. **Add a Rectangle**: Prompts for top-left corner `(x, y)`, width, and height.
4. **Add a Circle**: Prompts for center `(cx, cy)` and radius.
5. **Add a Triangle**: Prompts for three vertex coordinates.
6. **Delete an Object**: Lists all active shapes with coordinate details and allows you to select one by number to delete.
7. **Clear Canvas**: Removes all shapes.
8. **Exit**: Terminates the program.
# avaneesh_mini_project
