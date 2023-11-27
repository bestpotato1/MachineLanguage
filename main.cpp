#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <bitset>
#include <sstream>

#include "assembler.h"

int main(){

    uint32_t machinecode = 0;

    std::string filename;
    std::ifstream infile;

    // Input file
    std::cout << "Please enter testcase filename: ";
    std::cin >> filename;

    infile.open(filename.c_str());

    // Error checking input file
    if(!infile.is_open()){
        std::cout << "Error opening input file. Please run program again.\n";
        return 1;
    }

    // Output file
    std::ofstream outfile("output.txt");

    // Error checking output file
    if(!outfile.is_open()){
        std::cout << "Error opening output file. Please run program again.\n";
        return 1;
    }

    std::string input_line; // Input line for getline

    while(std::getline(infile, input_line)){

        // istringstream captures the entire line and allows you to extract the information into variables through iss
        std::istringstream iss(input_line);

        std::string instruction;

        iss >> instruction;

        if(instruction == "MOV" || instruction == "CMP"){
            std::string operand1, operand2;
            iss >> operand1 >> operand2;
            std::cout << instruction << " " << operand1 << " " << operand2 << "\n";

            machinecode = assembler::assembleDP(instruction, operand1, operand2, "");
            outfile << "0x" << std::hex << std::uppercase << machinecode << "\n";

        } else if(instruction == "LSL" || instruction == "ASR" || instruction == "LSR" ||
            instruction == "ROR" || instruction == "ADD" || instruction == "SUB" || 
            instruction == "AND" || instruction == "OR"){
            
            std::string operand1, operand2, operand3;
            iss >> operand1 >> operand2 >> operand3;
            std::cout << instruction << " " << operand1 << " " << operand2 << " " << operand3 << "\n";

            machinecode = assembler::assembleDP(instruction, operand1, operand2, operand3);
            outfile << "0x" << std::hex << std::uppercase << machinecode << "\n";

        } else if(instruction == "STR" || instruction == "LDR"){

            std::string operand1, operand2, operand3;
            iss >> operand1 >> operand2 >> operand3;
            std::cout << instruction << " " << operand1 << " " << operand2 << " " << operand3 << "\n";

            machinecode = assembler::assembleMem(instruction, operand1, operand2, operand3);
            outfile << "0x" << std::hex << std::uppercase << machinecode << "\n";

        } else if (instruction == "B" || instruction == "BL"){

            std::cout << instruction << "\n";

            machinecode = assembler::assembleBranch(instruction);
            outfile << "0x" << std::hex << std::uppercase << machinecode << "\n";

        } else {
            
            std::cout << instruction << " is either a function label or an instruction that is not supported by this assembler.\n";

        }

    }

    // Closing files
    infile.close();
    outfile.close();

    return 0;
}