#pragma once
#include "BaseSensor.hpp"
#include <memory>
#include <string>
#include <string_view>


namespace firewatch {
	class TempSensor : public BaseSensor
	{
	private:
		double m_temperature{ 0.0 };
		mutable double m_lastValue{ 0.0 };

	public:
		//конструктор
		TempSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter, double temperature);
		//деструктор переопределенный под этот класс
		~TempSensor() override;

		//виртуальное свойство
		double ReadValue() const override;

		void SetTemperature(double temperature);

		std::string Describe() const override;//с вызовом базового

		//для глубокого копирования
		std::unique_ptr<BaseSensor> Clone() const override;

	};
}

