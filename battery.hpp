#pragma once
#include <string>
#include <string_view>

namespace firewatch
{
	class battery
	{
	public:
		static constexpr int min_charge = 20;
		static constexpr int critical_charge = 10;
		static constexpr int max_charge = 100;
	private:
		int m_charge_percent{ max_charge };
		int m_capacity_percent{ max_charge };
		std::string m_serial{ "BAT-000" };
	public:
		battery() = default;
		battery(int chargePercent, int capacityPercent, std::string_view serial);
		~battery();

		[[nodiscard]] int GetCharge() const;
		[[nodiscard]] int GetCapacity() const;
		[[nodiscard]] std::string GetSerial() const;

		//разряд в полете
		void Consume(int percent);

		//поставить на зарядку
		void Charge(int percent);

		//проверка заряд в диапазоне
		[[nodiscard]] bool Isvalid() const;
	};
}










