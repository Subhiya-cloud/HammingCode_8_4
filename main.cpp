#include <iostream>
#include <string>
#include "hamming.h"

int main() {
    std::string input_data;
    std::cout << "Введите данные для кодирования (например, 1011): ";
    std::cin >> input_data;

    auto encoded = hamming8x4::encode(input_data);
    std::cout << "Закодированные данные:" << std::endl;
    hamming8x4::print(encoded);

    std::string input;
    std::cout << "Введите закодированные данные для проверки (q -> выход):" << std::endl;
    while (true) {
        std::cout << "Введите полученные данные: ";
        std::cin >> input;

        if (input == "q") {
            break;
        }

        uint8_t error_pos;
        auto result = hamming8x4::check(input, error_pos);
        switch (result) {
        case hamming8x4::kWrongInput:
            std::cout << "Неверный ввод данных." << std::endl;
            break;
        case hamming8x4::kNoError:
            std::cout << "Ошибок нет. Полученные данные: " << input << std::endl;
            break;
        case hamming8x4::kHasOneError:
            std::cout << "Обнаружена ошибка в бите " << (int)error_pos
                      << ". Исправленные данные: " << input << std::endl;
            break;
        case hamming8x4::kHasTwoErrors:
            std::cout << "Найдено две ошибки. Исправление невозможно." << std::endl;
            break;
        }
    }

    return 0;
}
