#include "quadcopter.hpp"

#include <iostream>
#include <utility>

namespace firewatch 
{
	Quadcopter::Quadcopter(std::string_view number, Battery battery)
        :m_number{ number },
        m_battery{ std::move(battery) } 
    {
		std::cout << "[Quadcopter] Создан " << m_number << "\n";
	}

	Quadcopter::~Quadcopter() 
    {
		std::cout << "[Quadcopter] Уничтожен " << m_number << "\n";
	}

	std::string Quadcopter::GetNumber() const
	{
		return m_number;
	}

	Quadcopter::State Quadcopter::GetState() const
	{
		return m_state;
	}

    std::string Quadcopter::GetStateStr() const
    {
        switch (m_state)
        {
        case State::eOnBase:    return "на базе";
        case State::eFlying:    return "в полёте";
        case State::eScanning:  return "сканирует";
        case State::eReturning: return "возвращается";
        }
        return "неизвестно";
    }

    int Quadcopter::GetBatteryCharge() const
    {
        return m_battery.GetCharge();
    }

    bool Quadcopter::TakeOff()
    {
        if (!CanTakeOff())
        {
            std::cout << "[Quadcopter] Отказ взлёта: заряд "<< m_battery.GetCharge() << "% ниже минимума "<< Battery::MIN_CHARGE << "%\n";
            return false;
        }
        m_state = State::eFlying;
        std::cout << "[Quadcopter] " << m_number << " взлетел\n";
        return true;
    }

    void Quadcopter::Land()
    {
        m_state = State::eOnBase;
        std::cout << "[Quadcopter] " << m_number << " сел на базу\n";
    }

    void Quadcopter::Scan()
    {
        if (m_state != State::eFlying)
        {
            std::cout << "[Quadcopter] Сканирование невозможно: не в полёте\n";
            return;
        }
        m_state = State::eScanning;
        m_battery.Consume(2);
        std::cout << "[Quadcopter] " << m_number << " снял показания, заряд=" << m_battery.GetCharge() << "%\n";
        if (m_battery.GetCharge() < Battery::CRITICAL_CHARGE)
        {
            m_state = State::eReturning;
            std::cout << "[Quadcopter] " << m_number
                << " возвращается на базу: заряд ниже критического ("
                << Battery::CRITICAL_CHARGE << "%)\n";
            return;
        }
        m_state = State::eFlying;
    }

    bool Quadcopter::CanTakeOff() const
    {
        return m_state == State::eOnBase && m_battery.GetCharge() >= Battery::MIN_CHARGE;
    }
}