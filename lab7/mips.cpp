/*
File: lab.cpp
Description: Read in file and silmulates mips
Name: Dong Jun Woun
Date: 10/30/2020
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdio>
#include <cstdint>
#include <string>
#include <string.h>
#include <stdlib.h>
using namespace std;

// array of the register names
const string reg_names[] = {
    "$zero",
    "$at",
    "$v0",
    "$v1",
    "$a0",
    "$a1",
    "$a2",
    "$a3",
    "$t0",
    "$t1",
    "$t2",
    "$t3",
    "$t4",
    "$t5",
    "$t6",
    "$t7",
    "$s0",
    "$s1",
    "$s2",
    "$s3",
    "$s4",
    "$s5",
    "$s6",
    "$s7",
    "$t8",
    "$t9",
    "$k0",
    "$k1",
    "$gp",
    "$sp",
    "$fp",
    "$ra"
};

// Number of registers/ global constant
const uint32_t NUM_REGS = sizeof(reg_names) / sizeof(reg_names[0]);

struct Machine {
    // struct machine with the data types defined
    unsigned int regs[NUM_REGS];
    unsigned int pc;
    unsigned int num_instructions;
    unsigned int* instructions;

    // ____function that compares a string and the register names____
    // if they are alike, the value of the register is stored into the passed in val
    bool reg(const string& name, uint32_t& val)
    {
        for (unsigned int i = 0; i < NUM_REGS; i++)
        {
            if (name == reg_names[i])
            {
                val = regs[i];
                return true;
            }
        }
        return false;
    }


    // ____function that runs the instructions____
    bool run_Instr() {

        // returns false if the program is done reading all the instructions
        // PC/4 is greater or equal to  the number of instructions =>done
        if ((pc / 4) >= num_instructions) {
            printf("No more instructions to run.\n");

            return false;
        }


        // assigned instruction 
        // TA advised me to do, so I do not ever change the instructions itself
        // read in the opcode by shifting 26bits and masking
        unsigned int instr = instructions[pc / 4];
        unsigned int opcode = (instr >> 26) & 0x3f;


        // used switch to match opcode
        switch (opcode) {
        case 0x0://special
        {
            //special have the same instruction format
            // 36-26/ 25-21/ 20-16/ 15-11/ 10-6/ 5-0
            // it is noted that it is not always stored as "rs, rt, rd"
            unsigned int subopcode = instr & 0x3f;
            int rs = (instr >> 21) & 0x1f;
            int rt = (instr >> 16) & 0x1f;
            int rd = (instr >> 11) & 0x1f;
            int sa = (instr >> 6) & 0x1f;



            switch (subopcode) { // switch to match subopcode
            case 0x20: // add
            {

                // rd represents $zero register, and it cannot be moddified
                if (rd == 0) { break; }

                // Add rd, rs, rt
                regs[rd] = regs[rs] + regs[rt];
                break;
            }
            case 0x24: // and
            {

                if (rd == 0) { break; }

                // And rd, rs, rt
                regs[rd] = regs[rs] & regs[rt];
                break;
            }
            case 0x25: // or
            {
                if (rd == 0) { break; }

                // Or rd, rs, rt
                regs[rd] = regs[rs] | regs[rt];
                break;
            }
            case 0x0: // sll
            {
                if (rd == 0) { break; }

                // sll rd, rt, sa
                // shift left by sa increment
                regs[rd] = regs[rt] << sa;
                break;
            }
            case 0x2A: // slt
            {

                if (rd == 0) { break; }

                // slt rd, rs, rt
                // sets reg rd as 1 if reg rt is larger than reg rs
                // else reg rd is set to 0
                if (regs[rs] < regs[rt]) {
                    regs[rd] = 1;
                }
                else { regs[rd] = 0; }

                break;
            }
            case 0x3: // sra
            {
                if (rd == 0) { break; }

                // arirthmetic shifting
                // must sign extend so I put the register into a temporary signed 
                // then bitshifted 
                int temp = regs[rt];
                temp = temp >> sa;
                regs[rd] = temp;
                break;
            }
            case 0x2: // srl
            {
                if (rd == 0) { break; }

                // logical shifting
                // doesn't have to be sign extended, so bitshift in unsigned int form
                regs[rd] = regs[rt] >> sa;

                break;
            }
            // if none of the instructions are implemented, the program notifies the user nothing happened
            default:
            {
                printf("Unknown instruction...continuing.\n");
            }
            }

            break;
        }
        case 0x8://addi
        {

            // gets rs rt and the immediate by shifting/masking
            int rs = (instr >> 21) & 0x1f;
            int rt = (instr >> 16) & 0x1f;
            int16_t immediate = (instr) & 0xFFFF;

            // rt is the destination register
            // 0 is the $zero register, so it should not be modified
            if (rt == 0) { break; }

            // implement to all functions


            regs[rt] = regs[rs] + immediate;


            break;
        }
        case 0x4://BEQ
        {
            // gets rs rt and the immediate by shifting/masking
            int rs = (instr >> 21) & 0x1f;
            int rt = (instr >> 16) & 0x1f;
            int16_t immediate = (instr) & 0xFFFF;

            // bitshift the immediate by 2(mips instructions)
            int signedimmediate = immediate << 2;
            if (regs[rs] == regs[rt]) {
                // branch if  equal =>beq
                    // programmer counter equals the 
                pc = pc + signedimmediate;
            }

            break;
        }
        case 0x5://BNE
        {
            // gets rs rt and the immediate by shifting/masking
            int rs = (instr >> 21) & 0x1f;
            int rt = (instr >> 16) & 0x1f;
            int16_t immediate = (instr) & 0xFFFF;

            // bitshift the immediate by 2(mips instructions)
            int signedimmediate = immediate << 2;
            if (regs[rs] != regs[rt]) {
                // branch if not equal =>bne
                    // programmer counter equals the 
                pc = pc + signedimmediate;
            }
            break;
        }
        default:
        {
            printf("Unknown instruction...continuing.\n");
        }
        // if none of the instructions are implemented, the program notifies the user nothing happened

        }

        // incrementing the pc counter
        pc += 4;
        return true;
    }

    // ____function that initializes the machine____
    Machine* machine(string file)
    {

        // declare new pointer for machine
        Machine* mips = new Machine;

        FILE* fl;
        fl = fopen(file.c_str(), "rb");
        //if the file does not open the user is notified the bmp file did not open(fail)
        // and mips is set to nullptr again
        if (nullptr == fl) {
            printf("Unable to open file %s \n", file.c_str());
            mips = nullptr;
            return mips;
        }


        // read number of instructions
        fseek(fl, 0, SEEK_END);
        mips->num_instructions = ftell(fl) / 4;

        // sets the array of instructions based on the number of instructions needed
        mips->instructions = new unsigned int[mips->num_instructions];



        // fread all the instructions
        fseek(fl, 0, SEEK_SET);
        fread(mips->instructions, 1, (sizeof(mips->regs) * mips->num_instructions), fl);

        // file not needed close
        fclose(fl);


        // return the address
        return mips;
    }

    //____function that prints out the registers____
    void print()
    {
        for (int i = 0; i < 32; i++) {
            if ((i % 4 == 0) && (i != 0)) {
                printf("\n");
            }



            printf("%-5s: 0x%08X (%5d) ", reg_names[i].c_str(), (signed int)regs[i], (signed int)regs[i]);
            if (i == 31) {
                printf("\nPC: %u\n", pc);
            }
        }
    }


};


// const unsigned int NUM_REGS = 32;
int main() {

    //load user command
    string uInput;
    string command;
    string fileOregister;

    //pointer equal to nullptr
    Machine* mips = nullptr;

    do
    {

        // prompting the user for input
        while ((printf("> ")) && !(getline(cin, uInput)))
        {

            if ((cin.eof())) {
                // EOF signal close the program
                break;
            }

            // error checking
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printf("> ");
        }
        if ((cin.eof())) {
            // EOF signal close the program
            break;
        }
        // stringstream the user's input to two separate strings
        // command saves the command
        // fileOregister saves the second input 
        // ** noted fileOregister may be a integer for reg
        stringstream ss(uInput);
        ss >> command >> fileOregister;



        if (command == "load")
        {
            if (mips == nullptr) {
                // calls the machine function 
                // function returns address, mips's address is set to that returned address
                mips = mips->machine(fileOregister);
            }
            else
            {
                // if mips is not nullptr address exists so delete and create new 
                delete mips;
                Machine* mips = nullptr;

                mips = mips->machine(fileOregister);
            }
            //sets pc to 0
            mips->pc = 0;
            // clear registers to 0(make sure)
            for (int i = 0; i < 32; i++) {
                mips->regs[i] = 0;
            }
        }
        else if (command == "reg")
        {

            if (mips == nullptr)
            {
                //if equal to nullptr, machine is not loaded yet
                printf("Machine not loaded\n");
            }
            else
            {
                // vall will save the register value
                uint32_t val;

                // stringstreamed fileOregister for an integer
                // if so, the integer is matched with a register name
                // that register name is saved to the fileOregister
                // else nothing happens
                stringstream Nin(fileOregister);
                int reg;
                if (Nin >> reg) {
                    for (int i = 0; i < 32; i++) {
                        if (reg == i) {
                            fileOregister = reg_names[i];
                        }
                    }
                }

                // fileoregister and val is passed into the reg() function
                // fileOregister will represent the Register name
                // val will save the register value(happens in function)
                if (mips->reg(fileOregister, val))
                {
                    printf("%-5s: 0x%08X (   %i) \n", fileOregister.c_str(), val, val);
                }
                else {
                    // else Register does not exist is printed
                    printf("Register does not exist\n");
                }
            }


        }
        else if (command == "regs")
        {

            if (mips == nullptr)
            {
                //if equal to nullptr, machine is not loaded yet
                printf("Machine not loaded\n");
            }
            else {
                // calls print() and prints all registers 
                mips->print();
            }
        }

        else if (command == "next")
        {
            if (mips == nullptr)
            {
                //if equal to nullptr, machine is not loaded yet
                printf("Machine not loaded\n");
            }
            else
            {
                // else run Instruction function runs an instruction
                mips->run_Instr();
            }
        }
        else if (command == "run")
        {
            if (mips == nullptr)
            {
                //if equal to nullptr, machine is not loaded yet
                printf("Machine not loaded\n");
            }
            else
            {
                // else run Instruction function until it is false(bool function)
                while (mips->run_Instr()) {}
            }
        }
        else if (command == "quit")
        {
            // separates Unknown command and quit command
        }
        else
        {
            // if none of the commands match, Unknown command
            printf("Unknown Command\n");
        }

        // the string is cleared so it doesn't effect the next instruction
        fileOregister.clear();

    } while (command != "quit");

    // when the user quits the pointer is deleted
    delete mips;

    return 0;
}