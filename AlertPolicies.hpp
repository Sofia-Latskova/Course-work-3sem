#pragma once

#include "IAlertPolicy.hpp"

namespace firewatch {

    class SirenAlert final : public IAlertPolicy {
    public:
        [[nodiscard]] std::string AlertChannel() const noexcept override { return "SIREN"; }
        [[nodiscard]] std::string FormatAlert(const ScanResult& scan) const override;
    };

    class SilentLogAlert final : public IAlertPolicy {
    public:
        [[nodiscard]] std::string AlertChannel() const noexcept override { return "SILENT_LOG"; }
        [[nodiscard]] std::string FormatAlert(const ScanResult& scan) const override;
    };

    class EmercomCallAlert final : public IAlertPolicy {
    public:
        [[nodiscard]] std::string AlertChannel() const noexcept override { return "CALL_EMERCOM"; }
        [[nodiscard]] std::string FormatAlert(const ScanResult& scan) const override;
    };

}