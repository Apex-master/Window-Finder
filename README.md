# Window-Finder
Window Finder is the ultimate tool for finding windows with specific properties. You can quickly search for windows with specific sizes, class names, window styles, extended styles and you can export window details instantly.

* Window Finder (/windowfinder.h)

## Main Features

* Quickly display all top-level windows on the system in a table format
* Search by: ID, Window Name, Window Classs, Window HWND, Window Size, Process Name, Process ID, Thread ID, Child Windows, Window Styles and Extended Styles.
* Export window details with ease - choose between copying the information or saving it to a file.

<b>Demo:</b>
![](https://github.com/Apex-master/Window-Finder/blob/main/windowfinderdemo.mp4)

## How it works
<b>Window Finder</b><br/>
The program uses Windows API functions to enumerate all top-level windows on the system. The process starts with the EnumWindows function, which takes as its argument the EnumWindowsProc callback function. This callback function, in turn, retrieves information about each window that it enumerates, such as its window name, class, and process name, as well as its handle (hwnd), process ID, thread ID and other information such as its size, style flags and extended style flags. This information is stored in a custom AppWindow structure.

The EnumChildWindows function is called on each window to enumerate any child windows it might have. The information about each child window is similarly stored in the AppWindow structure and added to the windows vector.

The program also has the ability to export the information about the windows, either by copying it or saving it to a file. This information can then be used for further analysis or manipulation.

## To do
* Optimize the search function.
