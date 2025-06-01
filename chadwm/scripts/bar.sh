#!/bin/dash

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors
. ~/.config/chadwm/scripts/bar_themes/onedark

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)

  #printf "^c$green^ ^b$black^ "
  printf "^c$green^ ^b$black^  CPU"
  printf "^c$green^ ^b$grey^ $cpu_val"
}

pkg_updates() {
  #updates=$({ timeout 20 doas xbps-install -un 2>/dev/null || true; } | wc -l) # void
  updates=$({ timeout 20 checkupdates 2>/dev/null || true; } | wc -l) # arch
  # updates=$({ timeout 20 aptitude search '~U' 2>/dev/null || true; } | wc -l)  # apt (ubuntu, debian etc)

  if [ "$updates" = "0" ] || [ -z "$updates" ]; then
    printf " "
  else
    printf "  ^c$red^    $updates"" updates"
  fi
}

mullvad() {
    CITY=$(curl https://am.i.mullvad.net/city 2>/dev/null)
    IP=$(curl https://am.i.mullvad.net/ip 2>/dev/null)

    printf "^c$darkblue^^b$black^ VPN: "
    printf "^c$blue^^b$grey^  $CITY $IP"
}

mem() {
  printf "^c$blue^^b$black^  MEM "
  printf "^c$blue^^b$grey^ $(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)"
}

wlan() {
	case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
	up) printf "^c$black^ ^b$blue^ 󰤨 ^d^%s" " ^c$blue^Connected" ;;
	down) printf "^c$red^ ^b$black^ 󰤭 ^d^%s" " ^c$red^Disconnected" ;;
	esac
}


clock() {
	printf "^c$green^^b$black^  $(date '+%I:%M') "
}

while true; do

  [ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] && updates=$(pkg_updates)
  interval=$((interval + 1))

  sleep 1 && xsetroot -name "$updates $(mullvad) $(cpu) $(mem) $(clock)"
done
