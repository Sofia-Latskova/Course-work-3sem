#pragma once
#include "ScanResult.hpp"
#include <string>

namespace firewatch {

    class IAlertPolicy {
    public:
        virtual ~IAlertPolicy() = default;

        // Канал тревоги — короткий идентификатор для протокола:
        // "SIREN", "SILENT_LOG", "CALL_MCHS".
        [[nodiscard]] virtual std::string AlertChannel() const noexcept = 0;

        // Текст сообщения для протокола и консоли.
        [[nodiscard]] virtual std::string FormatAlert(const ScanResult& scan) const = 0;
    };

}