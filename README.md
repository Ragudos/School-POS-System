# School-POS-System
A school project as of 10/09/2024 creating a point-of-sale system in C++

## Instructions

For my fellow groupmates, please refer to [GUIDE.md](GUIDE.md) to setup your editor and start contributing to the source code.

After knowing how to contribute, please install these extensions on VsCode:

1. Clang-Format
2. C/C++
3. Makefile Tools

Then, to automatically format the file you're working on, follow these steps if you are using VsCode:

1. Click `Ctrl` + `Shift` + `p`
2. A command line should appear, type "User Settings".
3. Click on User Preferences (JSON)
4. On the JSON, add this:

```json
{
    "editor.formatOnSave": true
}
```

## Testing the project locally

**Prerequisites**
1. g++ or gcc compiler
2. make (if you don't have it yet and you have MSYS2 already, just run these on MSYS2 terminal)
```bash
pacman -Syu   # Update the package database
pacman -Su    # Update installed packages
pacman -S make
```
3. If you don't have MSYS2, you can go to their [website](https://www.msys2.org/) to download it

Now that you have `make`, you can start compiling the project by running this on your code editor's terminal:

```bash
make
```

This will create a `bin` folder. To run the project, run:

```bash
./bin/main.exe
```

To run the tests:

```bash
./bin/main_test.exe
```

## Resources

- [ASCII Codes for User Input](https://www.ascii-code.com/)
- [ANSI Codes for controlling terminal](https://en.wikipedia.org/wiki/ANSI_escape_code)
