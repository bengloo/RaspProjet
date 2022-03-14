all:pi_sur_pi

pi_sur_pi:
	gcc -Wall -O2 -o 7seg_bp_ada.o -c 7seg_bp_ada.c
	gcc -Wall -O2 -o 7seg_bp_ada_test.o -c 7seg_bp_ada_test.c
	gcc -Wall -O2 -o 7seg_bp_ada_test 7seg_bp_ada.o -li2c 7seg_bp_ada_test.o

shared:
	gcc -shared -o 7seg_bp_ada.so -fPic 7seg_bp_ada.c
 