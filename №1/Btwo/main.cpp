#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

// Функция для вычисления суммы цифр одного числа
int sumOfDigits(int number) {
    int sum = 0;
    number = std::abs(number); // На случай, если число отрицательное
    while (number > 0) {
        sum += number % 10;    // Берем последнюю цифру
        number /= 10;          // Удаляем последнюю цифру
    }
    return sum;
}

// Функция для красивого вывода
void printMatrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int elem : row) {
            std::cout << std::setw(5) << elem;
        }
        std::cout << std::endl;
    }
}

int main() {
    // Настройка вывода русском языке
    std::setlocale(LC_ALL, "Russian");

    int M, N;
    std::cout << "Введите количество строк (M): ";
    std::cin >> M;
    std::cout << "Введите количество столбцов (N): ";
    std::cin >> N;

    if (M <= 0 || N <= 0) {
        std::cout << "Размеры матрицы должны быть больше нуля!" << std::endl;
        return 1;
    }

    // Инициализация генератора случайных чисел в диапазоне [100, 200]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 200);

    std::vector<std::vector<int>> matrix(M, std::vector<int>(N));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    std::cout << "\n--- Сгенерированная матрица ---\n";
    printMatrix(matrix);
    std::cout << "-------------------------------\n\n";

    // Переменные для поиска строки с максимальной суммой цифр
    int max_row_idx = 0;
    int max_digit_sum = -1; 

    // Расчет суммы цифр для каждой строки
    for (int i = 0; i < M; ++i) {
        int current_row_digit_sum = 0;
        
        for (int j = 0; j < N; ++j) {
            current_row_digit_sum += sumOfDigits(matrix[i][j]);
        }

        std::cout << "Строка " << i << ": сумма цифр всех элементов = " << current_row_digit_sum << "\n";

        // Поиск строки с наибольшей суммой
        if (current_row_digit_sum > max_digit_sum) {
            max_digit_sum = current_row_digit_sum;
            max_row_idx = i;
        }
    }

    //Вывод результата
    std::cout << "\nНомер строки с наибольшей суммой цифр (индексация с 0): " << max_row_idx << std::endl;
    std::cout << "Наибольшая сумма цифр равна: " << max_digit_sum << std::endl;

    return 0;
}