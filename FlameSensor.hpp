#pragma once
#include "BaseSensor.hpp"
#include <memory>
#include <string>
#include <string_view>

namespace firewatch {

    // final запрещает наследование от этого класса
    class FlameSensor final : public BaseSensor
    {
    private:
        int m_intensity{ 0 }; // Интенсивность пламени от 0 до 100 баллов
        mutable int m_lastValue{ 0 };
    public:
        FlameSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter, int intensity);

        ~FlameSensor() override;

        // Возвращает интенсивность (0-100)
        double ReadValue() const override;

        // Переопределение БЕЗ вызова базового класса
        std::string Describe() const override;

        // Клонирование
        std::unique_ptr<BaseSensor> Clone() const override;

        // Метод для обновления (для будущих ЛР)
        void SetIntensity(int intensity);
    };

}