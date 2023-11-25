#include "assembler.h"

std::map<std::string, uint8_t> registers = {
    {"R0,", 0b0000}, {"R1,", 0b0001}, {"R2,", 0b0010}, {"R3,", 0b0011},
    {"R4,", 0b0100}, {"R5,", 0b0101}, {"R6,", 0b0110}, {"R7,", 0b0111},
    {"R8,", 0b1000}, {"R9,", 0b1001}, {"R10,", 0b1010}, {"R11,", 0b1011},
    {"R12,", 0b1100}, {"R13,", 0b1101}, {"R14,", 0b1110}, {"R15,", 0b1111},
    {"R0", 0b0000}, {"R1", 0b0001}, {"R2", 0b0010}, {"R3", 0b0011},
    {"R4", 0b0100}, {"R5", 0b0101}, {"R6", 0b0110}, {"R7", 0b0111},
    {"R8", 0b1000}, {"R9", 0b1001}, {"R10", 0b1010}, {"R11", 0b1011},
    {"R12", 0b1100}, {"R13", 0b1101}, {"R14", 0b1110}, {"R15", 0b1111},
    {"R0],", 0b0000}, {"R1],", 0b0001}, {"R2],", 0b0010}, {"R3],", 0b0011},
    {"R4],", 0b0100}, {"R5],", 0b0101}, {"R6],", 0b0110}, {"R7],", 0b0111},
    {"R8],", 0b1000}, {"R9],", 0b1001}, {"R10],", 0b1010}, {"R11],", 0b1011},
    {"R12],", 0b1100}, {"R13],", 0b1101}, {"R14],", 0b1110}, {"R15],", 0b1111},
    // 16 registers will be provided (because Rd 15:12)
};

std::map<std::string, uint8_t> cmds = {
    {"AND", 0b0000}, {"ORR", 0b1100}, {"ADD", 0b0100}, {"SUB", 0b0010}
};

uint32_t assembler::assembleDP(const std::string& instruction, const std::string& operand1, const std::string& operand2, const std::string& operand3){

    uint32_t machinecode = 0;

    // AL condition bits
    machinecode |= (0b1110 << 28);

    // Data processing OP
    machinecode |= (0b00 << 26);

    // Condition flags
    machinecode |= (0 << 20);

    if(instruction == "MOV" || instruction == "LSL" || instruction == "LSR" 
    || instruction == "ASR" || instruction == "ROR"){
        machinecode |= (0b1101 << 21); // cmd bits
        machinecode |= (0b0000 << 16); // Register source bits are 0 for cmd 1101
    
        // Instruction specific bits, i.e. immediate bit and cmd bits since MOV only has three parameters
        if(instruction == "MOV"){
            if(operand2.substr(0,0) == "#"){
                machinecode |= (1 << 25);
            } else{ // I = 1 OR instr_11:4 = 0
                machinecode |= (0 << 25);
                machinecode |= (0b00000000 << 4);
                auto reg = registers.find(operand2);
                machinecode |= (reg->second);
            }
            auto reg = registers.find(operand1);
            machinecode |= (reg->second << 12);
            // Do not know how to calculate Src2 for MOV so be default they will be zero
            return machinecode;
        } else{
            machinecode |= (0b0000 << 16); // No register source for these
            auto reg = registers.find(operand1);
            machinecode |= (reg->second << 12); // Register destination

            // If I=0, here are register operations
            if(operand3.substr(0,1) == "#"){ 
                
                reg = registers.find(operand2);
                machinecode |= (reg->second);

                machinecode |= (std::stoi(operand3.substr(1)) << 7);
                
                // Sh bits for register immediates
                if(instruction == "LSL"){
                    machinecode |= (0b000 << 4);
                    } else if(instruction == "ASR"){
                    machinecode |= (0b100 << 4);
                    } else if(instruction == "LSR"){
                    machinecode |= (0b010 << 4);
                    } else if(instruction == "ROR"){
                    machinecode |= (0b110 << 4);
                    }
                } 
                // If I=0, here are register-shifted register operations
                else{

                    // Register source (Rs) register
                    auto reg = registers.find(operand3);
                    machinecode |= (reg->second << 8);

                    reg = registers.find(operand2);
                    machinecode |= (reg->second);

                    if(instruction == "LSL"){
                        machinecode |= (0b0001 << 4);
                    } else if(instruction == "ASR"){
                        machinecode |= (0b0101 << 4);
                    } else if(instruction == "LSR"){
                        machinecode |= (0b0011 << 4);
                    } else if(instruction == "ROR"){
                        machinecode |= (0b0111 << 4);
                    }
                }
                return machinecode;
            }
        }

    // Cmd bits for other instructions
    auto cmd = cmds.find(instruction);
    machinecode |= (cmd->second << 21);    

    // Register source
    auto reg = registers.find(operand2);
    machinecode |= (reg->second << 16);

    // Register destination
    reg = registers.find(operand1);
    machinecode |= (reg->second << 12);

    // Immediate bit conditions
    if(operand3.substr(0,1) == "#"){
        machinecode |= (1 << 25); // I = 1
        machinecode |= (std::stoi(operand3.substr(1)));
    } else{ // Register
        machinecode |= (0 << 25); // I = 0
        
        machinecode |= (0b00000000 << 4); // 8 bits covering shamt5, sh, and fixed 0 bit for standard data processing instructions

        reg = registers.find(operand3);
        machinecode |= (reg->second); // Rm bits
    }

    return machinecode;
}

uint32_t assembler::assembleMem(const std::string& instruction, const std::string& operand1, const std::string& operand2, const std::string& operand3){

    uint32_t machinecode = 0;

    // AL condition bits
    machinecode |= (0b1110 << 28);

    // OP bits
    machinecode |= (0b01 << 26);

    // Immediate bit conditions
    if(!operand3.empty() && operand3.substr(0,1) == "#"){
        machinecode |= (0 << 25); // I
        machinecode |= (std::stoi(operand3.substr(1))); // Immediate bits
    } else{
        machinecode |= (1 << 25); // I
        machinecode |= (0b00000000 << 4); // Shift amount (in case of LSL to operand), sh value (00), and 0 bit.
        auto reg = registers.find(operand3.substr(0,operand3.size()-1)); // Rm register
        machinecode |= (reg->second);
    }

    // Pre or post indexing flags
    if(operand3.substr(operand3.size()-1,1) == "]" || operand3.substr(operand3.size()-1,1) == "!"
     || (operand2.substr(operand2.size()-1,1) == "]" && !operand3.empty())){
        machinecode |= (1 << 24);
    } else machinecode |= (0 << 23);

    // Increment or decrement bit
    if(!operand3.empty() && operand3.substr(1,1) == "-"){
        machinecode |= (0 << 23); // decrement in case of negative value
    } else machinecode |= (1 << 23); // otherwise increment

    // Offset byte instruction is out of the scope of this program, therefore it's 0 bit
    machinecode |= (0 << 22);

    // Write back flag bit
    if(!operand3.empty() && operand3.substr(operand3.size()-1) == "!"){
        machinecode |= (1 << 21);
    } else machinecode |= (0 << 21);

    if(instruction == "LDR"){
        machinecode |= (1 << 20);
    } else machinecode |= (0 << 20);

    // Register source
    auto reg = registers.find(operand2.substr(1));
    machinecode |= (reg->second << 16);

    // Destination register
    reg = registers.find(operand1);
    machinecode |= (reg->second << 12);

    return machinecode;

}

uint32_t assembler::assembleBranch(const std::string& instruction){

    uint32_t machinecode = 0;

    return machinecode;

}