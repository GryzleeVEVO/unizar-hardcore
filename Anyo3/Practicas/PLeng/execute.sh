if [[ $1 != *.cl ]] && [[ -f $1.cl ]]
then
	java -jar clike.jar $1.cl > $1.cpp
	g++ $1.cpp -o $1
	./$1 
	rm $1
	rm $1.cpp
else
	echo "Introduce el nombre correcto del fichero fuente sin .cl"
fi
