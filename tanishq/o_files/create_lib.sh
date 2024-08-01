rm *.o
rm ../lib/libtanishq.a

gcc -c ../src/matrix/*.c -I ../include -Wall
gcc -c ../src/vector/*.c -I ../include -Wall
gcc -c ../src/operations/*.c -I ../include -Wall

ar rcs ../lib/libtanishq.a *.o