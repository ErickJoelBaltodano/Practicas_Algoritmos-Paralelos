run:
	echo "Iniciando programa secuencial"
	gcc -fopenmp -o src/secuencial src/main.c
	./src/secuencial 1 
	sudo rm src/secuencial
	echo "Iniciando programa paralelo"
	gcc -fopenmp -o src/paralelo src/main.c
	./src/paralelo $(THREADS)
	sudo rm src/paralelo



