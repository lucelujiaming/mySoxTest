Kilo
===

Kilo is a small text editor in less than 1K lines of code (counted with cloc).

A screencast is available here: https://asciinema.org/a/90r2i9bq8po03nazhqtsifksb

Usage: kilo `<filename>`

Keys:
    CTRL-C: Copy current word(New Add)
    CTRL-G: Jump to end of file(New Add)
    CTRL-F: Find string in file (ESC to exit search, arrows to navigate)
    CTRL-J: Merge current line to previous line(New Add)
    CTRL-L: Copy current line(New Add)
    CTRL-N: Go to line(New Add)
    CTRL-S: Save
    CTRL-V: Paste word or line(New Add)
    CTRL-Y: Redo(Not implement)(New Add)
    CTRL-Z: Undo(New Add)
    CTRL-Q: Quit

Kilo does not depend on any library (not even curses). It uses fairly standard
VT100 (and similar terminals) escape sequences. The project is in alpha
stage and was written in just a few hours taking code from my other two
projects, load81 and linenoise.

People are encouraged to use it as a starting point to write other editors
or command line interfaces that are more advanced than the usual REPL
style CLI.

Kilo was written by Salvatore Sanfilippo aka antirez and is released
under the BSD 2 clause license.
