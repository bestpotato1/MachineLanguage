//assembler.h
#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <cstdint>
#include <string>
#include <map>
#include <bitset>
#include <iostream>

class assembler{

    public:
        static uint32_t assembleDP(const std::string& instruction, const std::string& operand1, const std::string& operand2, const std::string& operand3);
        static uint32_t assembleMem(const std::string& instruction, const std::string& operand1, const std::string& operand2, const std::string& operand3);
        static uint32_t assembleBranch(const std::string& instruction);
};

#endif