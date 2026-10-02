#pragma once

#include "battery.hpp"

#include <string>
#include <string_view>

namespace firewatch {
	class quadcopter {
	public:
        enum class State
        {
            eOnBase,
            eFlying,
            eScanning,
            eReturning
        };
    private:
        std::string m_number{ "DRN-000" };
        State m_state{ State::eOnBase };
        battery m_battery;
    public:
        quadcopter() = default;
        quadcopter(std::string_view number, battery Battery);
        ~quadcopter();

        [[nodiscard]] std::string GetNumber() const;
        [[nodiscard]] State GetState() const;
        [[nodiscard]] std::string GetStateStr() const;
        [[nodiscard]] int GetBatteryCharge() const;

        //взлет
        bool TakeOff();

        //посадка
        void Land();

        //скан
        void Scan();

        //Заряд ниже мин
        bool CanTakeOff() const;


	};


}