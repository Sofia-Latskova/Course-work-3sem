#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace firewatch {
    //длч работы программы (датчик сломался, заряд упал)
    class FirewatchError : public std::runtime_error {
    public:
        explicit FirewatchError(std::string_view message)
            : std::runtime_error{ std::string(message) } {}
    };

    //Датчик неисправен или ресурс исчерпан
    // для принятия решения о пожаре; его показания недостоверны
    class SensorFailure : public FirewatchError {
    public:
        explicit SensorFailure(std::string_view serial)
            : FirewatchError("датчик [" + std::string(serial) + "] неисправен") {
        }
    };

    // Заряд ниже минимального порога взлёта.
    // Нарушение правила: нельзя взлететь с разряженным аккумулятором.
    class LowBatteryError : public FirewatchError {
    public:
        LowBatteryError(int charge, int min)
            : FirewatchError("заряд " + std::to_string(charge)
                + "% ниже минимума " + std::to_string(min) + "%") {
        }
    };

    // Попытка разрядить батарею ниже 0.
    class BatteryOverDischarge : public std::logic_error {
    public:
        BatteryOverDischarge(int charge, int percent)
            : std::logic_error("разряд " + std::to_string(percent)
                + "% уводит заряд " + std::to_string(charge)
                + "% ниже 0") {
        }
    };

    // Неверный статус миссии: прервать/завершить можно только
    // выполняемое задание.
    class InvalidMissionState : public std::logic_error {
    public:
        explicit InvalidMissionState(std::string_view message)
            : std::logic_error{ std::string(message) } {}
    };

}