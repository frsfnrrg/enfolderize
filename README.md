
# Enfolderize

Plugin for Dolphin and Konqueror that lets you put a set of selected (or dropped) files in a new folder. When two or more files or folders are selected or dropped, Enfolderize adds an action `Place in New Folder` that creates to folder to contain the selected or dropped files.

#### To install

You need the development files for KDE 4, especially those for KIO and LibKonq. CMake > 2.8.8 is required. Running

    mkdir build && cd build
    cmake ..
    make

will build Enfolderize; then running

    make install

should install two plugins, one which hooks into dolphin's right-click menu, and the other of which adds a new action to menu appearing when dragging and dropping files into a folder using Dolphin or Konqueror. The drag-and-drop action should automatically work (although restarting Konqueror and reloading some of the shared libraries it depends on might be necessary). To enable the right-click menu option, open Dolphin, the `Settings` menu, then `Configure Dolphin`. Under the `Services` tab, check off `Enfolderize`, apply, restart, and the plugin should be active.

At minimum, Enfolderize works with Dolphin 15.04.0, Konqueror 4.14.8, KDE 4.14.3, gcc 4.8.3, and CMake 3.2.3, but should also work with versions a few years older.

##### User only install

If you do not have root access, build Enfolderize as explained in the previous section (but do not run `make install`).

Building Enfolderize should have created two files under `enfolderize/build/lib`, `enfolderizeplugin.so` and `enfolderizeplugin_dnd.so`. Copy both to `$HOME/.kde4/lib64/kde4/`. (Create any intervening folders (like `$HOME/.kde4/lib64/`) if they do not already exist. Those on 32-bit systems should copy to  `$HOME/.kde4/lib/kde4/`.)

From the project folder (`enfolderize/`), copy `enfolderizeplugin.desktop` and `enfolderizeplugin_dnd.desktop` to `$HOME/.kde4/share/kde4/services/ServiceMenus/`, again creating the intervening folders, since `services/` and `ServiceMenus/` typically don't exist by default.
