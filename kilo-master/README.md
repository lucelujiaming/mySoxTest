Kilo
===

Kilo is a small text editor in less than 1K lines of code (counted with cloc).

A screencast is available here: https://asciinema.org/a/90r2i9bq8po03nazhqtsifksb

Usage: kilo `<filename>`

Support files: java, c, c++, python

Keys:
    CTRL-B: Move backward by word(New Add)
    CTRL-C: Copy current word(New Add)
    CTRL-D: remove word(New Add)
    CTRL-G: Jump to end of file(New Add)
    CTRL-F: Find string in file (ESC to exit search, arrows to navigate)
    CTRL-J: Merge current line to previous line(New Add)
    CTRL-L: Copy current line(New Add)
    CTRL-N: Go to line(New Add)
    CTRL-S: Save
    CTRL-U: Switch uppercase and lowercase(Not implement yet)
    CTRL-V: Paste word or line(New Add)
    CTRL-W: Move forward by word
    CTRL-X: switch word(Not implement yet)
    CTRL-Y: Redo(New Add)
    CTRL-Z: Undo(New Add)
    CTRL-^: Go to start of line(New Add)
    CTRL-$: Go to end of line(New Add)
    CTRL-Q: Quit

NOTICE: kilo has the refcount error in E.dirty, I try to fix it in 09/23/25 and check later.
        kilo has a bug in horizonal scroll, I fix it in 09/23/25.
        Maybe I could try to add a function of line number.

Kilo does not depend on any library (not even curses). It uses fairly standard
VT100 (and similar terminals) escape sequences. The project is in alpha
stage and was written in just a few hours taking code from my other two
projects, load81 and linenoise.

People are encouraged to use it as a starting point to write other editors
or command line interfaces that are more advanced than the usual REPL
style CLI.

Kilo was written by Salvatore Sanfilippo aka antirez and is released
under the BSD 2 clause license.

