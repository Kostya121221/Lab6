package main

import (
	"fmt"
	"math"
)

// Константы по варианту задания
const (
	M_val = 1.08
	N_val = 0.22
	P_val = -1.16
	EPS   = 1e-3
	SIZE  = 4
)

// Вспомогательная функция для красивого вывода вектора
func printVector(vec []float64, name string) {
	fmt.Printf("%s: [", name)
	for i, val := range vec {
		fmt.Printf("%9.5f", val)
		if i < len(vec)-1 {
			fmt.Print(", ")
		}
	}
	fmt.Println(" ]")
}

// LU-разложение
func solveLU(A [][]float64, b []float64) {
	// Инициализация матриц L и U нулями
	L := make([][]float64, SIZE)
	U := make([][]float64, SIZE)
	for i := 0; i < SIZE; i++ {
		L[i] = make([]float64, SIZE)
		U[i] = make([]float64, SIZE)
		L[i][i] = 1.0 // На главной диагонали L стоят единицы
	}

	// Вычисление элементов матриц L и U
	for i := 0; i < SIZE; i++ {
		for k := i; k < SIZE; k++ {
			sum := 0.0
			for j := 0; j < i; j++ {
				sum += L[i][j] * U[j][k]
			}
			U[i][k] = A[i][k] - sum
		}
		for k := i + 1; k < SIZE; k++ {
			sum := 0.0
			for j := 0; j < i; j++ {
				sum += L[k][j] * U[j][i]
			}
			L[k][i] = (A[k][i] - sum) / U[i][i]
		}
	}

	fmt.Println("\n=== ПРЯМОЙ МЕТОД (LU-разложение) ===")

	// Шаг 1: Решение Ly = b (Прямая подстановка)
	y := make([]float64, SIZE)
	for i := 0; i < SIZE; i++ {
		sum := 0.0
		for j := 0; j < i; j++ {
			sum += L[i][j] * y[j]
		}
		y[i] = b[i] - sum
	}
	printVector(y, "Результат прямого хода (вектор y)")

	// Шаг 2: Решение Ux = y (Обратный ход)
	x := make([]float64, SIZE)
	for i := SIZE - 1; i >= 0; i-- {
		sum := 0.0
		for j := i + 1; j < SIZE; j++ {
			sum += U[i][j] * x[j]
		}
		x[i] = (y[i] - sum) / U[i][i]
	}
	printVector(x, "Результат обратного хода (точные корни x)")
}

// Метод Зейделя
func solveZeidel(A [][]float64, b []float64) {
	fmt.Println("\n=== ПРИБЛИЖЕННЫЙ МЕТОД (Метод Зейделя) ===")

	C := make([][]float64, SIZE)
	f := make([]float64, SIZE)
	for i := 0; i < SIZE; i++ {
		C[i] = make([]float64, SIZE)
	}

	// Преобразование системы к каноническому виду x = Cx + f (без перестановок)
	for i := 0; i < SIZE; i++ {
		diag := A[i][i]

		f[i] = b[i] / diag
		for j := 0; j < SIZE; j++ {
			if i == j {
				C[i][j] = 0.0
			} else {
				C[i][j] = -A[i][j] / diag
			}
		}
	}

	// Инициализация начального приближения x(0) = f
	xCurr := make([]float64, SIZE)
	copy(xCurr, f)
	xNext := make([]float64, SIZE)

	// Вывод шапки таблицы
	fmt.Println("-------------------------------------------------------------------------")
	fmt.Println("|   N   |    X1    |    X2    |    X3    |    X4    |       Eps_n       |")
	fmt.Println("-------------------------------------------------------------------------")

	// Нулевой шаг
	fmt.Printf("| %5d | ", 0)
	for _, val := range xCurr {
		fmt.Printf("%8.4f | ", val)
	}
	fmt.Printf("%17s |\n", "—")

	step := 0
	epsN := 1.0

	// Итерационный цикл Зейделя
	for epsN > EPS && step < 100 {
		step++
		copy(xNext, xCurr)

		// Новые вычисленные значения сразу используются в текущей итерации
		for i := 0; i < SIZE; i++ {
			sum := f[i]
			for j := 0; j < SIZE; j++ {
				sum += C[i][j] * xNext[j]
			}
			xNext[i] = sum
		}

		// Вычисление максимальной нормы погрешности (Чебышёва)
		epsN = 0.0
		for i := 0; i < SIZE; i++ {
			diff := math.Abs(xNext[i] - xCurr[i])
			if diff > epsN {
				epsN = diff
			}
		}

		copy(xCurr, xNext)

		// Печать строки таблицы
		fmt.Printf("| %5d | ", step)
		for _, val := range xCurr {
			fmt.Printf("%8.4f | ", val)
		}
		fmt.Printf("%17.6f |\n", epsN)
	}
	fmt.Println("-------------------------------------------------------------------------")

	fmt.Printf("\nПриближенные корни с точностью до %.3f:\n", EPS)
	printVector(xCurr, "x*")
}

func main() {
	// Матрица коэффициентов А
	A := [][]float64{
		{M_val, -0.04, 0.21, -1.16},
		{0.25, -1.23, N_val, -0.09},
		{-0.21, N_val, 0.8, -0.13},
		{0.15, -1.31, 0.06, P_val},
	}

	// Вектор правых частей b
	b := []float64{-1.24, P_val, 2.56, M_val}

	fmt.Printf("СЛАУ инициализирована параметрами: M=%.2f, N=%.2f, P=%.2f\n", M_val, N_val, P_val)

	// Запуск точного метода
	solveLU(A, b)

	// Запуск итерационного метода
	solveZeidel(A, b)
}
