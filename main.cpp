#include "TempSensor.hpp"
#include "SmokeSensor.hpp"
#include "FlameSensor.hpp"
#include "quadcopter.hpp"
#include "battery.hpp"
#include "flight-task.hpp"
#include "Interfaces.hpp"
#include "ScanResult.hpp"
#include "exceptions.hpp"
#include "AlertPolicies.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <map>

using namespace firewatch;

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "========== ЛР 5. Полиморфизм ==========\n";
    auto drone = std::make_shared<Quadcopter>("DRN-001", 80, 100, "BAT-001");
    auto temp = std::make_shared<TempSensor>("TS-001", drone, 45.0);
    auto smoke = std::make_shared<SmokeSensor>("SS-001", drone, 0.2);
    auto flame = std::make_shared<FlameSensor>("FS-001", drone, 10);

//работа через интерфейсы
    std::cout << "\n===== работа через интерфейсы =====\n";

    std::cout << "\n-- IDiagnosable --\n";
    std::vector<const IDiagnosable*> diag{ temp.get(), drone.get() };
    for (const auto* u : diag)
        std::cout << "  Health = " << u->HealthPercent() << "%"
        << (u->IsOperational() ? " [OK]" : " [FAIL]") << "\n";
    std::cout << "\n-- IReportable --\n";
    std::vector<const IReportable*> report{ temp.get(), smoke.get(), flame.get(), drone.get()};
    std::map<std::string, std::vector<std::string>> bySection;
    for (const auto* item : report)
        bySection[std::string(item->ReportSection())].push_back(item->ToReportLine());
    for (const auto& [section, lines] : bySection)
    {
        std::cout << "  === " << section << " ===\n";
        for (const auto& line : lines) std::cout << "    " << line << "\n";
    }

//раннее и позднее связывание
    std::cout << "\n=== раннее/позднее связывание =====\n";
    std::unique_ptr<BaseSensor> s = std::make_unique<TempSensor>("TS-002", drone, 45.0);
    std::cout << "  GetKind()     (невиртуальный, раннее):  " << s->GetKind() << "\n";
    std::cout << "  GetTypeName() (виртуальный, позднее):   " << s->GetTypeName() << "\n";

//dynamic_cast
    std::cout << "\n====dynamic_cast =====\n";
    std::vector<const IDiagnosable*> units{ temp.get(), smoke.get(), flame.get(), drone.get() };
    for (const auto* u : units)
    {
        if (auto* sc = dynamic_cast<const IScannable*>(u))
        {
            try {
                std::cout << "  [OK]   " << sc->SensorSerial()
                    << " -> " << sc->ReadValue() << "\n";
            }
            catch (const SensorFailure& e) {
                std::cout << "  [FAIL] " << e.what() << "\n";
            }
        }
        else
        {
            std::cout << "  [SKIP] объект не реализует IScannable\n";
        }
    }

//конфигурирование 
    std::cout << "\n====конфигурирование=====\n";
    FlightTask task{ "ZONE-A", drone, std::make_unique<SirenAlert>() };
    ScanResult scan;
    scan.readings = {
        { "TS-001", "температура", 55.0, true },
        { "SS-001", "дым",         0.7,  true },
        { "FS-001", "пламя",       45.0, true },
    };
    scan.hasReliableData = true;
    auto printAlert = [&](const char* label) {
        auto [channel, message] = task.TriggerAlert(scan);
        std::cout << "  " << label << " -> [" << channel << "] " << message << "\n";
        };
    printAlert("Сирена       ");
    task.SetPolicy(std::make_unique<SilentLogAlert>());
    printAlert("Тихий лог    ");
    task.SetPolicy(std::make_unique<EmercomCallAlert>());
    printAlert("Вызов МЧС    ");

//исключения

    std::cout << "\n=====исключения =====\n";
    std::cout << "\n Успешный прогон:\n"; 
    auto drone2 = std::make_shared<Quadcopter>("DRN-002", 80, 100, "BAT-002");
    FlightTask ok{ "ZONE-OK", drone2, std::make_unique<SirenAlert>() };
    try
    {
        ok.Start();
        drone2->Scan();
        drone2->Scan();
        ok.Complete();
    }
    catch (const std::exception& e)
    {
        std::cout << "  Неожиданная ошибка: " << e.what() << "\n";
    }
    auto lowBat = std::make_shared<Quadcopter>("DRN-LOW", 5, 100, "BAT-LOW");
    FlightTask bad{ "ZONE-LOW", lowBat, std::make_unique<SirenAlert>() };
    std::cout << "\n Низкий заряд при взлёте:\n";
    try
    {
        bad.Start();
    }
    catch (const LowBatteryError& e)
    {
        std::cout << "  [LowBatteryError] " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  [std::exception] " << e.what() << "\n";
    }

    std::cout << "\n Сканирование не в полёте:\n";
    auto drone3 = std::make_shared<Quadcopter>("DRN-003", 80, 100, "BAT-003");
    try
    {
        drone3->Scan();
    }
    catch (const InvalidMissionState& e)
    {
        std::cout << "  [InvalidMissionState] " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  [std::exception] " << e.what() << "\n";
    }

    std::cout << "\nПрервать неначатое задание:\n";
    auto drone4 = std::make_shared<Quadcopter>("DRN-004", 80, 100, "BAT-004");
    FlightTask t{ "ZONE-4", drone4, std::make_unique<SirenAlert>() };
    try
    {
        t.Abort("тест");
    }
    catch (const InvalidMissionState& e)
    {
        std::cout << "  [InvalidMissionState] " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  [std::exception] " << e.what() << "\n";
    }

    std::cout << "\n Неисправный датчик:\n";  
    auto broken = std::make_shared<TempSensor>("TS-BROKEN", drone, 45.0);
    try
    {
        broken->Break();
        broken->ReadValue();
    }
    catch (const SensorFailure& e)
    {
        std::cout << "  [SensorFailure] " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  [std::exception] " << e.what() << "\n";
    }

    std::cout << "\n Порядок catch (частный -> общий):\n"; 
    auto low = std::make_shared<Quadcopter>("DRN-X", 5, 100, "BAT-X");
    try
    {       
        low->TakeOff();
    }
    catch (const SensorFailure& e) { std::cout << "  SensorFailure: " << e.what() << "\n"; }
    catch (const LowBatteryError& e) { std::cout << "  LowBatteryError: " << e.what() << "\n"; }
    catch (const BatteryOverDischarge& e) { std::cout << "  BatteryOverDischarge: " << e.what() << "\n"; }
    catch (const InvalidMissionState& e) { std::cout << "  InvalidMissionState: " << e.what() << "\n"; }
    catch (const FirewatchError& e) { std::cout << "  FirewatchError: " << e.what() << "\n"; }
    catch (const std::exception& e) { std::cout << "  std::exception: " << e.what() << "\n"; }

    std::cout << "\n";
    std::cout << "\n=== КОНЕЦ ===\n";
}










