#include "lab5_17_stoc.h"

#include "input_helper.h"
#include "utils.h"
#include "list.h"

#include <iostream>
#include <iomanip>

struct Command {
    char shortName;
    std::string_view name;
    std::string_view args;
    bool (*f)(Stock& s, MultiInputHelper& ih);//returns true if should exit
    std::string_view description;
    void print() const {
        std::cout << "  " << std::setw(10) << std::left << name
                  << " - " << shortName << " "
                  << std::setw(25) << args << " - " << description << "\n";
    }
};
using IH = MultiInputHelper;

void printHelp();
bool printHelp(Stock&, IH&) {
    printHelp();
    return true;
}
bool quit(Stock&, IH&) { return false; }
bool print(Stock& s, IH&) {
    s.print();
    return true;
}

bool add(Stock& s, IH& ih) {
    Product p;
    ih.readName(p.name, "name");
    ih.readFP(p.quantity, "quantity");
    ih.readString(p.unit, "unit");
    ih.readFP(p.unitPrice, "unit price");
    s.add(p);
    return true;
}

bool sell(Stock& s, IH& ih) {
    std::string name;
    FP quantity;
    ih.readName(name, "name");
    ih.readFP(quantity, "quantity");

    s.sell(name, quantity);
    return true;
}

bool resupply(Stock& s, IH& ih) {
    std::string name;
    FP quantity;
    ih.readName(name, "name");
    ih.readFP(quantity, "quantity");
    
    s.sell(name, quantity);
    return true;
}

bool value(Stock& s, IH&) {
    std::cout << "Total stock value: " << s.value() << "\n";
    return true;
}
bool init(Stock& s, IH& ih) {
    std::string str;
    //std::getline(std::cin, str);
    char c;
    for (;;) {
        ih.getLine("+ ");
        //std::cout << "+ ";
        add(s, ih);
        ih.getLine("Add more products (y/N)? ");
        //std::cout << "Add more products (y/N)? ";
        //std::cin >> c;
        if (tolower(ih.readChar()) != 'y') break;
    }
    return true;
}
constexpr Command cmds[] = {
    { 'H', "help", "", printHelp, "Show help" },
    { 'A', "add", "name quantity unit price", add, "Add product" },
    { 'I', "init", "", init, "Add multiple elements" },
    { 'S', "sell", "name quantity", printHelp, "Sell product" },
    { 'R', "resupply", "name quantity", printHelp, "Resupply product" },
    { 'V', "value", "", printHelp, "Print stock total value" },
    { 'P', "print", "", print, "Print a table of products" },
    { 'Q', "quit", "", quit, "Quit the program" },
};
void printHelp() {
    std::cout << "\nFormat of commands: \n"
                 "name - shortName args - description\n";

    for (size_t i = 0; i < (sizeof(cmds) / sizeof(cmds[0])); ++i)
        cmds[i].print();
}
bool eval(Stock& stock, MultiInputHelper& ih) {
    auto s = ih.readStringView("> ");
    if (s.size() == 1) {
        for (size_t i = 0; i < (sizeof(cmds) / sizeof(cmds[0])); ++i) {
            if (cmds[i].shortName == toupper(s[0])) return cmds[i].f(stock, ih);
        }
    }
    for (size_t i = 0; i < (sizeof(cmds) /sizeof(cmds[0])); ++i) {
        if (cmds[i].name == s) return cmds[i].f(stock, ih);
    }
    std::cout << "Invalid command!\n";
    printHelp();

    return true;
}

int main() {
    FP::test();
    Stock stock;
    printHelp();
    MultiInputHelper ih("> ");
    for (;;) {
        //std::cout << "> ";
        //std::cin >> s;
        if (!eval(stock, ih)) break;
    }
    return 0;
}
