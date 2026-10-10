#pragma once

#include "quadcopter.hpp"

#include <string>
#include <string_view>

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
		Quadcopter* m_drone{ nullptr };
	public:
		FlightTask() = default;
		FlightTask(std::string_view zoneName, Quadcopter* drone);
		~FlightTask();
		[[nodiscard]] std::string GetZoneName() const;
		[[nodiscard]] Status      GetStatus() const;
		[[nodiscard]] std::string GetStatusStr() const;

		//начать задание 
		bool Start();

		//прервать
		void Abort(std::string_view reason);

		//завершить
		void Complete();

		//нельзя начать если дрон занят
		[[nodiscard]] bool CanStart() const;

	};
}











