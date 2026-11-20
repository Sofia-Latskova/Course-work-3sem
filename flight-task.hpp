#pragma once

#include "quadcopter.hpp"
#include "Interfaces.hpp"
#include "IAlertPolicy.hpp"
#include "ScanResult.hpp"
#include <string>
#include <string_view>
#include <memory>

namespace firewatch 
{
	class FlightTask 
	{
	public:
		enum class Status
		{
			eCreated,
			eInProgress,
			eCompleted,
			eAborted
		};
	private:
		std::string m_zoneName{ "ZONE-0" };
		Status      m_status{ Status::eCreated };
		std::weak_ptr<Quadcopter> m_drone;
		std::unique_ptr<IAlertPolicy> m_policy;

	public:
		FlightTask() = default;
		FlightTask(std::string_view zoneName, const std::shared_ptr<Quadcopter> drone, std::unique_ptr<IAlertPolicy> policy);
		~FlightTask() noexcept;
		// Копирование запрещено
		FlightTask(const FlightTask&) = delete;
		FlightTask& operator=(const FlightTask&) = delete;

		// Перемещение разрешено и noexcept
		FlightTask(FlightTask&&) noexcept = default;
		FlightTask& operator=(FlightTask&&) noexcept = default;

		[[nodiscard]] std::string GetZoneName() const;
		[[nodiscard]] Status      GetStatus() const;
		[[nodiscard]] std::string GetStatusStr() const;
		//начать задание 
		bool Start();

		//прервать
		void Abort(std::string_view reason);

		//завершить
		void Complete();

		//работа с дроном
		void PerformMission();

		//нельзя начать если дрон занят
		[[nodiscard]] bool CanStart() const;

		void SetPolicy(std::unique_ptr<IAlertPolicy> policy);

		[[nodiscard]] std::pair<std::string, std::string> TriggerAlert(const ScanResult& scan) const;

	};
}











