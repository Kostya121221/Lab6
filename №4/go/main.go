package main

import (
	"fmt"
	"math/rand"
	"time"
)

var N, M int
var totalBits int

// Преобразование координат (r, c) в индекс бита (от 0 до N*M - 1)
func getBitIndex(r, c int) int {
	return r*M + c
}

// Проверка на победу (половина сверху — одно значение, снизу — другое)
func isTarget(mask int) bool {
	half := N / 2
	topVal := mask & 1
	bottomVal := (mask >> (totalBits - 1)) & 1

	if topVal == bottomVal {
		return false
	}

	for r := 0; r < N; r++ {
		var expected int
		if r < half {
			expected = topVal
		} else {
			expected = bottomVal
		}
		for c := 0; c < M; c++ {
			currentBit := (mask >> getBitIndex(r, c)) & 1
			if currentBit != expected {
				return false
			}
		}
	}
	return true
}

// Вывод доски из битовой маски в консоль
func printBoardFromMask(mask int) {
	for r := 0; r < N; r++ {
		fmt.Print("| ")
		for c := 0; c < M; c++ {
			fmt.Printf("%d ", (mask>>getBitIndex(r, c))&1)
		}
		fmt.Println("|")
	}
}

func main() {
	// Инициализируем seed для случайных чисел
	rand.Seed(time.Now().UnixNano())

	fmt.Println("Выберите режим:")
	fmt.Println("1. Пример с картинки (4x3)")
	fmt.Println("2. Случайная генерация")
	fmt.Print("Ваш выбор: ")

	var choice int
	_, err := fmt.Scan(&choice)
	if err != nil {
		choice = 1
	}

	startMask := 0

	if choice == 1 {
		N, M = 4, 3
		totalBits = N * M
		exampleMatrix := [4][3]int{
			{1, 0, 1},
			{0, 1, 0},
			{1, 1, 0},
			{0, 0, 1},
		}
		for r := 0; r < N; r++ {
			for c := 0; c < M; c++ {
				if exampleMatrix[r][c] == 1 {
					startMask |= (1 << getBitIndex(r, c))
				}
			}
		}
	} else {
		fmt.Print("Введите количество строк N (желательно четное): ")
		fmt.Scan(&N)
		fmt.Print("Введите количество столбцов M: ")
		fmt.Scan(&M)

		totalBits = N * M
		for r := 0; r < N; r++ {
			for c := 0; c < M; c++ {
				if rand.Intn(2) == 1 {
					startMask |= (1 << getBitIndex(r, c))
				}
			}
		}
		fmt.Println("\nСгенерирована случайная доска:")
		printBoardFromMask(startMask)
	}

	maxStates := 1 << totalBits // Всего состояний 2^(N*M)
	fmt.Println("\nМгновенный поиск решения через быстрый BFS...")

	// Выделяем память под массив предков и очередь
	parent := make([]int, maxStates)
	for i := range parent {
		parent[i] = -1
	}
	q := make([]int, maxStates)
	head, tail := 0, 0

	// Помещаем стартовое состояние
	q[tail] = startMask
	tail++
	parent[startMask] = startMask // Помечаем как посещенное (само на себя)

	targetMask := -1

	// Главный цикл BFS
	for head < tail {
		curr := q[head]
		head++

		if isTarget(curr) {
			targetMask = curr
			break
		}

		// Генерируем все смежные ходы
		for r := 0; r < N; r++ {
			for c := 0; c < M; c++ {
				// Ход вправо
				if c+1 < M {
					nextMask := curr ^ (1 << getBitIndex(r, c)) ^ (1 << getBitIndex(r, c+1))
					if parent[nextMask] == -1 { // Если еще не посещали
						parent[nextMask] = curr // Запоминаем откуда пришли
						q[tail] = nextMask
						tail++
					}
				}
				// Ход вниз
				if r+1 < N {
					nextMask := curr ^ (1 << getBitIndex(r, c)) ^ (1 << getBitIndex(r+1, c))
					if parent[nextMask] == -1 {
						parent[nextMask] = curr
						q[tail] = nextMask
						tail++
					}
				}
			}
		}
	}

	// Если очередь пуста, а цель не найдена
	if targetMask == -1 {
		fmt.Println("Решение НЕ существует математически! (Конфигурация тупиковая).")
		return
	}

	// Восстанавливаем точный путь от финиша к старту
	var path []int
	curr := targetMask
	for curr != startMask {
		path = append(path, curr)
		curr = parent[curr]
	}
	path = append(path, startMask)

	fmt.Println("\n=== НАЧАЛО ПОШАГОВОГО ВЫВОДА ===")
	steps := 0
	for i := len(path) - 1; i >= 0; i-- {
		fmt.Printf("\nШаг %d:\n", steps)
		steps++
		printBoardFromMask(path[i])
	}

	fmt.Printf("\nМинимальное количество ходов: %d\n", len(path)-1)
}
