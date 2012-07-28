#!/bin/bash
# Author: MaG
# URL: www.newbieshell.blogspot.com

DEC="./decomment"  # ruta del programa; cambiar si es necesario

if [ -z $1 ]; then
	echo "Uso: $0 <directorio>"
	exit 1
fi

if [ ! -d $1 ]; then
	echo "\`$1' no es un directorio."
	exit 1
fi

if [ ! -x $DEC ]; then
	echo $DEC
	echo "Especifique la direccion absoluta del programa descomentador."
	exit 1
fi

cd $1

REGEX='.*\.([ch]|cpp|js|java)(\.?(~|old|back))+?$'

OLD_IFS=$IFS
IFS=$'\n'
for line in `find -regextype posix-extended -iregex $REGEX`; do
   $DEC "$line" 2>/dev/null > tmpfile
   mv tmpfile "$line"

done	
IFS=$OLD_IFS

echo $(pwd)
exit 0
