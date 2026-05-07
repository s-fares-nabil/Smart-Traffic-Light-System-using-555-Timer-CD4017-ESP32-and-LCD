Libraries Directory
===================

This directory is for storing any libraries that are not available through the standard Library Manager (Arduino IDE or PlatformIO).

For this project, the main required library is:
- LiquidCrystal_I2C (by Frank de Brabander)

This library should be installed via:
1. Arduino IDE: Sketch → Include Library → Manage Libraries...
2. Search for "LiquidCrystal I2C"
3. Install by Frank de Brabander
4. Or via PlatformIO: pio lib install "LiquidCrystal I2C"

If you need to manually add a library version or use a forked version, place it in this directory.

Library Structure:
Each library should be in its own subdirectory, for example:
lib/
└── LiquidCrystal_I2C/
    ├── LiquidCrystal_I2C.h
    ├── LiquidCrystal_I2C.cpp
    ├── keywords.txt
    └── examples/
        └── ... (example files)

To use a library from this directory:
1. Ensure the library folder name matches the #include statement
2. The Arduino IDE will automatically detect libraries in the lib/ folder
3. Or manually add the path in your IDE settings

Note: For most users, installing via Library Manager is recommended and simpler.