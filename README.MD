# mousemode

WORK IN PROGRESS

## Description

A dead-simple, reliable keyboard interface for moving the mouse cursor and pressing buttons. When the program
is executed, mousemode will:

1. lock most of the keyboard
2. allow you to use vim or arrow keys to move the mouse cursor
3. holding "v" will greatly decrease cursor speed
4. holding "f" will double the speed (can be combined with "v")
5. [a,s,d] keys correspond to left, middle and right mouse buttons
6. [e, y] keys correspond to mouse wheel down and up rolls
6. escape or "q" exits the program
7. key-combos using the super key can still be used (so that you can still use system-wide shortcuts that you may have)

Mousemode is aimed at laptop power users that are annoyed with touchpads or people that in general find themselves
in a position uncomfortable to be using the mouse via conventional means. The "f" and "v" keybindings feel a bit
strange at first, but I wanted to avoid standard modifiers like shift and control, because some programs have
shortcuts with those (for example vim, if you enable the mouse feature, selects differently when you hold shift).

---

Mousemode does NOT use Xlib events. In fact, by nature it does not distinguish between key presses and releases.
Instead, it reads the keyboard state 60 times a second and emulate mouse clicks/movement accordingly. This
synchronous nature allows it to be very simple and reliable (only about 120 lines of code).

## Building

You should only need Xlib, which is widely available on virtually all linux distributions. You very likely
already have it installed!

Run the following (if necessary, as root):

    make install

The resulting binary will be installed at `/usr/local/bin/mousemode`.
