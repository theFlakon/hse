#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

// Класс для работы с калькулятором
class Calculator {
public:
    // Метод для сложения
    double add(double a, double b) {
        return a + b;
    }

    // Метод для вычитания
    double subtract(double a, double b) {
        return a - b;
    }

    // Метод для умножения
    double multiply(double a, double b) {
        return a * b;
    }

    // Метод для деления
    double divide(double a, double b) {
        if (b == 0) {
            throw runtime_error("Ошибка: деление на ноль!");
        }
        return a / b;
    }
};

// Функция для получения числа от пользователя
double getNumber(const string& prompt) {
    double num;
    while (true) {
        cout << prompt;
        cin >> num;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите корректное число!\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return num;
        }
    }
}

// Функция для отображения меню
void displayMenu() {
    cout << "\n=== ПРОСТОЙ КАЛЬКУЛЯТОР ===\n";
    cout << "1. Сложение (+)\n";
    cout << "2. Вычитание (-)\n";
    cout << "3. Умножение (*)\n";
    cout << "4. Деление (/)\n";
    cout << "5. Выйти\n";
    cout << "============================\n";
    cout << "Выберите операцию (1-5): ";
}

int main() {
    Calculator calc;
    int choice;
    double num1, num2, result;
    
    cout << "Добро пожаловать в калькулятор!\n";
    
    while (true) {
        displayMenu();
        cin >> choice;
        
        if (cin.fail() || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: выберите число от 1 до 5!\n";
            continue;
        }
        
        if (choice == 5) {
            cout << "Выход из программы. До свидания!\n";
            break;
        }
        
        try {
            num1 = getNumber("Введите первое число: ");
            num2 = getNumber("Введите второе число: ");
            
            switch (choice) {
                case 1:
                    result = calc.add(num1, num2);
                    cout << "Результат: " << num1 << " + " << num2 << " = " << result << endl;
                    break;
                case 2:
                    result = calc.subtract(num1, num2);
                    cout << "Результат: " << num1 << " - " << num2 << " = " << result << endl;
                    break;
                case 3:
                    result = calc.multiply(num1, num2);
                    cout << "Результат: " << num1 << " * " << num2 << " = " << result << endl;
                    break;
                case 4:
                    result = calc.divide(num1, num2);
                    cout << "Результат: " << num1 << " / " << num2 << " = " << result << endl;
                    break;
            }
        } catch (const exception& e) {
            cout << e.what() << endl;
        }
        
        cout << "\nНажмите Enter для продолжения...";
        cin.get();
    }
    
    return 0;
}
