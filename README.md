RAF Manger
==========

`RA File Manger` is a console file manager developed in C++ for the Linux operating system for file and directory management. `RAF` has a simple interface that allows users to easily navigate through file system.


## Screenshots

![RAF Kitty](https://i.imgur.com/DIE7EBt.png)
![RAF GNOME](https://i.imgur.com/3xvylif.png)

## Usage/Examples

```
NOTE:

In the shortcuts below, "C" stands for CTRL and "A" stands for "ALT".

----------------------------

- File/directory operations
  C-r                           Rename file under cursor

- Navigation
  TAB                           Change your current panel

  C-s                           Search for item

  Up Arrow                      Move selection to the previous file
  Down Arrow                    Move selection to the next file
```


## Installation

### Manual

1. Download `RAF`:
  ```bash
    git clone git@github.com:primovocatus/FileManager.git
  ```
2. Change directory:
  ```bash
   cd FileManager/
  ```
4. Build a project using cmake:
  ```bash
    cmake .
    cmake --build .

    ./main
  ```
    
## License

RAF is made by Andrew Korobko ([@primovocatus](https://github.com/primovocatus)) and it's released under [MIT](https://github.com/primovocatus/FileManager/blob/main/LICENSE) license.
