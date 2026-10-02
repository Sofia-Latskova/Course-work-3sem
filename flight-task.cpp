#include "flight-task.hpp"

#include <iostream>

namespace firewatch
{

    FlightTask::FlightTask(std::string_view zoneName, Quadcopter* drone)
        : m_zoneName{ zoneName },
        m_drone{ drone }
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
        if (m_drone == nullptr)
        {
            return false;
        }
        if (m_drone->GetState() != Quadcopter::State::eOnBase)
        {
            return false;
        }
        return m_status == Status::eCreated;
    }

    bool FlightTask::Start()
    {
        if (!CanStart())
        {
            std::cout << "[FlightTask] Отказ запуска: дрон не готов или занят\n";
            return false;
        }
        if (!m_drone->TakeOff())
        {
            std::cout << "[FlightTask] Отказ запуска: дрон не смог взлететь\n";
            return false;
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
        m_drone->Land();
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
        m_drone->Land();
        m_status = Status::eCompleted;
        std::cout << "[FlightTask] Задание в зоне " << m_zoneName << " завершено\n";
    }

}