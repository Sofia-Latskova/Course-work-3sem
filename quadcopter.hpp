#pragma once
#include "Interfaces.hpp"
#include "battery.hpp"

#include <string>
#include <string_view>
#include <memory>

namespace firewatch 
{
	class Quadcopter : public IDiagnosable, public IReportable
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
        std::unique_ptr<Battery> m_battery;
    public:
        Quadcopter(std::string_view number, int chargePercent, int capacityPercent, std::string_view batterySerial);
        ~Quadcopter() noexcept;
        //конструктор копирования
        Quadcopter(const Quadcopter& other);
        Quadcopter& operator=(const Quadcopter& other);
        Quadcopter(Quadcopter&&) noexcept = default;
        Quadcopter& operator=(Quadcopter&&) noexcept = default;

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

        //IDiagnosable
        [[nodiscard]] bool IsOperational() const  override
        {
            return m_state == State::eOnBase
                && m_battery->GetCharge() >= Battery::MIN_CHARGE;
        }
        [[nodiscard]] int HealthPercent() const  override
        {
            return m_battery->GetCharge();
        }

        //IReportable
        [[nodiscard]] std::string_view ReportSection() const  override
        {
            return "ДРОН";
        }
        [[nodiscard]] std::string ToReportLine() const override;
	};
}