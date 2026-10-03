#pragma once

#include "battery.hpp"

#include <string>
#include <string_view>

namespace firewatch 
{
	class Quadcopter 
    {
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
        Battery* m_battery;
    public:
        Quadcopter() = default;
        Quadcopter(std::string_view number, Battery* battery);
        ~Quadcopter();

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
        [[nodiscard]] bool CanTakeOff() const;
	};
}