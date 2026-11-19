#pragma once
#include "BaseSensor.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace firewatch {
	class SmokeSensor final : public BaseSensor
	{
	private:
		double m_smokeLevel{ 0.0 };
		mutable int m_warningCount{ 0 };   // Счетчик накопления тревог
		mutable int m_warmupCounter{ 0 };  // Счетчик прогрева
		mutable double m_lastValue{ 0.0 };
	public:
		SmokeSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter, double smokeLevel);

		~SmokeSensor() override;

		double ReadValue() const override;

		void SetSmokeLevel(double level);

		std::string Describe() const override; //без вызова базы

		std::unique_ptr<BaseSensor> Clone() const override;
	};

}