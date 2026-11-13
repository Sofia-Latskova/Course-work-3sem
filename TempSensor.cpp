#include "TempSensor.hpp"

#include <iostream>
#include <cstdlib>
#include <utility>
#include <format>

namespace firewatch {
	//конструктор
	TempSensor::TempSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter, double temperature)
	    : BaseSensor(serial, std::move(quadcopter)) // базовый конструктор
		, m_temperature{ temperature }
	{
	   std::cout << "[TempSensor] Создан датчик темп. " << GetSerial() << "\n";
	}

	//деструктор
	TempSensor::~TempSensor()
	{
		std::cout << "[TempSensor] Уничтожен датчик темп. " << GetSerial() << "\n";
	}

	//снятие показаний
	double TempSensor::ReadValue() const
	{
		if (WearDown()) return -999.0; //если сломан или закончился ресурс
		double noise = (std::rand() % 100 - 50) / 100.0; //погрешность
		m_lastValue = m_temperature + noise; 
		return m_lastValue;
	}

	void TempSensor::SetTemperature(double temperature) {
		m_temperature = temperature;
		std::cout << "[TempSensor] " << GetSerial()
			<< ": установлена температура " << temperature << " °C\n";
	}

	std::string TempSensor::Describe() const
	{
		std::string result = BaseSensor::Describe() + "\n";
		result += "Тип: Датчик температуры\n";
		if (IsBroken())return result;
		result += std::format("Показания: {:.2f} °C\n", m_lastValue);
		result += (m_lastValue > 50.0) ? "Статус: [ОПАСНО: ПЕРЕГРЕВ]"
			: "Статус: [В норме]";
		return result;
	}

	std::unique_ptr<BaseSensor> TempSensor::Clone() const
	{
		auto copy = std::make_unique<TempSensor>(*this);                  // shallow
		copy->m_quadcopter = std::make_shared<Quadcopter>(*m_quadcopter); // ← deep
		return copy;
	}

}













