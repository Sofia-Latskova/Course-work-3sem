#include "SmokeSensor.hpp"
#include <iostream>
#include <cstdlib>
#include <utility>

namespace firewatch {
	SmokeSensor::SmokeSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter, double smokeLevel)
		:BaseSensor(serial, std::move(quadcopter))
		, m_smokeLevel{ smokeLevel }
	{
		std::cout << "[SmokeSensor] Создан датчик дыма. " << GetSerial() << "\n";
	}

	SmokeSensor::~SmokeSensor()
	{
		std::cout << "[SmokeSensor] Уничтожен датчик дыма. " << GetSerial() << "\n";
	}

	double SmokeSensor::ReadValue() const
	{
        if (WearDown()) return -999.0; //если сломался
        // Прогрев
        if (m_warmupCounter < 2)
        {
            m_warmupCounter++;
            std::cout << "[SmokeSensor] Прогрев датчика " << GetSerial() << "...\n";
            return 0.0;
        }
        // Перегрузка
        if (m_smokeLevel > 0.90)
        {
            std::cout << "[SmokeSensor] ПЕРЕГРУЗКА! Датчик " << GetSerial() << " сгорел!\n";
            Break();
            return -999.0;
        }
        // Накопление тревог
        if (m_smokeLevel > 0.5)
        {
            m_warningCount++;
            if (m_warningCount >= 3)
                std::cout << "[SmokeSensor] КРИТИЧЕСКАЯ ТРЕВОГА! Дым замечен в трех точках!\n";
        }
        else
        {
            m_warningCount = 0;
        }
        double noise = (std::rand() % 5 - 2) / 100.0;
        m_lastValue = m_smokeLevel + noise; 
        double result = m_lastValue ;
        if (result < 0.0) result = 0.0;
        if (result > 1.0) result = 1.0;
        return result;
	}

    void SmokeSensor::SetSmokeLevel(double level) {
        m_smokeLevel = level;
        std::cout << "[SmokeSensor] " << GetSerial()
            << ": установлен уровень дыма " << (int)(m_smokeLevel * 100) << "%\n";
    }

    std::string SmokeSensor::Describe() const
    {
        if (IsBroken()) return GetHeader() + "\n[ДАТЧИК НЕИСПРАВЕН]";

        int percent = static_cast<int>(m_lastValue * 100);

        std::string result = GetHeader() + "\n";
        result += "ID: " + std::to_string(GetId()) + " | Тип: Датчик дыма\n";
        result += "Задымление: " + std::to_string(percent) + "%\n";

        if (m_lastValue > 0.9) result += "Статус: [КРИТИЧЕСКИЙ УРОВЕНЬ]";
        else if (m_lastValue > 0.5) result += "Статус: [Внимание: дым]";
        else result += "Статус: [В норме]";

        return result;
    }

    std::unique_ptr<BaseSensor> SmokeSensor::Clone() const
    {
        auto copy = std::make_unique<SmokeSensor>(*this);
        copy->m_quadcopter = std::make_shared<Quadcopter>(*m_quadcopter);
        return copy;
    }


}