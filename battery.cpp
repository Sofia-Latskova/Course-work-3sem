#include "battery.hpp"

#include <iostream>


namespace firewatch 
{
	Battery::Battery()
		: m_chargePercent{ MAX_PERCENT }
		, m_capacityPercent{ MAX_PERCENT }
		, m_serial{ "BAT-000" }
	{
		std::cout << "[Battery] Создан " << m_serial << " (по умолчанию)\n";
	}

	Battery::Battery(int chargePercent, int capacityPercent, std::string_view serial)
		:m_chargePercent{ chargePercent },
		m_capacityPercent{ capacityPercent },
		m_serial{ serial } 
	{
		if (!IsValid()) {
			std::cout << "[Battery] Некорректный заряд, сброшен в 0\n";
			m_chargePercent = 0;
		}
		std::cout << "[Battery] Создан " << m_serial << " заряд=" << m_chargePercent << "%\n";
	}

	Battery::~Battery() 
	{
		std::cout << "[Battery] Уничтожен " << m_serial << "\n\n";
	}

	int Battery::GetCharge() const
	{
		return m_chargePercent;
	}

	int Battery::GetCapacity() const
	{
		return m_capacityPercent;
	}

	std::string Battery::GetSerial() const
	{
		return m_serial;
	}

	void Battery::Consume(int percent)
	{
		if (percent <= 0)
		{
			return;
		}
		m_chargePercent -= percent;
		if (m_chargePercent < 0)
		{
			m_chargePercent = 0;
		}
		std::cout << "[Battery] " << m_serial << " разряжен до " << m_chargePercent << "%\n";
	}

	void Battery::Charge(int percent)
	{
		if (percent <= 0)
		{
			return;
		}
		m_chargePercent += percent;
		if (m_chargePercent > m_capacityPercent)
		{
			m_chargePercent = m_capacityPercent;
		}
		std::cout << "[Battery] " << m_serial<< " заряжен до " << m_chargePercent << "%\n";
	}

	bool Battery::IsValid() const
	{
		return m_chargePercent >= 0 && m_chargePercent <= m_capacityPercent;
	}
}