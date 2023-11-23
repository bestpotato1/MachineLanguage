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
    // 16 registers will be provided (because Rd 15:12)
};

std::map<std::string, uint8_t> cmds = {
    {"AND", 0b0000}, {"ORR", 0b1100}, {"ADD", 0b0100}, {"SUB", 0b0010}
};

uint32_t assembler::assemble(const std::string& instruction, const std::string& operand1, const std::string& operand2, const std::string& operand3){

    uint32_t machinecode = 0;

    // Establish conditions that will set the values for each of the parameters
    if(instruction == "MOV"){ // Mov instruction part of data processing instruction

        //AL condition bits - 1110 (0xE)
        machinecode |= (0b1110 << 28);

        //OP codes for data processing instruction
        machinecode |= (0b00 << 26);

        machinecode |= (1 << 25); // Immediate flag bit
        
        machinecode |= (0b1101 << 21); // Cmd bits for MOV according to textbook Appendix B p. 536
        
        machinecode |= (0 << 20); // S (ADDS for example - set conditions flag)

        machinecode |= (0b0000 << 16); // Source register bits

        auto reg = registers.find(operand1);

        if(reg != registers.end()){
            machinecode |= (reg->second << 12); // Destination register
        }

        int immediateValue = std::stoi(operand2.substr(1)); 

        // Determining rotation value
        int rotation = 0;
        for (int i = 0; i < 16; ++i) {
        if ((immediateValue & 0xFF) == (immediateValue >> 8)) {
            break;
        }
        immediateValue = (immediateValue << 1) | (immediateValue >> 31);
        rotation += 2;
        }

        machinecode |= ((rotation/2) & 0xF << 8); // Rotation bits

        machinecode |= (immediateValue & 0xFF << 0); // Immediate value limited to 8 bits

    }

    if(instruction == "LSL"){ // Logical left shift part of data processing instructiosn

        // AL condition bits
        machinecode |= (0b1110 << 28);

        // OP codes for data processing instruction
        machinecode |= (0b00 << 26);

        // Immediate flag
        machinecode |= (0 << 25);

        // Cmd bits for Logical left shift according to textbook p. 536
        machinecode |= (0b1101 << 21);

        // S flag - condition flags
        machinecode |= (0 << 20);

        // Source register for LSL operation is 0
        machinecode |= (0b0000 << 16);

        //Destination register bits
        auto reg = registers.find(operand1);

        if(reg != registers.end()){
            machinecode |= (reg->second << 12);
        }

        // Shift amount (shamt5)
        machinecode |= (std::stoi(operand3.substr(1)) << 7);

        // sh = 00 for lsl and additional 0 bit
        machinecode |= (0b000 << 4);

        // Rm register
        reg = registers.find(operand2);

        if(reg != registers.end()){
            machinecode |= (reg->second);
        }

    }

    if(instruction == "STR"){

        // AL condition bits
        machinecode |= (0b1110 << 28);

        // OP code bits
        machinecode |= (0b01 << 26);

        // Register immediate bits I = 1 for register and I = 0 for immediate value according to p 538
        // Pre or post index bit
        // Increment decrement bit
        auto reg = registers.find(operand3.substr(0, 2));
        if(reg != registers.end()){
            machinecode |= (1 << 25);
            machinecode |= (1 << 24); // Always a pre or post index instruction if it incorporates another register
            machinecode |= (1 << 23); // Always a increment or decrement bit if it includes a register
            machinecode |= (0 << 22);
        } else if(operand3.substr((0,0)) == "#"){ // In case of constant value
            machinecode |= (0 << 25);
            machinecode |= (0 << 24);
            machinecode |= (1 << 23); // Incrementing the register by a certain value
            machinecode |= (1 << 22);
        }
        else{
            machinecode |= (0 << 25);
            machinecode |= (0 << 24);
            machinecode |= (0 << 23);
            machinecode |= (0 << 22);
        }

        // Write back flag bit
        if(operand3.substr(operand3.size()-1) == "!"){
            machinecode |= (1 << 21);
        } else machinecode |= (0 << 21);

        // L flag designation LDR/STR
        machinecode |= (0 << 20);

        // Base register value
        reg = registers.find(operand2.substr(1,operand2.size()-2));
        if(reg != registers.end()){
            machinecode |= (reg->second << 16);
        } else machinecode |= (0b0000 << 16);

        // Destination register value
        reg = registers.find(operand1);
        if(reg != registers.end()){
            machinecode |= (reg->second << 12);
        } else machinecode |= (0b0000 << 12);

        if(operand3.substr((0,0)) != "#"){
            machinecode |= (0b0000 << 7); // Shift amount (shamt5) is zero in this case
            machinecode |= (0b000 << 4); // Sh value for STR
            reg = registers.find(operand3.substr(0,operand3.size()-1)); // This only works for post index but oh well
            if(reg != registers.end()){
                machinecode |= (reg->second);
            }
        } // else find the value of the immediate operand option

    }

    if(instruction == "ASR"){

        // AL condition bits
        machinecode |= (0b1110 << 28);

        // OP code bits
        machinecode |= (0b00 << 26);

        // Immediate bit
        auto reg = registers.find(operand3);
        if(reg != registers.end()){
            machinecode |= (0 << 25);
        } else machinecode |= (1 << 25);

        // Cmd bits
        machinecode |= (0b1101 << 21);

        // S bit (condition flags)
        machinecode |= (0 << 20);

        machinecode |= (0b0000 << 16);

        // Destination register
        reg = registers.find(operand1);
        if(reg != registers.end()){
            machinecode |= (reg->second << 12);
        }

        // Register shift
        reg = registers.find(operand3);
        if(reg != registers.end()){
            machinecode |= (reg->second << 8);
        }

        // 0 bit
        machinecode |= (0 << 7);

        // sh bits for ASR = 10
        machinecode |= (0b101 << 4);

        // Rm bits
        reg = registers.find(operand2);
        if(reg != registers.end()){
            machinecode |= (reg->second);
        }
        
    }

    return machinecode;
}

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
                        machinecode |= (0b0000 << 4);
                    } else if(instruction == "ASR"){
                        machinecode |= (0b0100 << 4);
                    } else if(instruction == "LSR"){
                        machinecode |= (0b0010 << 4);
                    } else if(instruction == "ROR"){
                        machinecode |= (0b0110 << 4);
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
        std::cout << "In the immediate check clause.\n";
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
