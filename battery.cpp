#include "battery.hpp"

#include <iostream>


namespace firewatch {

	battery::battery(int chargePercent, int capacityPercent, std::string_view serial)
		:m_charge_percent{ chargePercent },
		m_capacity_percent{ capacityPercent },
		m_serial{ serial } 
	{
		if (!Isvalid()) {
			std::cout << "[Battery] Некорректный заряд, сброшен в 0\n";
			m_charge_percent = 0;
		}
		std::cout << "[Battery] Создан " << m_serial << " заряд=" << m_charge_percent << "%\n";
	}

	battery::~battery() 
	{
		std::cout << "[Battery] Уничтожен " << m_serial << "\n";
	}

	int battery::GetCharge() const
	{
		return m_charge_percent;
	}

	int battery::GetCapacity() const
	{
		return m_capacity_percent;
	}

	std::string battery::GetSerial() const
	{
		return m_serial;
	}

	void battery::Consume(int percent)
	{
		if (percent <= 0)
		{
			return;
		}
		m_charge_percent -= percent;
		if (m_charge_percent < 0)
		{
			m_charge_percent = 0;
		}
		std::cout << "[Battery] " << m_serial << " разряжен до " << m_charge_percent << "%\n";
	}

	void battery::Charge(int percent)
	{
		if (percent <= 0)
		{
			return;
		}
		m_charge_percent += percent;
		if (m_charge_percent > m_capacity_percent)
		{
			m_charge_percent = m_capacity_percent;
		}
		std::cout << "[Battery] " << m_serial<< " заряжен до " << m_charge_percent << "%\n";
	}

	bool battery::IsValid() const
	{
		return m_charge_percent >= 0 && m_charge_percent <= m_capacity_percent;
	}
}