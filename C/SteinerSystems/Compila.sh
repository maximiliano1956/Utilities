gcc -DWFILE -DDEBUG -g SteinerSystems.c dlx.c -o SteinerSystems
#gcc -g SteinerSystems.c dlx.c -o SteinerSystems
#gcc -DWFILE -g SteinerSystems.c dlx.c -o SteinerSystems
#gcc -DWDEBUG -g SteinerSystems.c dlx.c -o SteinerSystems
gcc -g debugSteiner.c dlx.c -o debugSteiner
gcc -g sparseMatrix.c -o sparseMatrix
