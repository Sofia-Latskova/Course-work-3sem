#include "quadcopter.hpp"

#include <iostream>

namespace firewatch {
	quadcopter::quadcopter(std::string_view number, battery Battery):m_number{ number }, m_battery{ std::move(Battery) } 
    {
		std::cout << "[Quadcopter] Создан " << m_number << "\n";
	}

	quadcopter::~quadcopter() 
    {
		std::cout << "[Quadcopter] Уничтожен " << m_number << "\n";
	}

	std::string quadcopter::GetNumber() const
	{
		return m_number;
	}

	quadcopter::State quadcopter::GetState() const
	{
		return m_state;
	}

    std::string quadcopter::GetStateStr() const
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

    int quadcopter::GetBatteryCharge() const
    {
        return m_battery.GetCharge();
    }

    bool quadcopter::TakeOff()
    {
        if (!CanTakeOff())
        {
            std::cout << "[Quadcopter] Отказ взлёта: заряд "<< m_battery.GetCharge() << "% ниже минимума "<< battery::min_charge << "%\n";
            return false;
        }
        m_state = State::eFlying;
        std::cout << "[Quadcopter] " << m_number << " взлетел\n";
        return true;
    }

    void quadcopter::Land()
    {
        m_state = State::eOnBase;
        std::cout << "[Quadcopter] " << m_number << " сел на базу\n";
    }

    void quadcopter::Scan()
    {
        if (m_state != State::eFlying)
        {
            std::cout << "[Quadcopter] Сканирование невозможно: не в полёте\n";
            return;
        }
        m_state = State::eScanning;
        m_battery.Consume(2);
        std::cout << "[Quadcopter] " << m_number << " снял показания, заряд=" << m_battery.GetCharge() << "%\n";
        m_state = State::eFlying;
    }

    bool quadcopter::CanTakeOff() const
    {
        return m_state == State::eOnBase && m_battery.GetCharge() >= battery::min_charge;
    }
}