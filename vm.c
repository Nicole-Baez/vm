/*
    Assignment:
    vm.c - Implement a P-machine virtual machine
    Authors: Nicole Baez Espinosa, Lianet Martin
    Language: C (only)
    To Compile:
    gcc -O2 -Wall -std=c11 -o vm vm.c
    To Execute (on Eustis):
    ./vm input.txt
    where:
    input.txt is the name of the file containing PM/0 instructions;
    each line has three integers (OP L M)
    Notes:
    - Implements the PM/0 virtual machine described in the homework
    instructions.
    - No dynamic memory allocation, no pointer arithmetic, and
    no function-like macros.
    - Does not implement any VM instruction using a separate function.
    - Runs on Eustis.
    Class: COP 3402 - Systems Software - Spring 2026
    Instructor : Dr. Jie Lin
    Due Date: Monday, February 9th, 2026
*/



/*
        STEPS:
        1. READ PM INSTRUCTIONS AND STORE THEM IN AN ARRAY
        2. MAKE AN IR STRUCT
        3. HAVE A LOOP THAT EXTRACTS THE INSTRUCTION AND EXECUTES IT (3 INTEGERS PER INSTRUCTION, BASED ON PC)
        
    
*/



#include <stdio.h>
#include <stdlib.h>


//these two are global
typedef struct IR
{

    int OP;
    int L;
    int M;
}IR;


int PC = 0;
//Pas array declared and every element is initialized to 0
int pas[500];

/* Find base L levels down from the current activation record */
int base(int BP, int L) {
    int arb = BP; // activation record base
    while (L > 0) {
        arb = pas[arb]; // follow static link
        L--;
    }
    return arb;
}


int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);

    for(int i = 0; i < 500; i++)
    {
        pas[i] = 0;
    }


    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
        
        if(argv[i] != "/n"){
            pas[i] = atoi(argv[i]);
        }
    }
        printf("\n");
    if (argc > 1)
    {
        int x = atoi(argv[1]); // convert string to int (simple)
        printf("Converted argv[1] to int: %d\n", x);

    }
    else
    {
        printf("No extra arguments provided.\n");
        printf("Try: ./00_args 123\n");
    }


    
    printf("pas[1] = %d", pas[1]);
    

    int BP = 480;
    int SP = 481;


    IR curInstruction;

    //this has to be within a loop, terminating condition SYS 0 3 
    while(pas[PC] != 9 && pas[PC + 1] != 0 && pas[PC + 2] != 3)
    {

        curInstruction.OP = pas[PC];
        curInstruction.L = pas[PC + 1];
        curInstruction.M = pas[PC + 2];
        PC += 3;

        //LIT: push literal
        if (curInstruction.OP == 1)
        {
            SP++;
            pas[SP] = curInstruction.M;


        }

        //OPR: arithemtic operations
        /*
        if (curInstruction.OP == 2)
        {
            
        }*/

        //LOD: load to the stack
        if (curInstruction.OP == 3)
        {
            SP++;
            pas[SP] = pas[base(BP, curInstruction.L) + curInstruction.M];

        }

        //STO: store from stack
        if (curInstruction.OP == 4)
        {
            pas[base(BP, curInstruction.L) + curInstruction.M] = pas[SP];
            SP--;
            
        }


        if (curInstruction.OP == 5)
        {
            pas[SP + 1] = base(BP, curInstruction.L);
            pas[SP + 2] = BP;
            pas[SP + 3] = PC;
            
            BP = SP + 1;
            PC = curInstruction.M;

        }

        //INC: allocate locals
        if (curInstruction.OP == 6)
        {
            SP = SP + curInstruction.M;
        }

        //JMP: unconditional jump
        if (curInstruction.OP == 7)
        {
            PC = curInstruction.M;
        }

        //JPC: conditional jump
        if (curInstruction.OP == 8)
        {
            if(pas[SP] == 0)
            {
                PC = curInstruction.M;
                SP--;
            }

        }

        /*
        if (curInstruction.OP == 9)
        {
            
        }*/




    }
    
    

    return 0;
}







