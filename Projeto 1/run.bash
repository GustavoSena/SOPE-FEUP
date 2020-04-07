#!/bin/bash
echo "CHANGE FLAGS IN BASH"
cd src
echo "----------------------------"
# $? = 0 se compilou bem
# $? = 2 otherwise
make 
if [ $? -eq 0 ] ; then
  ./simpledu -l ../TestDir -a --block-size=4096 --max-depth=4 -L -S $@
  make clean
else
  echo "COMPILATION ERROR";
fi
# Flags Possiveis:
# -a  --all               - mostra também files
# -b  --bytes             - apresenta nº de bytes em vez de blocos 
# -B  --block-size=SIZE   - define o tamanho dos blocos
# -l  --count-links       - contabiliza multiplas vezes o mesmo ficheiro
# -L  --dereference       - segue links simbolicos
# -S  --separate-dirs     - não inclui o tamanho dos subdirs
# --max-depth=N           - limita a informação exibida a N (0,1, …) níveis de profundidade de diretórios.
