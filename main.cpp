#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <bitset>

#include "assembler.h"

int main(){

    std::string filename;
    std::ifstream infile;
    
    std::cout << "Please enter testcase filename: ";
    std::cin >> filename;

    infile.open(filename.c_str());

    std::string instruction, operand1, operand2, operand3, instruction4, operand4, operand5, operand6;
    std::string instruction2, operand12, operand13, operand14;
    std::string instruction3, operand21, operand22, operand23;

    // CMP instructions on testcase-1 test
    infile >> instruction >> operand1 >> operand2 >> operand3;

    uint32_t machinecode = assembler::assembleDP(instruction, operand1, operand2, operand3);
    
    std::cout << "MOV operation: \n";
    std::cout << std::hex << std::uppercase << machinecode << "\n";
    
    std::bitset<32> binaryRepresentation(machinecode);
    std::cout << "\n" << binaryRepresentation << "\n";

    // LDR instructions on testcase-1
    infile >> instruction4 >> operand4 >> operand5 >> operand6;

    machinecode = assembler::assembleMem(instruction4, operand4, operand5, operand6);

    std::cout << "\n\nLDR operation\n";
    std::cout << std::hex << std::uppercase << machinecode << "\n";
    
    std::bitset<32> binaryRepresentation2(machinecode);
    std::cout << "\n" << binaryRepresentation2 << "\n";

/*
    infile >> instruction2 >> operand12 >> operand13 >> operand14;

    machinecode = assembler::assembleMem(instruction2, operand12, operand13, operand14);

    std::cout << "\n\nSUB operation\n";
    std::cout << std::hex << std::uppercase << machinecode << "\n";

    std::bitset<32> binaryRepresentation3(machinecode);
    std::cout << "\n" << binaryRepresentation3 << "\n";

    infile >> instruction3 >> operand21 >> operand22 >> operand23;

    machinecode = assembler::assembleDP(instruction3, operand21, operand22, operand23);

    std::cout << "\n\nLSL operation\n";
    std::cout << std::hex << std::uppercase << machinecode << "\n";

    std::bitset<32> binaryRepresentation4(machinecode);
    std::cout << "\n" << binaryRepresentation4 << "\n";

    infile.close();

    return 0; */
}