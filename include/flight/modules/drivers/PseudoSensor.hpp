#ifndef FLIGHT_PSEUDOSENSOR_HPP
#define FLIGHT_PSEUDOSENSOR_HPP

#define NUM_SENSORS 6

#include <vector>
#include <tuple>
#include <string>
#include <map>
#include <flight/modules/drivers/PseudoArduino.hpp>

class PseudoSensor : public PseudoArduino {
private:
    vector<tuple<string, string>> sensor_list;
    map<tuple<string, string>, double> sensor_values;

    void set_sensor_values();

public:
    PseudoSensor();
    char* read() override;
    void write(char* msg) override;
};

#endif //FLIGHT_PSEUDOSENSOR_HPP
