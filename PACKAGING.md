# Packaging Instructions for Windows Transfer

Since you're developing in an Ubuntu container but need to run the tool on Windows, follow these steps to package and transfer your files properly:

## 1. Create a Windows-Compatible Package

Run these commands in your Ubuntu terminal:

```bash
# Create a directory for the package
mkdir -p hideprocess_win

# Copy all necessary files
cp win_process_hider.c win_injector.c build.bat hide_process.bat hideproc.bat installer.bat check_process.bat SETUP_GUIDE.md DEMO_GUIDE.md README.md hideprocess_win/

# Ensure files have Windows line endings (CRLF)
apt-get update && apt-get install -y dos2unix
find hideprocess_win -type f -name "*.bat" -exec dos2unix -f {} \;
find hideprocess_win -type f -name "*.md" -exec dos2unix -f {} \;

# Create ZIP archive
apt-get install -y zip
zip -r hideprocess_windows.zip hideprocess_win/

# Clean up
rm -rf hideprocess_win
```

## 2. Transfer to Windows

Now transfer `hideprocess_windows.zip` to your Windows system using one of these methods:

- Download via browser if your dev container has a web interface
- Use git to push to a repository and clone on Windows
- Use a file sharing service
- Use SSH/SCP if available

## 3. On Windows

1. Extract the ZIP file
2. Open a Command Prompt as Administrator
3. Navigate to the extracted directory:
   ```cmd
   cd path\to\extracted\hideprocess_win
   ```
4. Either:
   - Run the installer: `installer.bat`
   - Or follow the steps in SETUP_GUIDE.md

## Troubleshooting

If you encounter errors like "not was unexpected at this time" on Windows:

1. Make sure your batch files have proper Windows line endings (CRLF)
2. Check for any invisible Unicode characters
3. Try opening each batch file in Notepad, saving again to ensure Windows compatibility

The packaging steps above should fix most Windows compatibility issues.
