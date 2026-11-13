Make sure that your terminal supports VT100 ansi codes!.   
   ____        _   _                 ____ 
  / __ \      | | (_)               / /_ |
 | |  | |_ __ | |_ _  ___  _ __    / / | |
 | |  | | '_ \| __| |/ _ \| '_ \  / /  | |
 | |__| | |_) | |_| | (_) | | | |/ /   | |
  \____/| .__/ \__|_|\___/|_| |_/_/    |_|
        | |                               
        |_|                               
Option/1 from Memory Space --- Simple text editor by Filip

This uses command line input.
The command line can store values in a stack.
1 -5 "hello"
Will put the following on the stack:
     (1 -5 "hello")
The stack is typed (signed int/string).

In forth tradition the stack is represented inside ().
For notation on changes being done to the stack:
     ( A -- B )
Where A is the stack before and B is the stack after.
For example: dup could be described with ( a -- a a).

Commands:
undo
redo
spool
     Spools a print job to the print queue.
print
     Prints document from the print queue.
show
     Show document.
(n -- ) add
     Adds a line before line n (0 is the first line).
     If n=-1 the line is added at the end.
     For simplicity you get a prompt after the command runs to write the new line.
     (No text is taken from the stack.)
(n -- ) remove
     Remove line n from the document.
(n -- ) load
     Load file with the filename n.

These commands are interpreted with a string search tree.
Each node has an array with one index for each printable ASCII char.
Said tree will try to autocomplete words in asciibetical order:
`sh` will resolve to `show`, and `re` to `redo` NOT `remove` (but `rem`=`remove`).

Examples:
------------
6 add s
Hello world!
------------
Adds 'Hello world!' before line 6 and displays the document.

-----------------------------------
"/home/filip/.ssh/config" load show
-----------------------------------
Load and show a file. As of writing this is equivalent to:
-----------------------------
"/home/filip/.ssh/config" l s
-----------------------------