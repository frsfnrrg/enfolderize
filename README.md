# Enfolderize

Plugin for Dolphin that lets you put a set of selected (or dropped) files in a new folder. When two or more files or folders are selected or dropped, Enfolderize adds an action `Place in New Folder` that creates to folder to contain the selected or dropped files.

#### To install

You need the development files for KDE Frameworks 5, especially those for KIO. CMake > 2.8.12 is required. Running

    mkdir build && cd build
    cmake ..
    make

will build Enfolderize; then running

    make install

should install two plugins, one which hooks into dolphin's right-click menu, and the other of which adds a new action to menu appearing when dragging and dropping files into a folder using Dolphin. The drag-and-drop action should automatically work. To enable the right-click menu option, open Dolphin, the `Settings` menu, then `Configure Dolphin`. Under the `Services` tab, check off `Enfolderize`, apply, restart, and the plugin should be active.

At minimum, Enfolderize works with Dolphin 15.08.0, KDE Frameworks 5.13, gcc 5.2.0, and CMake 3.3.1, but should also work with versions a year or two older.

##### User only install

If you do not have root access, build Enfolderize as explained in the previous section (but do not run `make install`).

First, create a local directory to host the plugins. Then, edit `.profile` or `.bashrc` to set the environment variable

    export QT_PLUGIN_PATH="$LOCAL_PLUGINS_PATH"

with your folder's path replacing `$LOCAL_PLUGINS_PATH`;

Building Enfolderize should have created two files under `enfolderize/build`, `enfolderizeplugin.so` and `enfolderizeplugin_dnd.so`. Copy `enfolderizeplugin.so` into `$LOCAL_PLUGINS_PATH`. Then, create the folders `$LOCAL_PLUGINS_PATH/kf5` and `$LOCAL_PLUGINS_PATH/kf5/kio_dnd/`, and copy `enfolderizeplugin_dnd.so` into the latter.

From the project folder (`enfolderize/`), copy `enfolderizeplugin.desktop` and `enfolderizeplugin_dnd.desktop` into `$HOME/.local/share/kservices5/`, again creating the intervening folders, since `kservices5/` typically doesn't exist by default.

#### Possible issues

The installation directories used with `make install` may not be correct on all systems -- when in doubt, place them in the same folder as other matching kde/qt plugins.

Edit the value `MIN_NUMBER_OF_OBJECTS` in `flags.h` to control the minimum number of selected items needed to activate the plugin (default 2).
