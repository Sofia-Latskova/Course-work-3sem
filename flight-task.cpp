#include "flight-task.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <utility>

namespace firewatch
{

    FlightTask::FlightTask(std::string_view zoneName, const std::shared_ptr<Quadcopter> drone, std::unique_ptr<IAlertPolicy> policy)
        : m_zoneName{ zoneName },
        m_drone{ std::move(drone) }
        , m_policy{ std::move(policy) }
    {
        if (!m_policy)
        {
            throw InvalidMissionState{ "политика сигнализации не задана" };
        }
        std::cout << "[FlightTask] Создано задание для зоны " << m_zoneName << " (политика: " << m_policy->AlertChannel() << "\n";
    }

    FlightTask::~FlightTask() noexcept
    {
        std::cout << "[FlightTask] Уничтожено задание для зоны " << m_zoneName << "\n";
    }

    std::string FlightTask::GetZoneName() const
    {
        return m_zoneName;
    }

    FlightTask::Status FlightTask::GetStatus() const
    {
        return m_status;
    }

    std::string FlightTask::GetStatusStr() const
    {
        switch (m_status)
        {
        case Status::eCreated:    return "создано";
        case Status::eInProgress: return "выполняется";
        case Status::eCompleted:  return "завершено";
        case Status::eAborted:    return "прервано";
        }
        return "неизвестно";
    }

    bool FlightTask::CanStart() const
    {
        if (auto drone = m_drone.lock())
        {
            return drone->GetState() == Quadcopter::State::eOnBase
                && m_status == Status::eCreated;
        }
        return false;
    }

    bool FlightTask::Start()
    {
        if (!CanStart())
        {
            throw InvalidMissionState{
                "нельзя начать: дрон не готов или задание не в статусе 'создано'"
            };
        }
        if (auto drone = m_drone.lock())
        {
            if (!drone->TakeOff()) return false;
        }
        m_status = Status::eInProgress;
        std::cout << "[FlightTask] Задание в зоне " << m_zoneName << " начато\n";
        return true;
    }

    void FlightTask::Abort(std::string_view reason)
    {
        if (m_status != Status::eInProgress)
        {
            std::cout << "[FlightTask] Прервать можно только выполняемое задание\n";
            return;
        }
        if (auto drone = m_drone.lock())
        {
            drone->Land();
        }
        m_status = Status::eAborted;
        std::cout << "[FlightTask] Задание прервано. Причина: " << reason << "\n";
    }

    void FlightTask::Complete()
    {
        if (m_status != Status::eInProgress)
        {
            throw InvalidMissionState{
                "завершить можно только выполняемое задание (текущий статус: "
                + GetStatusStr() + ")" };
        }
        if (auto drone = m_drone.lock())
        {
            drone->Land();
        }
        m_status = Status::eCompleted;
        std::cout << "[FlightTask] Задание в зоне " << m_zoneName << " завершено\n";
    }

    void FlightTask::PerformMission()
    {
        auto drone = m_drone.lock();
        if (!drone)
        {
            throw InvalidMissionState{ "дрон потерян, миссия невозможна" };
        }
            std::cout << "\n--- Сканирование точек ---\n";
            drone->Scan();
            drone->Scan();
            drone->Scan();
            std::cout << "Заряд: " << drone->GetBatteryCharge() << "%\n";
  
    }

    void FlightTask::SetPolicy(std::unique_ptr<IAlertPolicy> policy)
    {
        if (!policy)
        {
            throw InvalidMissionState{ "нельзя установить пустую политику" };
        }
        m_policy = std::move(policy);
        std::cout << "[FlightTask] Политика заменена на: "
            << m_policy->AlertChannel() << "\n";
    }

    std::pair<std::string, std::string>
        FlightTask::TriggerAlert(const ScanResult& scan) const
    {
        if (!m_policy)
        {
            throw InvalidMissionState{ "политика сигнализации не задана" };
        }
        return { m_policy->AlertChannel(), m_policy->FormatAlert(scan) };
    }
}