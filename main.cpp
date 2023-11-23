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

    std::string instruction, operand1, operand2, operand3, operand4, operand5, operand6;
    std::string instruction2, operand12, operand13, operand14;
    std::string instruction3, operand21, operand22, operand23;

    /* while(infile >> instruction >> operand1 >> operand2 >> operand3){

        std::cout << instruction << " " << operand1 << " " << operand2 << " " << operand3;

        std::cout << "\n";
        
        // uint32_t machineCode = assembler::assemble(instruction, operand1, operand2, operand3);

    } */


    // MOV instructions on testcase-1
    infile >> instruction >> operand1 >> operand2 >> operand3;

    uint32_t machineCode = assembler::assembleDP(instruction, operand1, operand2, operand3);
    
    std::cout << "MOV operation: \n";
    std::cout << std::hex << std::uppercase << machineCode << "\n";
    
    std::bitset<32> binaryRepresentation(machineCode);
    std::cout << "\n" << binaryRepresentation << "\n";

    // LSL instructions on testcase-1
    infile >> operand4 >> operand5 >> operand6;

    machineCode = assembler::assembleDP(operand3, operand4, operand5, operand6);

    std::cout << "\n\nLSL operation\n";
    std::cout << std::hex << std::uppercase << machineCode << "\n";
    
    std::bitset<32> binaryRepresentation2(machineCode);
    std::cout << "\n" << binaryRepresentation2 << "\n";
/*
    infile >> instruction2 >> operand12 >> operand13 >> operand14;

    machineCode = assembler::assemble(instruction2, operand12, operand13, operand14);

    std::cout << "\n\nSTR operation\n";
    std::cout << std::hex << std::uppercase << machineCode << "\n";

    std::bitset<32> binaryRepresentation3(machineCode);
    std::cout << "\n" << binaryRepresentation3 << "\n";

    infile >> instruction3 >> operand21 >> operand22 >> operand23;

    machineCode = assembler::assemble(instruction3, operand21, operand22, operand23);

    std::cout << "\n\nASR operation\n";
    std::cout << std::hex << std::uppercase << machineCode << "\n";

    std::bitset<32> binaryRepresentation4(machineCode);
    std::cout << "\n" << binaryRepresentation4 << "\n";

    infile.close();

    return 0; */
}