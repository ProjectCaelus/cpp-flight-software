#ifndef FLIGHT_ARDUINO_HPP
#define FLIGHT_ARDUINO_HPP

#include <string>
#include <flight/modules/drivers/PseudoArduino.hpp>

using namespace std;

class Arduino {
private:
    string name;
    PseudoArduino* arduino;
public:
    explicit Arduino(const string& name) : name(name) {
        reset();
    }

    void reset();
    char* read();
    void write(char* msg);
};

#endif //FLIGHT_ARDUINO_HPP
