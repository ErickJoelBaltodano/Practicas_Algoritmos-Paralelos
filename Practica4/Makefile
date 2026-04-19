Suma:
	@echo "==================== Suma de Matrices A+B con N hilos ====================="
	gcc -fopenmp -o src/Suma src/Suma.c
	./src/Suma $(N)
	rm -f src/Suma
	@echo "==========================================================================="
Transpuesta:
	@echo "================= Transpuesta de la matriz A con N hilos ==================="
	gcc -fopenmp -o src/Transpuesta src/Transpuesta.c
	./src/Transpuesta $(N)
	rm -f src/Transpuesta
	@echo "==========================================================================="
Inversa:
	@echo "==================== Inversa dela matriz B con N hilos ====================="
	gcc -fopenmp -o src/Inversa src/Inversa.c
	./src/Inversa $(N)
	rm -f src/Inversa
	@echo "==========================================================================="