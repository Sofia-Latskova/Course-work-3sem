#include "FlameSensor.hpp"
#include <iostream>
#include <cstdlib>
#include <utility>

namespace firewatch {
    //конструктор
    FlameSensor::FlameSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter, int intensity)
        : BaseSensor(serial, std::move(quadcopter))
        , m_intensity{ intensity }
    {
        std::cout << "[FlameSensor] Создан датчик пламени." << GetSerial() << "\n";
    }

    //деструктор
    FlameSensor::~FlameSensor()
    {
        std::cout << "[FlameSensor] Уничтожен датчик пламени." << GetSerial() << "\n";
    }
    
    double FlameSensor::ReadValue() const
    {
        if (WearDown()) return -999.0; //чтото не так с датчиком
        if (m_intensity > 90)
        {
            std::cout << "[FlameSensor] " << GetSerial() << " ПЕРЕГРУЗКА! Вышел из строя от сильного пламени!\n";
            Break();
            return -999.0;
        }

        // Если пламя слабое (< 10 баллов), считаем, что его нет
        int result = m_intensity;
        if (result < 10)
        {
            result = 0;
        }

        // Небольшой шум 
        int noise = (std::rand() % 5) - 2;
        int value = result + noise;
        if (value < 0)   value = 0;
        if (value > 100) value = 100;

        m_lastValue = value;
        return static_cast<double>(m_lastValue);
    }

    std::string FlameSensor::Describe() const
    {
        if (IsBroken()) return GetHeader() + "\n[ДАТЧИК НЕИСПРАВЕН]";

        std::string result = GetHeader() + "\n";
        result += "ID: " + std::to_string(GetId()) + " | Тип: Датчик пламени\n";
        result += "Интенсивность: " + std::to_string(m_lastValue) + " / 100 баллов\n";

        if (m_lastValue > 80) result += "Статус: [КРИТИЧЕСКИЙ ПОЖАР]";
        else if (m_lastValue > 40) result += "Статус: [ВНИМАНИЕ: ПЛАМЯ]";
        else result += "Статус: [В норме]";

        return result;
    }

    std::unique_ptr<BaseSensor> FlameSensor::Clone() const
    {
        auto copy = std::make_unique<FlameSensor>(*this);
        copy->m_quadcopter = std::make_shared<Quadcopter>(*m_quadcopter);
        return copy;
    }

 
    void FlameSensor::SetIntensity(int intensity)
    {
        m_intensity = intensity;
        std::cout << "[FlameSensor] " << GetSerial()
            << ": установлена интенсивность " << m_intensity << " баллов\n";
    }

}