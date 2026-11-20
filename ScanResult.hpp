#pragma once
#include <vector>
#include <string>
namespace firewatch {

    // Результат сканирования одной точки маршрута.
    struct ScanResult {
        struct Reading {
            std::string serial;     // серийный номер датчика
            std::string kind;       // тип: "температура", "дым", "пламя"
            double      value{};    // показание
            bool        reliable{}; // false, если датчик отказал — значение недостоверно
        };

        std::vector<Reading> readings;
        bool hasReliableData{};     // есть ли хотя бы одно достоверное показание
    };

}