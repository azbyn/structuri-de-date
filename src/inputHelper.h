#pragma once
#include "fixedPoint.h"
#include <iostream>

enum class Echo { No, Yes};
class MultiInputHelper {
public:
    std::string line;
    const char* p;
    std::istream& s;
    Echo echo = Echo::No;
public:
    MultiInputHelper(std::istream& s, Echo echo = Echo::Yes): s(s), echo(echo) {}
    MultiInputHelper(): s(std::cin)  {}
    bool getLine(std::string_view msg = "") {
        if (msg!="") {
            std::cout << msg;
            if (msg.back() != ' ') std::cout << ": ";
        }
        if (!std::getline(s, line)) return false;
        if (echo == Echo::Yes) std::cout << line << "\n";
        p = line.c_str();
        return true;
    }

    void getLineAfterInvalid(std::string_view msg) {
        std::cout << "!";
        getLine(msg);
    }
    void readName(std::string& res, std::string_view msg) {
        eatWhiteSpace(msg);
        if (*p == '"') {
            const char* beg = ++p;
            res = "";
            for (;;) {
                if (!*p) {
                    getLine/*AfterInvalid*/(msg);
                    return readName(res, msg);
                } else if (*p++ == '"') {
                    if (*(p-2) == '\\') {
                        res += std::string(beg, p-beg-2) + '"';
                        beg = p;
                    } else {
                        res += std::string(beg, p-beg-1);
                        return;
                    }
                }
            }
        } else {
            readString(res, msg);
        }
    }

    void readString(std::string& res, std::string_view msg) {
        eatWhiteSpace(msg);
        const char* beg = p;
        while (*p && !isspace(*p)) {
            ++p;
        }
        auto sz = beg-p;
        if (sz == 0) {
            getLine/*AfterInvalid*/(msg);
            return readString(res, msg);
        } else {
            res = std::string(beg, p - beg);
        }
    }
    char readChar(std::string_view msg = "") {
        eatWhiteSpace(msg);
        return *p++;
    }

    std::string_view readStringView(std::string_view msg) {
        eatWhiteSpace(msg);
        const char* beg = p;
        while (*p && !isspace(*p)) ++p;

        auto sz = beg-p;
        if (sz == 0) {
            getLineAfterInvalid(msg);
            return readStringView(msg);
        } else {
            return std::string_view(beg, p - beg);
        }
    }
    template<size_t precision>
    void readFP(FixedPoint<precision>& res, std::string_view msg) {
        eatWhiteSpace(msg);
        auto str = FixedPoint<precision>::fromString(p, res);
        if (str == nullptr) {
            getLineAfterInvalid(msg);
            readFP(res, msg);
            return;
        } else {
            p = str;
        }
    }

private:
    void eatWhiteSpace(std::string_view message) {
        for (;;) {
            if (*p == 0) getLine(message);
            if (isspace(*p)) ++p;
            else return;
        }
    }
};
