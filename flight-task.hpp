#pragma once

#include "quadcopter.hpp"

#include <string>
#include <string_view>

namespace firewatch {
	class flightask {
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
		quadcopter* m_drone{ nullptr };
	public:
		flightask() = default;
		flightask(std::string_view zoneName, quadcopter* drone);
		~flightask();
		[[nodiscard]] std::string GetZoneName() const;
		[[nodiscard]] Status      GetStatus() const;
		[[nodiscard]] std::string GetStatusStr() const;

		//начать задание 
		bool Start();

		//прервать
		void Abort();

		//завершить
		void Complete();

		//нельзя начать если дрон занят
		bool CanStart() const;

	};
}











