#include "binaryTree.h"
#include "stock_bst.h"

#include "inputHelper.h"
#include "utils.h"
#include "list.h"

#include <iostream>
#include <iomanip>
struct Command {
    char shortName;
    std::string_view name;
    std::string_view args;
    bool (*f)(Stock& s, MultiInputHelper& ih);//returns false if should exit
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
    std::string name;
    ih.readName(name, "name");
    ih.readFP(p.quantity, "quantity");
    ih.readString(p.unit, "unit");
    ih.readFP(p.unitPrice, "unit price");
    s.add(name, p);
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
    
    s.resupply(name, quantity);
    return true;
}

bool value(Stock& s, IH&) {
    std::cout << "Total stock value: " << s.value() << "\n";
    return true;
}
bool init(Stock& s, IH& ih) {
    std::string str;
    for (;;) {
        ih.getLine("+ ");
        add(s, ih);
        ih.getLine("Add more products (y/N)? ");
        if (tolower(ih.readChar()) != 'y') break;
    }
    return true;
}
std::ostream& operator<<(std::ostream& s,Product) { return s << "{Prod}"; }
bool debug(Stock& s, IH& ) {
    std::cout << "Products:\n" << s.products << "\n";
    return true;
}
constexpr Command cmds[] = {
    { 'H', "help", "", printHelp, "Show help" },
    { 'A', "add", "name quantity unit price", add, "Add product" },
    { 'I', "init", "", init, "Add multiple elements" },
    { 'S', "sell", "name quantity", sell, "Sell product" },
    { 'R', "resupply", "name quantity", resupply, "Resupply product" },
    { 'V', "value", "", value, "Print stock total value" },
    { 'P', "print", "", print, "Print a table of products" },
    { 'Q', "quit", "", quit, "Quit the program" },
    { 'D', "debug", "", debug, "Debug" },
};
void printHelp() {
    std::cout << "\nFormat of commands: \n"
                 "name - shortName args - description\n";

    for (size_t i = 0; i < (sizeof(cmds) / sizeof(cmds[0])); ++i)
        cmds[i].print();
}

bool eval(Stock& stock, MultiInputHelper& ih) {
    if (!ih.getLine("> ")) return false;
    auto s = ih.readStringView("> ");
    if (s.size() == 1) {
        for (size_t i = 0; i < (sizeof(cmds) / sizeof(cmds[0])); ++i) {
            if (cmds[i].shortName == toupper(s[0]))
                return cmds[i].f(stock, ih);
        }
    }
    for (size_t i = 0; i < (sizeof(cmds) /sizeof(cmds[0])); ++i) {
        if (cmds[i].name == s) return cmds[i].f(stock, ih);
    }
    std::cout << "Invalid command!\n";
    printHelp();
    std::cout << "!";
    return true;
}

int main(int argc, char* argv[]) {
    Stock stock;
    if (argc >= 2) {
        auto path = argv[1];
        std::cout << "Reading '" << path << "'\n";
        std::ifstream f(path);
        MultiInputHelper fih(f);
        for (;;) {
            if (!eval(stock, fih)) break;
        }
    }
    printHelp();
    MultiInputHelper ih;
    for (;;) {
        if (!eval(stock, ih)) break;
    }
    return 0;
}
