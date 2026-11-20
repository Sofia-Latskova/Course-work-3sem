#include "AlertPolicies.hpp"
#include <sstream>

namespace firewatch {

    std::string SirenAlert::FormatAlert(const ScanResult& scan) const {
        std::ostringstream os;
        os << "[СИРЕНА] ТРЕВОГА! Показания: ";
        bool first = true;
        for (const auto& r : scan.readings) {
            if (!r.reliable) continue;
            if (!first) os << ", ";
            os << r.kind << "=" << r.value;
            first = false;
        }
        return os.str();
    }

    std::string SilentLogAlert::FormatAlert(const ScanResult& scan) const {
        std::ostringstream os;
        os << "[ЛОГ] Отклонение: ";
        bool first = true;
        for (const auto& r : scan.readings) {
            if (!r.reliable) continue;
            if (!first) os << "; ";
            os << r.serial << " " << r.kind << "=" << r.value;
            first = false;
        }
        return os.str();
    }

    std::string EmercomCallAlert::FormatAlert(const ScanResult& scan) const {
        std::ostringstream os;
        os << "[МЧС] Вызов бригады. Показания: ";
        bool first = true;
        for (const auto& r : scan.readings) {
            if (!first) os << ", ";
            os << r.serial << "(" << r.kind << ")=" << r.value;
            if (!r.reliable) os << " [НЕДОСТОВЕРНО]";
            first = false;
        }
        os << ". Требуется выезд.";
        return os.str();
    }

}