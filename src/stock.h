#pragma once
#include "fixedPoint.h"
#include "list.h"

#include <iostream>
#include <iomanip>

constexpr size_t precision = 2;
using FP = FixedPoint<precision>;

template<class T>
constexpr void setMax(T& a, T b) {
    if (b > a) a = b;
}
struct Product {
    std::string name;
    FP quantity;
    std::string unit;
    FP unitPrice;
    constexpr auto totalPrice() const {
        return quantity * unitPrice;
    }
};

struct Stock {
    List<Product> products;

    //retun value: was the operation was successful
    bool add(Product p) {
        auto* res = products.find(
            [&] (Product& other) { return other.name == p.name; });
        if (res != nullptr) {
            auto& v = res->val;
            std::cout << "Product '"<< p.name << "' already exists.\n"
                "Resuplying.\n";
            if (v.unit != p.unit) {
                std::cout << "Units did not match ('"
                          << v.unit <<"' != '" <<p.unit << "')\n";
                return false;
            }
            v.unitPrice += (v.quantity*v.unitPrice + p.quantity*p.unitPrice)
                / (v.quantity +p.quantity);
            v.quantity += p.quantity;
        } else {
            products.push_front(p);
        }
        return true;
    }
    //retun value: was the operation was successful
    bool resupply(const std::string& name, FP quantity) {
        auto* res = products.find(
            [&] (Product& other) { return other.name == name; });
        if (res == nullptr){
            std::cout << "Product '" << name << "' not found.\n";
            return false;
        }
        res->val.quantity += quantity;
        return true;
    }
    //retun value: was the operation was successful
    bool sell(const std::string& name, FP quantity) {
        auto* res = products.find(
            [&] (Product& other) { return other.name == name; });
        if (res == nullptr) {
            std::cout << "Product '" << name << "' not found.\n";
            return false;
        }
        auto& v = res->val;
        if (quantity > v.quantity) {
            std::cout << "Quantity too high. Can't sell. (" << quantity
                      << " > " << v.quantity << ")\n";
            return false;
        }
        v.quantity -= quantity;
        return true;
    }
    constexpr FP value() const {
        FP total = 0;
        for (auto& p : products)
            total += p.totalPrice();
        return total;
    }
    void print() const {
        if (products.empty()) {
            std::cout << "No products\n";
            return;
        }
        auto ph = PrintHelper(*this);
        ph.printHeader();
        FP total = 0;
        for (auto& p : products) {
            ph.printProd(p);
            total += p.totalPrice();
        }
        ph.printFooter(total);
    }
private:

    class PrintHelper {
        static constexpr std::string_view fields[] = {
            "Name", "Quantity", "Unit Price", "Total Price"
        };
        struct Longest {
            size_t name = fields[0].size();
            size_t quantity = 0;// fields[1].size();
            size_t unit = 0;
            size_t unitPrice = fields[2].size();
            size_t totalPrice = fields[3].size();
        } longest;
    public:
        constexpr PrintHelper(const Stock& s) {
            auto& l = longest;
            for (auto& p : s.products) {
                setMax(l.name, p.name.size());
                setMax(l.quantity, p.quantity.textLen());
                setMax(l.unit, p.unit.size());
                setMax(l.unitPrice, p.unitPrice.textLen());
                setMax(l.totalPrice, p.totalPrice().textLen());
            }

            longest.quantity = std::max(fields[1].size() - longest.unit -1,
                                        longest.quantity);
        }
        void printProd(const Product& p) const {
            std::cout << "| " << std::left;
            printPadded(longest.name, p.name);
            std::cout << " | " << std::right;
            printPadded(longest.quantity, p.quantity);
            std::cout << " " << std::left;
            printPadded(longest.unit, p.unit);
            std::cout << " | " << std::right;
            printPadded(longest.unitPrice, p.unitPrice);
            std::cout << " | ";
            printPadded(longest.totalPrice, p.totalPrice());
            std::cout << " |\n";
        }
        void printHeader() const {
            printLine();
            std::cout << "| ";
            printCentered(longest.name, fields[0]);
            std::cout << " | ";
            printCentered(longest.quantity + 1 + longest.unit, fields[1]);
            std::cout << " | ";
            printCentered(longest.unitPrice, fields[2]);
            std::cout << " | ";
            printCentered(longest.totalPrice, fields[3]);
            std::cout << " |\n";
            printLine();
        }
        void printFooter(FixedPoint<precision> totalPrice) const {
            printLine();
            std::cout << "| ";
            printPadded(longest.name, "");
            std::cout << " | ";
            printPadded(longest.quantity + 1 + longest.unit, "");
            std::cout << " | ";
            printPadded(longest.unitPrice, "");
            std::cout << " | "<<std::right;
            printPadded(longest.totalPrice, totalPrice);
            std::cout << " |\n";
            printLine();
        }

    private:
        template<typename T>
        static void printPadded(size_t len, T v) {
            std::cout << std::setw(len) << v;
        }
        static void printCentered(size_t len, std::string_view s) {
            //we can safely assume that len >= s.len();
            int total = len - s.size();
            int left = total / 2;
            int right = total - left;
            hline(left, ' ');
            std::cout << std::setw(0) << s;
            hline(right, ' ');
        }

        static void hline(int len, char c) {
            while (--len >= 0)
                std::cout << c;
        }

        void printLine() const {
            std::cout << "+";
            hline(longest.name+2, '-');
            std::cout << "+";
            hline(longest.quantity + 3 + longest.unit, '-');
            std::cout << "+";
            hline(longest.unitPrice+2, '-');
            std::cout << "+";
            hline(longest.totalPrice+2, '-');
            std::cout << "+\n";
        }
    };
};
