#!/bin/bash

#On dézoom

#xdotool key control+0x002d


#On remet la fenêtre au zoom normal

#xdotool key Ctrl+0





function zoomArriere() {
	for i in 1 2 3 4 5 6 7

	do

		xdotool key control+0x002d

	done
}

function zoomAvant() {

	xdotool key control+0x0030
}

while getopts "mp" opt; do >
	case $opt in 
	m)
		zoomArriere
		;;
	p)
		zoomAvant
		;;
	\?)
	exit 1
	;;
	esac 
done 
