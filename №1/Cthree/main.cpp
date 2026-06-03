#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

const int ROWS = 20;
const int COLS = 40;
enum class Choi {
    DEMO = 1,
    GEN         
};


// Отображение 
void printGrid(const std::vector<std::vector<bool>>& grid) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            // Живая клетка — закрашенный квадрат, мертвая — точка
            std::cout << (grid[i][j] ? "■ " : "· ");
        }
        std::cout << "\n";
    }
}

// Подсчет живых соседей вокруг клетки 
int countNeighbors(const std::vector<std::vector<bool>>& grid, int r, int c) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // Саму клетку не считаем

            int nr = r + i;
            int nc = c + j;

            // Проверяем, не вышли ли за границы ограниченной плоскости
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS) {
                if (grid[nr][nc]) {
                    count++;
                }
            }
        }
    }
    return count;
}

// Генерация следующего поколения
void updateGrid(std::vector<std::vector<bool>>& grid) {
    std::vector<std::vector<bool>> nextGrid = grid;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int neighbors = countNeighbors(grid, i, j);

            if (grid[i][j]) {
                // Клетка умирает от одиночества или перенаселения
                if (neighbors < 2 || neighbors > 3) {
                    nextGrid[i][j] = false;
                }
                // Живет дальше при 2 или 3 соседях
            } else {
                // В пустой клетке зарождается жизнь, если соседей ровно 3
                if (neighbors == 3) {
                    nextGrid[i][j] = true;
                }
            }
        }
    }
    grid = nextGrid;
}

// Функция размещения устойчивых фигур 
void placeStillLifes(std::vector<std::vector<bool>>& grid) {
    // Block 
    grid[2][2] = true; grid[2][3] = true;
    grid[3][2] = true; grid[3][3] = true;

    // Beehive 
    grid[2][9] = true;  grid[2][10] = true;
    grid[3][8] = true;                      grid[3][11] = true;
    grid[4][9] = true;  grid[4][10] = true;

    // Loaf 
    grid[2][18] = true; grid[2][19] = true;
    grid[3][17] = true;                     grid[3][20] = true;
    grid[4][18] = true;                     grid[4][21] = true;
    grid[5][19] = true; grid[5][20] = true;

    // Boat 
    grid[2][28] = true; grid[2][29] = true;
    grid[3][28] = true;                     grid[3][30] = true;
    grid[4][29] = true;
}

// Функция случайного заполнения поля
void fillRandom(std::vector<std::vector<bool>>& grid) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            // Здесь делаем маленький шанс сделать живую клетку
            grid[i][j] = (dis(gen) == 1 && dis(gen) == 1); 
        }
    }
}

int main() {
    std::setlocale(LC_ALL, "Russian");
    
    std::vector<std::vector<bool>> grid(ROWS, std::vector<bool>(COLS, false));

    std::cout << "Выберите режим запуска:\n";
    std::cout << "1. Демонстрация устойчивых фигур (Block, Beehive, Loaf, Boat)\n";
    std::cout << "2. Случайная генерация колонии\n";
    int choice;
    std::cin >> choice;
    Choi userChoice = static_cast<Choi>(choice);
    if (userChoice == Choi::DEMO) {
        placeStillLifes(grid);
    } else {
        fillRandom(grid);
    }

    // Скрываем курсор консоли, чтобы он не мигал при обновлении экрана

    // Очищаем консоль один раз перед стартом анимации
    system("cls");

    // Бесконечный цикл анимации
    int generation = 0;
    while (true) {
        system("clear"); 
       
        
        std::cout << "Поколение: " << generation++ << " (Нажмите Ctrl+C для выхода)\n\n";
        printGrid(grid);
        
        updateGrid(grid);

        // Задержка между поколениями 
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}