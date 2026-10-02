#include "flight-task.hpp"

#include <iostream>

namespace firewatch
{

    flightask::flightask(std::string_view zoneName, quadcopter* drone): m_zoneName{ zoneName }, m_drone{ drone }
    {
        std::cout << "[FlightTask] Создано задание для зоны " << m_zoneName << "\n";
    }

    flightask::~flightask()
    {
        std::cout << "[FlightTask] Уничтожено задание для зоны " << m_zoneName << "\n";
    }

    std::string flightask::GetZoneName() const
    {
        return m_zoneName;
    }

    flightask::Status flightask::GetStatus() const
    {
        return m_status;
    }

    std::string flightask::GetStatusStr() const
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

    bool flightask::CanStart() const
    {
        if (m_drone == nullptr)
        {
            return false;
        }
        if (m_drone->GetState() != quadcopter::State::eOnBase)
        {
            return false;
        }
        return m_status == Status::eCreated;
    }

    bool flightask::Start()
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

    void flightask::Abort(std::string_view reason)
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

    void flightask::Complete()
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