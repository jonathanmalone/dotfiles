#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

alias ls='ls --color=auto --group-directories-first'
alias grep='grep --color=auto'
#PS1='[\u@\h \W]\$ '
PS1="\[\e[1;32m\]\u@\h\[\e[1;34m\] \w\[\e[0m\]\$ "

alias ll='ls -lh --color=auto'
alias sync-drives='rsync -avh --ignore-existing /mnt/disk1/plex/ /mnt/disk2/plex'
export PATH="/home/tyler/.local/bin:$PATH"
export _JAVA_AWT_WM_NONREPARENTING=1
alias sync-drives='rsync -avh --ignore-existing /mnt/disk1/plex /mnt/disk2/'
