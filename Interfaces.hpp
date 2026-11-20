#pragma once

#include <string>
#include <string_view>

namespace firewatch {
    // IScannable умею снимать показание датчика
    class IScannable {
    public:
        virtual ~IScannable() = default;

        // Бросает SensorFailure, если датчик неисправен.
        virtual double ReadValue() const = 0;

        [[nodiscard]] virtual std::string_view SensorSerial() const = 0;
    };

    // IDiagnosable умею сообщать о своей исправности
    class IDiagnosable {
    public:
        virtual ~IDiagnosable() = default;

        [[nodiscard]] virtual bool IsOperational() const  = 0;
        [[nodiscard]] virtual int  HealthPercent() const  = 0;
    };

    // IReportable умею попасть в итоговый отчёт
    class IReportable {
    public:
        virtual ~IReportable() = default;

        // Имя секции: "ДАТЧИКИ", "ПИТАНИЕ", "ДРОН", "МИССИЯ".
        [[nodiscard]] virtual std::string_view ReportSection() const  = 0;

        // Одна строка без перевода строки в конце.
        [[nodiscard]] virtual std::string ToReportLine() const = 0;
    };
}