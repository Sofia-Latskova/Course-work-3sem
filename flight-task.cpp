#include "flight-task.hpp"

#include <iostream>

namespace firewatch
{

    FlightTask::FlightTask(std::string_view zoneName, const std::shared_ptr<Quadcopter> drone)
        : m_zoneName{ zoneName },
        m_drone{ std::move(drone) }
    {
        std::cout << "[FlightTask] Создано задание для зоны " << m_zoneName << "\n";
    }

    FlightTask::~FlightTask()
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
            std::cout << "[FlightTask] Отказ запуска: дрон не готов или занят\n";
            return false;
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
            std::cout << "[FlightTask] Завершить можно только выполняемое задание\n";
            return;
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
        if (auto drone = m_drone.lock())
        {
            std::cout << "\n--- Сканирование точек ---\n";
            drone->Scan();
            drone->Scan();
            drone->Scan();
            std::cout << "Заряд: " << drone->GetBatteryCharge() << "%\n";
        }
        else
        {
            std::cout << "[FlightTask] Ошибка: дрон потерян, миссия невозможна.\n";
        }
    }
}