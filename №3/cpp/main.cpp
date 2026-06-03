#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Константы по варианту
const double M_val = 1.08;
const double N_val = 0.22;
const double P_val = -1.16;
const double EPS = 1e-3;
const int SIZE = 4;

// Вспомогательная функция для красивого вывода вектора корней
void printVector(const vector<double>& vec, const string& name) {
    cout << name << ": [";
    for (int i = 0; i < vec.size(); ++i) {
        cout << setw(9) << fixed << setprecision(5) << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << " ]\n";
}

// LU-разложение
void solveLU(const vector<vector<double>>& A, const vector<double>& b) {
    vector<vector<double>> L(SIZE, vector<double>(SIZE, 0.0));
    vector<vector<double>> U(SIZE, vector<double>(SIZE, 0.0));

    // На главной диагонали L всегда стоят единицы
    for (int i = 0; i < SIZE; ++i) L[i][i] = 1.0;

    // Вычисление матриц L и U 
    for (int i = 0; i < SIZE; ++i) {
        for (int k = i; k < SIZE; ++k) {
            double sum = 0.0;
            for (int j = 0; j < i; ++j) sum += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - sum;
        }
        for (int k = i + 1; k < SIZE; ++k) {
            double sum = 0.0;
            for (int j = 0; j < i; ++j) sum += L[k][j] * U[j][i];
            L[k][i] = (A[k][i] - sum) / U[i][i];
        }
    }

    cout << "\n=== ПРЯМОЙ МЕТОД (LU-разложение) ===\n";
    
    // Решение Ly = b 
    vector<double> y(SIZE, 0.0);
    for (int i = 0; i < SIZE; ++i) {
        double sum = 0.0;
        for (int j = 0; j < i; ++j) sum += L[i][j] * y[j];
        y[i] = b[i] - sum;
    }
    printVector(y, "Результат прямого хода (вектор y)");

    // Решение Ux = y 
    vector<double> x(SIZE, 0.0);
    for (int i = SIZE - 1; i >= 0; --i) {
        double sum = 0.0;
        for (int j = i + 1; j < SIZE; ++j) sum += U[i][j] * x[j];
        x[i] = (y[i] - sum) / U[i][i];
    }
    printVector(x, "Результат обратного хода (точные корни x)");
}

void solveZeidel(const vector<vector<double>>& A, const vector<double>& b) {
    cout << "\n=== ПРИБЛИЖЕННЫЙ МЕТОД (Метод Зейделя) ===\n";
    
    vector<vector<double>> C(SIZE, vector<double>(SIZE, 0.0));
    vector<double> f(SIZE, 0.0);

    // Преобразование системы к каноническому виду x = Cx + f 
    for (int i = 0; i < SIZE; ++i) {
        double diag = A[i][i];
        
        f[i] = b[i] / diag;
        for (int j = 0; j < SIZE; ++j) {
            if (i == j) C[i][j] = 0.0;
            else C[i][j] = -A[i][j] / diag;
        }
    }

    // Инициализация начального приближения x(0) = f
    vector<double> x_curr = f;
    vector<double> x_next(SIZE, 0.0);
    
    // Вывод шапки таблицы 
    cout << "-------------------------------------------------------------------------\n";
    cout << "|   N   |    X1    |    X2    |    X3    |    X4    |       Eps_n       |\n";
    cout << "-------------------------------------------------------------------------\n";
    
    // Нулевой шаг
    cout << "| " << setw(5) << 0 << " | ";
    for (double val : x_curr) cout << setw(8) << fixed << setprecision(4) << val << " | ";
    cout << setw(17) << "—" << " |\n";

    int step = 0;
    double eps_n = 1.0;

    // Итерационный цикл Зейделя
    while (eps_n > EPS && step < 100) {
        step++;
        x_next = x_curr; // Копируем текущие значения для обновления
        
        // Новые компоненты x_next[j] сразу используются на текущем шаге
        for (int i = 0; i < SIZE; ++i) {
            double sum = f[i];
            for (int j = 0; j < SIZE; ++j) {
                sum += C[i][j] * x_next[j];
            }
            x_next[i] = sum;
        }

        // Вычисление погрешности по Чебышёвской норме (максимум разностей)
        eps_n = 0.0;
        for (int i = 0; i < SIZE; ++i) {
            eps_n = max(eps_n, abs(x_next[i] - x_curr[i]));
        }

        x_curr = x_next;

        cout << "| " << setw(5) << step << " | ";
        for (double val : x_curr) cout << setw(8) << fixed << setprecision(4) << val << " | ";
        cout << setw(17) << fixed << setprecision(6) << eps_n << " |\n";
    }
    cout << "-------------------------------------------------------------------------\n";

    cout << "\nПриближенные корни с точностью до " << EPS << ":\n";
    printVector(x_curr, "x*");
}

int main() {
    // Матрица коэффициентов А (с подставленными M, N, P)
    vector<vector<double>> A = {
        { M_val, -0.04,  0.21, -1.16},
        {  0.25, -1.23, N_val, -0.09},
        {-0.21,  N_val,   0.8, -0.13},
        {  0.15, -1.31,  0.06,  P_val}
    };

    // Вектор правых частей b
    vector<double> b = {-1.24, P_val, 2.56, M_val};

    cout << "СЛАУ инициализирована параметрами: M=" << M_val << ", N=" << N_val << ", P=" << P_val << "\n";

    // Вызов точного метода
    solveLU(A, b);

    // Вызов выбранного приближенного метода (Зейделя)
    solveZeidel(A, b);

    return 0;
}