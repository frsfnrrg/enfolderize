#! /bin/sh
$EXTRACTRC *.kcfg >> rc.cpp
$XGETTEXT *.cpp -o $podir/enfolderizeplugin.pot
