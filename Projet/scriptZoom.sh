#!/bin/bash

projet_mid_jeu=2 # A changer si menu different dans gnome profil cf README par shift+F10
projet_mid_menu=3 # A changer si menu different dans gnome profil cf README par shift+F10
if [ $1 = "-m" ]; then
	#xdotool type --clearmodifiers 'io:format("\e[H\e[J").'
    #xdotool key ctrl+l
	clear
	dconf load /org/gnome/terminal/legacy/profiles:/:b1dcc9dd-5262-4d8d-a863-c897e6d979b9/ < ./profile_gnome2.txt
	resize -s 200 800 > /dev/null
else 
	# cas p
	#xdotool key shift+F10 r $projet_mid_menu
	#xdotool type --clearmodifiers 'io:format("\e[H\e[J").'
    #xdotool key Return
    #xdotool key ctrl+l
	dconf load /org/gnome/terminal/legacy/profiles:/:b1dcc9dd-5262-4d8d-a863-c897e6d979b9/ < ./profile_gnome3.txt
	clear
	resize -s 30 110  > /dev/null
fi
