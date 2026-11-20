#pragma once

#include "Interfaces.hpp"
#include <string>
#include <string_view>

namespace firewatch
{
	class Battery: public IDiagnosable, public IReportable
	{
	public:
		static constexpr int MIN_CHARGE = 20;
		static constexpr int CRITICAL_CHARGE = 10;
		static constexpr int MAX_PERCENT = 100;
	private:
		int m_chargePercent{ MAX_PERCENT };
		int m_capacityPercent{ MAX_PERCENT };
		std::string m_serial{ "BAT-000" };
	public:
		Battery();
		Battery(int chargePercent, int capacityPercent, std::string_view serial);
		~Battery() noexcept;

		Battery(const Battery&) = default;
		Battery& operator=(const Battery&) = default;
		Battery(Battery&&) noexcept = default;
		Battery& operator=(Battery&&) noexcept = default;

		[[nodiscard]] int GetCharge() const;
		[[nodiscard]] int GetCapacity() const;
		[[nodiscard]] std::string GetSerial() const;

		//разряд в полете
		void Consume(int percent);

		//поставить на зарядку
		void Charge(int percent);

		//проверка заряд в диапазоне
		[[nodiscard]] bool IsValid() const;

		//IDiagnosable 
		[[nodiscard]] bool IsOperational() const override
		{
			return m_chargePercent >= CRITICAL_CHARGE;
		}
		[[nodiscard]] int HealthPercent() const override
		{
			return m_chargePercent;
		}

		//IReportable
		[[nodiscard]] std::string_view ReportSection() const override
		{
			return "ПИТАНИЕ";
		}
		[[nodiscard]] std::string ToReportLine() const override;


	};
}










