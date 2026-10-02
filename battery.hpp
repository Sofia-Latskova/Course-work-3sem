#pragma once
#include <string>
#include <string_view>

namespace firewatch
{
	class Battery
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
		Battery() = default;
		Battery(int chargePercent, int capacityPercent, std::string_view serial);
		~Battery();

		[[nodiscard]] int GetCharge() const;
		[[nodiscard]] int GetCapacity() const;
		[[nodiscard]] std::string GetSerial() const;

		//разряд в полете
		void Consume(int percent);

		//поставить на зарядку
		void Charge(int percent);

		//проверка заряд в диапазоне
		[[nodiscard]] bool IsValid() const;
	};
}










