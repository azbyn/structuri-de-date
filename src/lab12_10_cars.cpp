#include "heap.h"

struct Vehicle {
    double speed;
    std::string id;
    Vehicle() {}//the alternative would have been more complicated
    Vehicle(double speed, const std::string& id) : speed(speed), id(id) {}
    constexpr bool operator<(const Vehicle& rhs) const {
        return speed < rhs.speed;
    }

    constexpr bool operator>(const Vehicle& rhs) const {
        return speed > rhs.speed;
    }
    friend std::ostream& operator <<(std::ostream& s, const Vehicle& v) {
        return s << v.id << " - speed" << v.speed << " km/h\n";
    }
};
int main() {
    MaxHeap<Vehicle> cars;
    cars.push({1.2, "A"});
    std::cout << "oi" << cars.data.capacity() << "\n";
    cars.push({1.5, "B"});
    std::cout << "oi" << cars.data.capacity() << "\n";
    cars.push({1.4, "C"});
    std::cout << "oi" << cars.data.capacity() << "\n";
    cars.printSorted();
    return 0;
}
