# Basic Kernel and System Monitor
A basic kernel with a system monitor. Uses a LCD to display kernel and tasks information.

## Prerequisites
Before you begin, ensure you have the following software installed and accessible from your command line (i.e., in your system's PATH).

 1. GNU Make: A tool for automating the build process.

	- Linux/macOS: Usually pre-installed. You can check by typing make --version.
	- Windows: Can be installed via tools like Chocolatey, MSYS2, or by using the Windows Subsystem for Linux (WSL).
	
 2. Arm GNU Toolchain: This provides the cross-compiler, linker, and other tools needed to build firmware for Arm processors.

	- Download from: Arm GNU Toolchain Downloads
	- After installation, make sure the toolchain's bin directory is added to your system's PATH. You can verify this by running arm-none-eabi-gcc --version.

## Building the Firmware
Once the prerequisites are installed, you can build the project by running make from the root directory of this repository.

 1. Clone the repository:

```bash
	git clone <your-repository-url>
	cd <your-repository-directory>
```

 2. Build the project:
	This command will compile the source files and generate the final firmware.bin.

```bash
	make
```

## Cleaning Up
To remove all generated files (object files, ELF files, and the final binary), run the clean command.

```bash
	make clean
```