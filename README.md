# Macro Creator
A windows application used to create macros which are executed automatically.

[Download for Windows](https://www.david-weichselbaum.com/user/downloads/Macro%20Creator.exe)

Questions, suggestions or found a bug? Create a new [Issue](https://github.com/damr-/macro-creator/issues), pull request or contact me.

Contributions are always welcome.

# Changelog

## 1.1

- Added new command: "Apply Regex". Applies the given Regex to the currently selected text. The (first) result will be stored in the clipboard or automatically pasted if the user wants to.
- Added a "paste" option to the "Write Text" command. This method doesn't simulate keystrokes, but pastes each individual character. Using that approach, many more characters can be used.
- Changed the save file format for texts from raw to hexadecimal to allow more characters.
- Added the functionality to open '.mccr' files directly via right-click->"Open with..."->"Macro Creator"
- Scrolling through the command list using the mouse wheel no longer accidentally changes values/properties of commands. To change a value/property using the mouse wheel it has to be focused.
- Removed the "lock" feature since the issues with scrolling the mouse wheel have been resolved.
- Added the "add click" option to "Cursor Position" command
- Removed the "letter" option from the "Press Key" command, since it's already covered by "key".
