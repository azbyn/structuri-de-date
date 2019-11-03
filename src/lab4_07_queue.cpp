#include "utils.h"
#include "queue.h"

int main() {
    auto q1 = Queue<std::string>::read("q1");
    auto q2 = Queue<std::string>::read("q2");

    while (!q1.empty() || !q2.empty()) {
        if (!q1.empty())
            std::cout << "'"<< q1.pop() <<"',";
        if (!q2.empty())
            std::cout << "'" << q2.pop() << "',";
    }

    std::cout << "\n";
    return 0;
}
