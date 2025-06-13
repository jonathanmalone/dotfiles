#!/bin/dash

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

BINDIR=$HOME/.local/bin

# load colors
. ~/.config/chadwm/scripts/bar_themes/onedark

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)

  #printf "^c$green^ ^b$black^ "
  printf "^c$green^ ^b$black^$($BINDIR/cpu)"
}

pkg_updates() {
  #updates=$({ timeout 20 doas xbps-install -un 2>/dev/null || true; } | wc -l) # void
  #updates=$({ timeout 20 checkupdates 2>/dev/null || true; } | wc -l) # arch
  updates=$({ timeout 20 aptitude search '~U' 2>/dev/null || true; } | wc -l)  # apt (ubuntu, debian etc)

  if [ "$updates" = "0" ] || [ -z "$updates" ]; then
    printf " "
  else
    printf "  ^c$red^    $updates"" updates"
  fi
}

mem() {
  printf "^c$blue^^b$black^ $($BINDIR/mem)"
}

wlan() {
    case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
    up) printf "^c$blue^ $(essid) %s^b$black^" $(signalperc) ;;
    down) printf "^c$red^ ^b$black^WIFI  ^d^%s" " ^c$red^Disconnected" ;;
    esac
}

battery() {
    printf "^c$green^^b$black^ $($BINDIR/battery perc)"
    printf "^c$green^^b$black^$($BINDIR/battery stat)^b$black^"
}


clock() {
    printf "^c$green^^b$black^ $($BINDIR/gettime) "
}

while true; do
  [ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] && updates=$(pkg_updates)
  interval=$((interval + 1))

  xsetroot -name "$updates $(battery) $(wlan) $(cpu) $(mem) $(clock)" && sleep 5
done
