#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>

void printMatrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int elem : row) {
            std::cout << std::setw(4) << elem;
        }
        std::cout << std::endl;
    }
}

int main() {
    std::setlocale(LC_ALL, "Russian");

    int M, N;
    std::cout << "Введите количество строк (M): ";
    std::cin >> M;
    std::cout << "Введите количество столбцов (N): ";
    std::cin >> N;

    if (M <= 0 || N <= 0) {
        std::cout << "Размеры матрицы должны быть положительными числами!" << std::endl;
        return 1;
    }

    // Инициализация генератора случайных чисел в диапазоне [10, 50]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 50);

    // Создание и заполнение матрицы
    std::vector<std::vector<int>> matrix(M, std::vector<int>(N));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    std::cout << "\n--- Матрица до модификации ---\n";
    printMatrix(matrix);

    // заполнение массива разностей
    std::vector<int> differences(M);

    for (int i = 0; i < M; ++i) {
        int min_idx = 0;
        int max_idx = 0;

        // Поиск индексов ПЕРВОГО минимального и ПЕРВОГО максимального элементов
        for (int j = 1; j < N; ++j) {
            if (matrix[i][j] < matrix[i][min_idx]) {
                min_idx = j;
            }
            if (matrix[i][j] > matrix[i][max_idx]) {
                max_idx = j;
            }
        }

        // Меняем местами первый минимум и первый максимум в строке
        std::swap(matrix[i][min_idx], matrix[i][max_idx]);

        // Вычисляем разность после перестановки 
        differences[i] = std::abs(matrix[i][min_idx] - matrix[i][max_idx]);
    }

    std::cout << "\n--- Матрица после модификации ---\n";
    printMatrix(matrix);

    std::cout << "\nМассив разностей между max и min каждой строки:\n";
    for (int i = 0; i < M; ++i) {
        std::cout << "Строка " << i << ": " << differences[i] << "\n";
    }


    int max_diff_idx = 0;
    for (int i = 1; i < M; ++i) {
        if (differences[i] > differences[max_diff_idx]) {
            max_diff_idx = i;
        }
    }

    std::cout << "\nНомер строки с максимальной разностью (индексация с 0): " << max_diff_idx << std::endl;

    return 0;
}