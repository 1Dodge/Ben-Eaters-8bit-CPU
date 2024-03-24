#include <iostream>
#include <fstream>
#include <cstring>

#define HLT 0b1000000000000000 // HALT CPU
#define MI  0b0100000000000000 // MAR IN
#define RI  0b0010000000000000 // RAM IN
#define RO  0b0001000000000000 // RAM OUT
#define IO  0b0000100000000000 // INSTR. REG IN
#define II  0b0000010000000000 // INSTR. REG OUT
#define AI  0b0000001000000000 // REG A IN
#define AO  0b0000000100000000 // REG A OUT
#define EO  0b0000000010000000 // SUM OUT
#define SU  0b0000000001000000 // SUBTRACT
#define BI  0b0000000000100000 // REG B IN
#define OI  0b0000000000010000 // OUTPUT IN
#define CE  0b0000000000001000 // COUNTER ENABLE
#define CO  0b0000000000000100 // COUNTER OUT
#define J   0b0000000000000010 // JUMP TO ADDR
#define FI  0b0000000000000001 // FLAGS IN (CF/ZF)

#define FLAGS_Z0C0 0
#define FLAGS_Z0C1 1
#define FLAGS_Z1C0 2
#define FLAGS_Z1C1 3

#define JC 0b0111
#define JZ 0b1000

uint16_t UCODE_TEMPLATE[16][8] = {
    { MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0 },  // 0000 - NOP
    { MI|CO, RO|II|CE, IO|MI, RO|AI, 0,           0, 0, 0 },  // 0001 - LDA
    { MI|CO, RO|II|CE, IO|MI, RO|BI, AI|EO|FI,    0, 0, 0 },  // 0010 - ADD
    { MI|CO, RO|II|CE, IO|MI, RO|BI, AI|EO|SU|FI, 0, 0, 0 },  // 0011 - SUB
    { MI|CO, RO|II|CE, IO|MI, AO|RI, 0,           0, 0, 0 },  // 0100 - STA
    { MI|CO, RO|II|CE, IO|AI, 0,     0,           0, 0, 0 },  // 0101 - LDI
    { MI|CO, RO|II|CE, IO|J,  0,     0,           0, 0, 0 },  // 0110 - JMP
    { MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0 },  // 0111 - JC
    { MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0 },  // 1000 - JZ
    { MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0 },  // 1001 - NOP
    { MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0 },  // 1010 - NOP
    { MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0 },  // 1011 - NOP
    { MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0 },  // 1100 - NOP
    { MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0 },  // 1101 - NOP
    { MI|CO, RO|II|CE, AO|OI, 0,     0,           0, 0, 0 },  // 1110 - OUT
    { MI|CO, RO|II|CE, HLT,   0,     0,           0, 0, 0 }   // 1111 - HLT
};

uint16_t ucode[4][16][8];

void initUCode() {
    // ZF = 0 | CF = 0
    memcpy(ucode[FLAGS_Z0C0], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));

    // ZF = 0 | CF = 1
    memcpy(ucode[FLAGS_Z0C1], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));
    ucode[FLAGS_Z0C1][JC][2] = IO|J;

    // ZF = 1 | CF = 0
    memcpy(ucode[FLAGS_Z1C0], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));
    ucode[FLAGS_Z1C0][JZ][2] = IO|J;

    // ZF = 1 | CF = 1
    memcpy(ucode[FLAGS_Z1C1], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));
    ucode[FLAGS_Z1C1][JC][2] = IO|J;
    ucode[FLAGS_Z1C1][JZ][2] = IO|J;
}

void writeHEXfile() {
    std::ofstream outfile("ucode.hex"); // Generate file

    // Write values
    outfile << "v2.0 raw\n";

    for(int ver = 0; ver < 4; ver++) {
        for(int inst = 0; inst < 16; inst++) {
            for(int step = 0; step < 8; step++) {
                outfile << std::hex << ucode[ver][inst][step] << std::endl;
            }
        }
    }

    outfile.close(); // close the file
}

int main() {
    
    initUCode();
    writeHEXfile();

    // Print out "unformated" HEX dump
    for(int ver = 0; ver < 4; ver++) {
        for(int inst = 1; inst <= 16; inst++) {
            for(int step = 0; step < 8; step++) {
                std::cout << std::hex << ucode[ver][inst - 1][step] << " ";
                if(step == 7) std::cout << " ";
            }
            if(inst % 2 == 0) std::cout << "\n";
        }
        std::cout << "\n\n";
    }
}