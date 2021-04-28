#include <set>
#include <ArduinoJson.h>
#include <flight/modules/lib/logger_util.hpp>
#include <flight/modules/mcl/Supervisor.hpp>
#include <flight/modules/tasks/SensorTask.hpp>
#include <flight/modules/tasks/TelemetryTask.hpp>
#include <flight/modules/tasks/ValveTask.hpp>
#include <flight/modules/lib/Util.hpp>
#include <flight/modules/mcl/Config.hpp>
#include <flight/modules/lib/Constants.hpp>

#ifndef DESKTOP
    #include "Arduino.h"
#endif


Supervisor::~Supervisor() {
    delete control_task;

    for (auto task : tasks) {
        delete task;
    }
}

void Supervisor::initialize() {
    /* Load config */

    Util::doc.clear();
    deserializeJson(Util::doc, CONFIG_STR);
    JsonObject j = Util::doc.as<JsonObject>();

    global_config = Config(j);
    global_registry.initialize();

    print("Supervisor: Parsing config");
    parse_config();

    print("Tasks: Initializing");
    for (Task* task : tasks){
        task->initialize();
    }

    print("Control tasks: Initializing");
    control_task->begin();
}

void Supervisor::read() {
    print("Supervisor: Reading");
    for (Task* task : tasks){
        task->read();
    }
}

void Supervisor::control() {
    print("Supervisor: Controlling");
    control_task->control();
}

void Supervisor::actuate() {
    print("Supervisor: Actuating");
    for (Task* task : tasks){
        task->actuate();
    }
}

void Supervisor::run() {
    while (true) {
        // double delay = global_config.timer.delay;
        // Serial.println(delay);
        // Serial.println("HELLOO");
        // Serial.println(millis());
        // double delay = 0;
        // long double start_time = Util::getTime();
        read();
        control();
        actuate();
        Util::pause(1000);
    }
}

void Supervisor::parse_config() {
    // parse_json_list automatically parses config.json
    for (const string& task : global_config.task_config.tasks) {
        print("Found task: " + task);
        if (task == "sensor") tasks.push_back(new SensorTask());
        if (task == "telemetry") tasks.push_back(new TelemetryTask());
        if (task == "valve") tasks.push_back(new ValveTask());
    }

    set<string> control_tasks;
    for (const string& control_task : global_config.task_config.control_tasks) {
        control_tasks.insert(control_task);
        print("Control task [" + control_task + "]: Enabled");
    }

    control_task = new ControlTask(control_tasks);
}