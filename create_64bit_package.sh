#!/bin/bash
# Create a complete 64-bit package for Windows 11

echo "========================================================"
echo "Creating Complete Windows 11 64-bit Process Hiding Package"
echo "========================================================"
echo

# Make the script executable
chmod +x package_win11_64bit.sh

# Create the complete package
echo "Creating the package..."
./package_win11_64bit.sh

# Display instructions
echo
echo "========================================================"
echo "TRANSFER INSTRUCTIONS:"
echo "========================================================"
echo
echo "1. Download the ZIP file: windows11_64bit_fixed.zip"
echo "2. Transfer it to your Windows 11 system"
echo "3. Extract all files to a folder"
echo "4. On Windows 11, follow these steps:"
echo "   a. Open 'x64 Native Tools Command Prompt for VS 2022' as Administrator"
echo "   b. Navigate to the extracted folder"
echo "   c. Run: INSTALL.bat"
echo "   d. After installation, run: run_tool_x64.bat notepad.exe"
echo
echo "This complete package has everything needed to work on Windows 11 64-bit systems."
echo "It includes proper verification and error checking to prevent the 16-bit application error."
echo
echo "Opening browser to help you download the package..."

# Open browser to help download
"$BROWSER" "file://$(pwd)"

echo
echo "Download windows11_64bit_fixed.zip to transfer to your Windows system."
