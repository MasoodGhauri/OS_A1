#include <iostream>
#include <windows.h>

using namespace std;

enum Opcode
{
    ADD,
    SUB,
    MULT,
    DIVD,
    LOAD,
    STORE,
    HALT,
    VAL
};

const int size = 256;

class Instruction
{
public:
    Opcode opcode;
    int operand;

    Instruction()
    {
        opcode = VAL;
        operand = 0;
    }

    // parametrized constructor
    Instruction(Opcode op, int operand) : opcode(op), operand(operand) {}
};

class RAM
{
private:
    static Instruction memory[];

public:
    // load array of instructions/program TO WORK ON
    static void loadRAM(Instruction A[], int size)
    {
        for (int i = 0; i < size; i++)
        {
            setAtMemory(i, A[i]);
        }
    }

    static void setAtMemory(int address, Instruction value)
    {
        memory[address] = value;
    }
    static void setAtMemory(int address, int value)
    {
        memory[address].opcode = VAL;
        memory[address].operand = value;
    }

    static Instruction getAtMemory(int address)
    {
        return memory[address];
    }
};

// decaring ram memory
Instruction RAM::memory[33];

class CPU
{
private:
    int PC = 0;
    int AC = 0;
    Instruction IR;

public:
    void run()
    {
        // fetch
        setIR(RAM::getAtMemory(PC));

        if (getIR().opcode == HALT)
        {
            cout << "Result: " << RAM::getAtMemory(32).operand << endl;
        }

        // decode and execute
        executeInstruction(IR);
    }

    void outputStates()
    {
        cout << "IR-Instruction -->> " << IR.opcode << " " << IR.operand << endl;
        cout << "PC-STATE -->> " << PC << endl;
        cout << "AC-STATE -->> " << AC << endl;
        cout << "RAM-STATE -->>\n";
        for (int i = 0; i < 33; i++)
        {
            cout << i << ". [" << RAM::getAtMemory(i).opcode << " " << RAM::getAtMemory(i).operand << "]\t";
        }
    }

    void setPC(int A)
    {
        PC = A;
    }

    void incrementPC()
    {
        PC++;
    }

    void setIR(Instruction A)
    {
        IR = A;
    }
    Instruction getIR()
    {
        return IR;
    }

    void executeInstruction(Instruction i)
    {
        Instruction temp;

        switch (i.opcode)
        {
        case ADD:
            AC += RAM::getAtMemory(i.operand).operand;
            incrementPC();
            break;
        case SUB:
            AC -= RAM::getAtMemory(i.operand).operand;
            incrementPC();
            break;
        case MULT:
            AC *= RAM::getAtMemory(i.operand).operand;
            incrementPC();
            break;
        case DIVD:
            AC /= RAM::getAtMemory(i.operand).operand;
            incrementPC();
            break;
        case LOAD:
            temp = RAM::getAtMemory(i.operand);
            if (temp.opcode == VAL)
            {
                AC = temp.operand;
                incrementPC();
            }
            else
            {
                setIR(temp);
            }
            break;
        case STORE:
            RAM::setAtMemory(i.operand, AC);
            incrementPC();
            break;
        case VAL:
            incrementPC();
            break;
        case HALT:
            cout << "HALT encountered. Exiting program with status " << i.operand << endl;
            exit(1);
        default:
            std::cout << "Invalid opcode encountered. Exiting program.\n";
            exit(1);
        }
    }
};

int main()
{
    Instruction BMI[] = {
        {VAL, 73}, // weight in kg
        {VAL, 2},  // height in meters
        {VAL, 1},
        {LOAD, 0},   // Load weight from memory
        {STORE, 31}, // Store weight in a temporary memory location
        {LOAD, 1},   // Load height from memory
        {MULT, 1},   // (weight * weight)
        {STORE, 30}, // Store height^2 in a temporary memory
        {LOAD, 31},  // load back weight from temporary memory
        {DIVD, 30},  // Divide by height^2
        {ADD, 2},    // Adding 1 as error threshold
        {STORE, 32}, // Store BMI result in memory address 2
        {HALT, 0}};

    Instruction percentage[] = {
        {VAL, 480},  // otained marks
        {VAL, 100},  // 100
        {VAL, 600},  // total marks
        {LOAD, 0},   // load marks
        {MULT, 1},   // multiply marks with 100
        {DIVD, 2},   // divide by total marks
        {STORE, 32}, // strore percentage
        {HALT, 0}};

    CPU cpu;

    //  program 1 for percentage calculation
    RAM::loadRAM(percentage, 8);

    cout << "================================\n";
    while (true)
    {
        cout << "\t---Before---\n";
        cpu.outputStates();
        cout << endl;
        cpu.run();
        cout << "\t---After---\n";
        cpu.outputStates();
        cout << endl;
    }

    //  program 2 for BMI calculation
    RAM::loadRAM(BMI, 13);
    cpu.setPC(0);

    cout << "================================\n";
    while (true)
    {
        cpu.run();
        cout << "\t---Before---\n";
        cpu.outputStates();
        cout << endl;
        cpu.run();
        cout << "\t---After---\n";
        cpu.outputStates();
        cout << endl;
    }

    return 0;
}
