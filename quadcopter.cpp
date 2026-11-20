#include "quadcopter.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <utility>

namespace firewatch 
{
	Quadcopter::Quadcopter(std::string_view number, int chargePercent, int capacityPercent, std::string_view batterySerial)
        :m_number{ number },
        m_battery{ std::make_unique<Battery>(chargePercent, capacityPercent, batterySerial) }
    {
		std::cout << "[Quadcopter] Создан " << m_number << "\n";
	}

	Quadcopter::~Quadcopter()  noexcept
    {  
		std::cout << "[Quadcopter] Уничтожен " << m_number << "\n";
	}

    Quadcopter::Quadcopter(const Quadcopter& other)
        : m_number{ other.m_number }
        , m_state{ other.m_state }
        , m_battery{ std::make_unique<Battery>(*other.m_battery) }   
    {
        std::cout << "[Quadcopter] Копия: " << m_number << "\n";
    }

    Quadcopter& Quadcopter::operator=(const Quadcopter& other)
    {
        if (this == &other) return *this;         
        m_number = other.m_number;
        m_state = other.m_state;
        *m_battery = *other.m_battery;              
        return *this;
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
        return m_battery->GetCharge();
    }

    bool Quadcopter::TakeOff()
    {
        if (!CanTakeOff())
        {
            throw LowBatteryError{ m_battery->GetCharge(), Battery::MIN_CHARGE };
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
            throw InvalidMissionState{
            "сканирование невозможно: дрон не в полёте (состояние: "
            + GetStateStr() + ")" };
        }
        m_state = State::eScanning;
        m_battery->Consume(2);
        std::cout << "[Quadcopter] " << m_number << " снял показания, заряд=" << m_battery->GetCharge() << "%\n";
        if (m_battery->GetCharge() < Battery::CRITICAL_CHARGE)
        {
            m_state = State::eReturning;
            throw LowBatteryError{ m_battery->GetCharge(), Battery::CRITICAL_CHARGE };
        }
        m_state = State::eFlying;
    }

    bool Quadcopter::CanTakeOff() const
    {
        return m_state == State::eOnBase && m_battery->GetCharge() >= Battery::MIN_CHARGE;
    }

    std::string Quadcopter::ToReportLine() const
    {
        return m_number + ";DRONE;" + std::to_string(m_battery->GetCharge())
            + "%;" + GetStateStr();
    }
}