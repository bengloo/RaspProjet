#!/bin/bash

projet_mid_jeu=2 # A changer si menu different dans gnome profil cf README par shift+F10
projet_mid_menu=3 # A changer si menu different dans gnome profil cf README par shift+F10
if [ $1 = "-m" ]; then
	xdotool key shift+F10 r $projet_mid_jeu
	resize -s 200 800 > /dev/null
else 
	# cas p
	resize -s 40 120  > /dev/null
	xdotool key shift+F10 r $projet_mid_menu
fi
