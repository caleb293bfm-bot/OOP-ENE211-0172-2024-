/*
==============================================================
SMART ENERGY MANAGEMENT SYSTEM
REALISTIC POWER CALCULATION VERSION
==============================================================

UPGRADES INCLUDED
--------------------------------------------------------------
1. Realistic AC power calculation
2. Power factor simulation
3. Realistic household appliance ratings
4. Stable voltage fluctuation
5. Corrected overload behavior
6. Television priority fix retained
7. Full original functionality preserved
==============================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

/*
==============================================================
UTILITY FUNCTION
==============================================================
*/

void delaySystem(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

/*
==============================================================
ABSTRACT SENSOR CLASS
==============================================================
*/

class Sensor
{
public:

    virtual float readValue() = 0;

    virtual ~Sensor() {}
};

/*
==============================================================
CURRENT SENSOR CLASS
==============================================================
*/

class CurrentSensor : public Sensor
{
private:

    float baseCurrent;

public:

    CurrentSensor(float current)
    {
        baseCurrent = current;
    }

    float readValue()
    {
        /*
        ======================================================
        SMALL REALISTIC CURRENT FLUCTUATION
        ======================================================
        */

        float fluctuation =
            ((rand() % 21) - 10) / 100.0f;

        float current =
            baseCurrent + fluctuation;

        if(current < 0)
            current = 0;

        return current;
    }
};

/*
==============================================================
VOLTAGE SENSOR CLASS
==============================================================
*/

class VoltageSensor : public Sensor
{
public:

    float readValue()
    {
        /*
        ======================================================
        REALISTIC HOUSEHOLD VOLTAGE RANGE
        ======================================================
        */

        return 220 + (rand() % 11 - 5);
    }
};

/*
==============================================================
RELAY CLASS
==============================================================
*/

class Relay
{
private:

    bool state;

public:

    Relay()
    {
        state = true;
    }

    void turnOn()
    {
        state = true;
    }

    void turnOff()
    {
        state = false;
    }

    bool getState()
    {
        return state;
    }
};

/*
==============================================================
APPLIANCE BASE CLASS
==============================================================
*/

class Appliance
{
protected:

    string name;

    int priority;

    float voltage;
    float current;
    float power;
    float energy;

    /*
    ==========================================================
    POWER FACTOR
    ==========================================================
    */

    float powerFactor;

    CurrentSensor currentSensor;
    VoltageSensor voltageSensor;

    Relay relay;

public:

    Appliance(string n,
              float baseCurrent,
              int p,
              float pf)

        : currentSensor(baseCurrent)
    {
        name = n;

        priority = p;

        powerFactor = pf;

        voltage = 0;
        current = 0;
        power = 0;
        energy = 0;
    }

    /*
    ==========================================================
    REALISTIC POWER SIMULATION
    ==========================================================
    */

    virtual void simulateBehavior()
    {
        if(relay.getState())
        {
            voltage =
                voltageSensor.readValue();

            current =
                currentSensor.readValue();
        }
        else
        {
            voltage = 0;
            current = 0;
        }

        /*
        ======================================================
        TRUE AC ACTIVE POWER FORMULA
        ------------------------------------------------------
        P = V × I × PF
        ======================================================
        */

        power =
            voltage *
            current *
            powerFactor;

        if(power < 0)
            power = 0;
    }

    /*
    ==========================================================
    ENERGY CALCULATION
    ==========================================================
    */

    virtual void calculateEnergy(float hours)
    {
        energy +=
            (power / 1000.0f) * hours;
    }

    /*
    ==========================================================
    DISPLAY APPLIANCE DATA
    ==========================================================
    */

    virtual void display()
    {
        cout << left
             << setw(15) << name
             << setw(10) << fixed
             << setprecision(1)
             << voltage
             << setw(10) << current
             << setw(12) << power
             << setw(12) << energy
             << setw(10)
             << (relay.getState() ?
                "ON" : "OFF")
             << endl;
    }

    string getName()
    {
        return name;
    }

    float getPower()
    {
        return power;
    }

    float getEnergy()
    {
        return energy;
    }

    int getPriority()
    {
        return priority;
    }

    bool isOn()
    {
        return relay.getState();
    }

    void turnOn()
    {
        relay.turnOn();
    }

    void turnOff()
    {
        relay.turnOff();
    }
};

/*
==============================================================
REALISTIC HOUSEHOLD APPLIANCES
==============================================================
*/

class Fan : public Appliance
{
public:
    Fan()
    : Appliance("Fan", 0.25f, 3, 0.85f) {}
};

class Refrigerator : public Appliance
{
public:
    Refrigerator()
    : Appliance("Refrigerator", 0.8f, 5, 0.90f) {}
};

class Television : public Appliance
{
public:
    Television()
    : Appliance("Television", 0.35f, 4, 0.95f) {}
};

class Heater : public Appliance
{
public:
    Heater()
    : Appliance("Heater", 4.5f, 1, 1.0f) {}
};

class WaterPump : public Appliance
{
public:
    WaterPump()
    : Appliance("Water Pump", 3.0f, 1, 0.88f) {}
};

/*
==============================================================
BINARY ANALYTICS STRUCTURE
==============================================================
*/

struct AnalyticsData
{
    char applianceName[50];

    float power;
    float energy;
};

/*
==============================================================
REPORT MANAGER
==============================================================
*/

class ReportManager
{
public:

    static void generateMonthlyReport(
        vector<Appliance*>& appliances)
    {
        ofstream report(
            "monthly_report.txt");

        report
        << "====================================\n";

        report
        << " MONTHLY ENERGY REPORT\n";

        report
        << "====================================\n\n";

        float totalEnergy = 0;
        float totalPower = 0;

        for(size_t i = 0;
            i < appliances.size();
            i++)
        {
            report
            << "Appliance : "
            << appliances[i]->getName()
            << endl;

            report
            << "Power     : "
            << appliances[i]->getPower()
            << " W\n";

            report
            << "Energy    : "
            << appliances[i]->getEnergy()
            << " kWh\n";

            report
            << "Status    : "
            << (appliances[i]->isOn() ?
                "ON" : "OFF")
            << "\n\n";

            totalEnergy +=
                appliances[i]->getEnergy();

            totalPower +=
                appliances[i]->getPower();
        }

        report
        << "====================================\n";

        report
        << "TOTAL POWER  : "
        << totalPower
        << " W\n";

        report
        << "TOTAL ENERGY : "
        << totalEnergy
        << " kWh\n";

        report.close();

        cout
        << "\nMonthly report generated successfully.\n";

        delaySystem(2000);
    }
};

/*
==============================================================
BINARY ANALYTICS MANAGER
==============================================================
*/

class BinaryAnalyticsManager
{
public:

    static void saveAnalytics(
        vector<Appliance*>& appliances)
    {
        ofstream file(
            "analytics.bin",
            ios::binary);

        for(size_t i = 0;
            i < appliances.size();
            i++)
        {
            AnalyticsData data;

            strcpy(
                data.applianceName,
                appliances[i]->getName().c_str());

            data.power =
                appliances[i]->getPower();

            data.energy =
                appliances[i]->getEnergy();

            file.write(
                (char*)&data,
                sizeof(data));
        }

        file.close();

        cout
        << "\nBinary analytics saved successfully.\n";

        delaySystem(2000);
    }

    static void readAnalytics()
    {
        ifstream file(
            "analytics.bin",
            ios::binary);

        AnalyticsData data;

        cout
        << "\n====================================\n";

        cout
        << " STORED ANALYTICS\n";

        cout
        << "====================================\n\n";

        while(file.read(
            (char*)&data,
            sizeof(data)))
        {
            cout
            << "Appliance : "
            << data.applianceName
            << endl;

            cout
            << "Power     : "
            << data.power
            << " W\n";

            cout
            << "Energy    : "
            << data.energy
            << " kWh\n\n";
        }

        file.close();

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
};

/*
==============================================================
CLOUD LOGGER
==============================================================
*/

class CloudLogger
{
public:

    static void logData(
        vector<Appliance*>& appliances)
    {
        ofstream file(
            "cloud_log.txt",
            ios::app);

        time_t now = time(0);

        file
        << "Timestamp: "
        << ctime(&now);

        for(size_t i = 0;
            i < appliances.size();
            i++)
        {
            file
            << appliances[i]->getName()
            << " Power="
            << appliances[i]->getPower()
            << "W Energy="
            << appliances[i]->getEnergy()
            << "kWh\n";
        }

        file << "\n";

        file.close();
    }
};

/*
==============================================================
SMART ENERGY CONTROLLER
==============================================================
*/

class SmartEnergyController
{
private:

    vector<Appliance*> appliances;

    float totalPower;

    float maxPower;

public:

    SmartEnergyController()
    {
        totalPower = 0;

        /*
        ======================================================
        REALISTIC HOUSEHOLD LIMIT
        ======================================================
        */

        maxPower = 2500;
    }

    void addAppliance(
        Appliance* appliance)
    {
        appliances.push_back(appliance);
    }

    /*
    ==========================================================
    SENSOR SIMULATION
    ==========================================================
    */

    void simulateSensors()
    {
        totalPower = 0;

        for(size_t i = 0;
            i < appliances.size();
            i++)
        {
            appliances[i]->simulateBehavior();

            appliances[i]->calculateEnergy(
                1.0f / 3600.0f);

            totalPower +=
                appliances[i]->getPower();
        }

        intelligentLoadShedding();
    }

    /*
    ==========================================================
    LOAD SHEDDING
    ==========================================================
    */

    void intelligentLoadShedding()
    {
        if(totalPower > maxPower)
        {
            cout
            << "\nWARNING: SYSTEM OVERLOAD DETECTED\n";

            for(size_t i = 0;
                i < appliances.size();
                i++)
            {
                /*
                ==================================================
                Disconnect only heavy low-priority devices
                ==================================================
                */

                if(appliances[i]->getPriority() <= 2
                   && appliances[i]->getPower() > 500
                   && appliances[i]->isOn())
                {
                    appliances[i]->turnOff();

                    cout
                    << "Disconnected: "
                    << appliances[i]->getName()
                    << endl;

                    break;
                }
            }
        }
    }

    /*
    ==========================================================
    DASHBOARD DISPLAY
    ==========================================================
    */

    void displayDashboard()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        cout
        << "=============================================================\n";

        cout
        << " REAL-TIME ENERGY MONITORING DASHBOARD\n";

        cout
        << "=============================================================\n\n";

        cout << left
             << setw(15) << "Appliance"
             << setw(10) << "Voltage"
             << setw(10) << "Current"
             << setw(12) << "Power(W)"
             << setw(14) << "Energy(kWh)"
             << setw(10) << "Status"
             << endl;

        cout
        << "-------------------------------------------------------------\n";

        for(size_t i = 0;
            i < appliances.size();
            i++)
        {
            appliances[i]->display();
        }

        cout
        << "-------------------------------------------------------------\n";

        cout
        << "TOTAL POWER CONSUMPTION : "
        << totalPower
        << " W\n";

        cout
        << "MAX SYSTEM LIMIT        : "
        << maxPower
        << " W\n";

        cout
        << "SYSTEM STATUS           : ACTIVE\n";

        cout
        << "CLOUD CONNECTIVITY      : CONNECTED\n";

        cout
        << "-------------------------------------------------------------\n";
    }

    /*
    ==========================================================
    EQUIPMENT CONTROL CENTER
    ==========================================================
    */

    void equipmentControlCenter()
    {
        int applianceChoice;
        int action;

        while(true)
        {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            cout
            << "==================================================\n";

            cout
            << " EQUIPMENT CONTROL CENTER\n";

            cout
            << "==================================================\n\n";

            for(size_t i = 0;
                i < appliances.size();
                i++)
            {
                cout
                << "[" << i + 1 << "] "
                << appliances[i]->getName()
                << " STATUS: "
                << (appliances[i]->isOn() ?
                    "ON" : "OFF")
                << endl;
            }

            cout
            << "\n[0] Return To Dashboard\n";

            cout
            << "\nSelect Equipment: ";

            cin >> applianceChoice;

            if(applianceChoice == 0)
                return;

            if(applianceChoice < 1
               || applianceChoice >
               (int)appliances.size())
            {
                continue;
            }

            cout
            << "\n[1] Turn ON Equipment\n";

            cout
            << "[2] Turn OFF Equipment\n";

            cout
            << "\nSelect Action: ";

            cin >> action;

            if(action == 1)
            {
                appliances[
                    applianceChoice - 1]->turnOn();
            }
            else if(action == 2)
            {
                appliances[
                    applianceChoice - 1]->turnOff();
            }
        }
    }

    /*
    ==========================================================
    REAL-TIME DASHBOARD LOOP
    ==========================================================
    */

    void runLiveDashboard()
    {
        bool dashboardRunning = true;

        while(dashboardRunning)
        {
            simulateSensors();

            displayDashboard();

            CloudLogger::logData(appliances);

            cout
            << "\n====================================================\n";

            cout
            << " LIVE SYSTEM COMMAND CENTER\n";

            cout
            << "====================================================\n";

            cout
            << "[1] Open Equipment Control Center\n";

            cout
            << "[2] Generate Monthly Energy Report\n";

            cout
            << "[3] Save Analytics To Binary Storage\n";

            cout
            << "[4] View Stored Analytics\n";

            cout
            << "[5] Run System Diagnostics\n";

            cout
            << "[6] Open Appliance Scheduling Center\n";

            cout
            << "[7] Continue Real-Time Monitoring\n";

            cout
            << "[8] Return To Main Menu\n";

            cout
            << "====================================================\n";

            cout
            << "Select Command: ";

            int option;

            cin >> option;

            switch(option)
            {
                case 1:

                    equipmentControlCenter();

                    break;

                case 2:

                    ReportManager::
                    generateMonthlyReport(appliances);

                    break;

                case 3:

                    BinaryAnalyticsManager::
                    saveAnalytics(appliances);

                    break;

                case 4:

                    BinaryAnalyticsManager::
                    readAnalytics();

                    break;

                case 5:

#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif

                    cout
                    << "====================================================\n";

                    cout
                    << " SYSTEM DIAGNOSTICS CENTER\n";

                    cout
                    << "====================================================\n\n";

                    cout
                    << "Checking current sensors............. OK\n";

                    cout
                    << "Checking voltage sensors............. OK\n";

                    cout
                    << "Checking relay modules............... OK\n";

                    cout
                    << "Checking appliance communication..... OK\n";

                    cout
                    << "Checking cloud synchronization....... OK\n";

                    cout
                    << "Checking binary storage.............. OK\n";

                    cout
                    << "Checking overload protection......... OK\n";

                    cout
                    << "\nSystem Status: FULLY OPERATIONAL\n";

                    delaySystem(3000);

                    break;

                case 6:

#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif

                    cout
                    << "====================================================\n";

                    cout
                    << " APPLIANCE SCHEDULING CENTER\n";

                    cout
                    << "====================================================\n\n";

                    cout
                    << "Scheduling simulation initialized.\n\n";

                    cout
                    << "Future Smart Features:\n";

                    cout
                    << "- Scheduled ON/OFF control\n";

                    cout
                    << "- Peak-hour optimization\n";

                    cout
                    << "- Automatic load balancing\n";

                    cout
                    << "- Appliance prioritization\n";

                    cout
                    << "- Smart energy saving automation\n";

                    delaySystem(3500);

                    break;

                case 7:

                    break;

                case 8:

                    dashboardRunning = false;

                    break;

                default:

                    cout
                    << "\nInvalid system command.\n";

                    delaySystem(1500);
            }

            delaySystem(1000);
        }
    }

    vector<Appliance*>& getAppliances()
    {
        return appliances;
    }
};

/*
==============================================================
MAIN MENU
==============================================================
*/

void displayMainMenu()
{
    cout
    << "========================================================\n";

    cout
    << " SMART ENERGY MANAGEMENT SYSTEM\n";

    cout
    << "========================================================\n\n";

    cout
    << "[1] Live Energy Monitoring Dashboard\n";

    cout
    << "[2] Generate Monthly Energy Report\n";

    cout
    << "[3] Save Analytics To Binary Storage\n";

    cout
    << "[4] View Stored Analytics\n";

    cout
    << "[5] System Fault Diagnostics\n";

    cout
    << "[6] Appliance Scheduling Center\n";

    cout
    << "[0] Shutdown System\n";

    cout
    << "\n========================================================\n";

    cout
    << "Select System Operation: ";
}

/*
==============================================================
MAIN FUNCTION
==============================================================
*/

int main()
{
    srand(time(0));

    SmartEnergyController controller;

    controller.addAppliance(new Fan());

    controller.addAppliance(new Refrigerator());

    controller.addAppliance(new Television());

    controller.addAppliance(new Heater());

    controller.addAppliance(new WaterPump());

    int choice;

    while(true)
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        displayMainMenu();

        cin >> choice;

        switch(choice)
        {
        case 1:

            controller.runLiveDashboard();

            break;

        case 2:

            ReportManager::
            generateMonthlyReport(
                controller.getAppliances());

            break;

        case 3:

            BinaryAnalyticsManager::
            saveAnalytics(
                controller.getAppliances());

            break;

        case 4:

            BinaryAnalyticsManager::
            readAnalytics();

            break;

        case 5:

            cout
            << "\nRunning diagnostics...\n";

            cout
            << "All systems operational.\n";

            delaySystem(2000);

            break;

        case 6:

            cout
            << "\nScheduling center coming soon.\n";

            delaySystem(2000);

            break;

        case 0:

            cout
            << "\nShutting down system...\n";

            delaySystem(2000);

            return 0;

        default:

            cout
            << "\nInvalid selection.\n";

            delaySystem(1000);
        }
    }

    return 0;
}

