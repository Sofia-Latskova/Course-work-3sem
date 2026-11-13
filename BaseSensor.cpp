#include "BaseSensor.hpp"

#include <iostream>
#include <utility>

namespace firewatch {
    //конструктор с параметрами
    BaseSensor::BaseSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter)
        : m_id{ NextId() }
        , m_serial{ serial }
        , m_durability{ 10 + (std::rand() % 41) }
        , m_quadcopter{ std::move(quadcopter) }
    {
        std::cout << "[BaseSensor] Создан: " << m_serial
            << " (ID: " << m_id
            << ", дрон: " << m_quadcopter->GetNumber() << ")\n";
    }
    //оператор копирования
    BaseSensor& BaseSensor::operator=(const BaseSensor& other)
    {
        if (this == &other)                
        {
            return *this;
        }
        m_serial = other.m_serial;
        m_isBroken = other.m_isBroken;
        m_durability = other.m_durability;
        m_quadcopter = other.m_quadcopter;
        return *this;                      
    }

    //деструктор
    BaseSensor::~BaseSensor()
    {
        std::cout << "[BaseSensor] Уничтожен: " << m_serial
            << " (ID: " << m_id << ")\n";
    }

    //виртуальный метод, наследник может вызвать и переписать свое
    std::string BaseSensor::Describe() const
    {
        std::string result = GetHeader() + "\n";
        result += "ID: " + std::to_string(m_id) + "\n";
        result += "Дрон: " + m_quadcopter->GetNumber();
           
        if (m_isBroken) result += "[ДАТЧИК НЕИСПРАВЕН]\n";
        return result;
    }

    //метод только для наследников
    std::string BaseSensor::GetHeader() const
    {
        return "--- Датчик [" + m_serial + "] на " + m_quadcopter->GetNumber() + " [" + m_quadcopter->GetStateStr() + "]\n" + " ---";
    }

    //для слежки за состоянием (износ)
    bool BaseSensor::WearDown() const
    {
        // Если уже сломан, ничего не делаем
        if (m_isBroken) return true;
        if (m_durability > 0)
        {
            m_durability--;
            // Если ресурс только что закончился
            if (m_durability == 0)
            {
                Break(); // Ломаем датчик
                std::cout << "[BaseSensor] Внимание: датчик " << m_serial
                    << " сломался от износа!\n"; 
                return true;
            }
        }
        return false;
    }

}

















