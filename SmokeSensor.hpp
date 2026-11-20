#pragma once
#include "BaseSensor.hpp"
#include "Interfaces.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace firewatch {
	class SmokeSensor final : public BaseSensor, public IScannable
	{
	private:
		double m_smokeLevel{ 0.0 };
		mutable int m_warningCount{ 0 };   // Счетчик накопления тревог
		mutable int m_warmupCounter{ 0 };  // Счетчик прогрева
		mutable double m_lastValue{ 0.0 };
	public:
		SmokeSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter, double smokeLevel);

		~SmokeSensor() override;

		//IScannable
		double ReadValue() const override;
		[[nodiscard]] std::string_view SensorSerial() const  override
		{
			return GetSerial();
		}

		[[nodiscard]] std::string GetKind() const { return "датчик дыма"; }
		[[nodiscard]] std::string GetTypeName() const override { return "дым"; }

		void SetSmokeLevel(double level);

		std::string Describe() const override; //без вызова базы

		std::unique_ptr<BaseSensor> Clone() const override;
	};

}