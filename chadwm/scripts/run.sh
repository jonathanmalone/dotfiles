#!/bin/sh

xrdb merge ~/.Xresources 
feh --bg-fill ~/.config/backgrounds/windows.png &
#xset r rate 200 50 &

dash ~/.config/chadwm/scripts/bar.sh &
while type chadwm >/dev/null; do chadwm && continue || break; done
