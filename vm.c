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
    LO QUE FALTA:
    - Terminar print
    - Llamar a print en todos los ifs


*/


#include <stdio.h>
#include <stdlib.h>

// these two are global
typedef struct IR
{
    // operation code
    int OP;
    // lexicographical level for instructions that access variables in other activation records
    int L;
    // its meaning depends on the opcode
    int M;
} IR;

// points to next instruction in the text segment
int PC = 0;

// Pas array declared and every element is initialized to 0
//  needs text segment, stack segment, unallocated region
// first instruction occupies 012, second 345
// stack segments starts at 480 and grows downward, runtime data stored here
// from 499 to 481 reserved region
int pas[500] = {0};


/*IDEA: tenemos dos arrays para hacer el print, */


//PROTOTYPES
int base(int BP, int L);
void printAll(int SP, int BP, int PC, int OPcode, int arr[], char instr[], char subOP[]);


//MAIN
int main(int argc, char *argv[])
{
    //para el print
    char instructions[9] = {"LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SYS"};
    char OP2instructions[11] = {"RTN", "NEG", "ADD", "SUB", "MUL", "DIV", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"}; 
    

    if (argc > 1)
    {
        //File pointer is initialized
        FILE *fp = NULL;

        //File is opened and read
        fp = fopen(argv[1], "r");

        //Used to read each number
        int num;
        //Counter used to store numbers into pas
        int count = 0; 

        //As long as each scan is valid, the numbers will get stored in pas
        while(fscanf(fp, "%d", &num) == 1)
        {
            if(num != EOF)
            {
                pas[count] = num;

            }

            count++;

        }

        //File pointer is closed
        fclose(fp);

    }
    else
    {
        printf("No extra arguments provided.\n");
        printf("Try: ./00_args 123\n");
    }

    


    // points to the base of the current activation record on the stack
    int BP = 480;
    // points to the top of the stack
    int SP = 481;

    // holds the OP,L,M fields of the instruction currently being executed
    IR curInstruction;

    //Prints the Header (that way it doesn't print every time print is called)
    printf("\t\tL\tM\tPC\tBP\tSP\tstack\n");
    printf("Initial values:\t\t\t%d\t%d\t%d\n", PC, BP, SP);

    // this has to be within a loop, terminating condition SYS 0 3
    while (pas[PC] != 9 && pas[PC + 1] != 0 && pas[PC + 2] != 3)
    {

        curInstruction.OP = pas[PC];
        curInstruction.L = pas[PC + 1];
        curInstruction.M = pas[PC + 2];
        PC += 3;

        // LIT: push literal
        if (curInstruction.OP == 1)
        {
            SP++;
            pas[SP] = curInstruction.M;
        }

        // OPR: arithemtic operations

        if (curInstruction.OP == 2)
        {
            // RTN return subroutine
            if (curInstruction.M == 0)
            {
                SP--;
                BP = pas[SP + 2];
                PC = pas[SP + 3];
            }

            // NEG negation
            if (curInstruction.M == 1)
            {
                pas[SP] = -pas[SP];
            }

            // ADD addition
            if (curInstruction.M == 2)
            {
                pas[SP - 1] = pas[SP - 1] + pas[SP];
                SP--;
            }

            // SUB subtraction
            if (curInstruction.M == 3)
            {
                pas[SP - 1] = pas[SP - 1] - pas[SP];
                SP--;
            }

            // MUL multiplication
            if (curInstruction.M == 4)
            {
                pas[SP - 1] = pas[SP - 1] * pas[SP];
                SP--;
            }

            // DIV integer division
            if (curInstruction.M == 5)
            {
                pas[SP - 1] = pas[SP - 1] / pas[SP];
                SP--;
            }

            // EQL equality comparison
            if (curInstruction.M == 6)
            {
                pas[SP - 1] = (pas[SP - 1] == pas[SP]);
                SP--;
            }

            // NEQ inequality comparison
            if (curInstruction.M == 7)
            {
                pas[SP - 1] = (pas[SP - 1] != pas[SP]);
                SP--;
            }

            // LSS less-than comparison
            if (curInstruction.M == 8)
            {
                pas[SP - 1] = (pas[SP - 1] < pas[SP]);
                SP--;
            }

            // LEQ less or equal
            if (curInstruction.M == 9)
            {
                pas[SP - 1] = (pas[SP - 1] <= pas[SP]);
                SP--;
            }

            // GTR greater than
            if (curInstruction.M == 10)
            {
                pas[SP - 1] = (pas[SP - 1] > pas[SP]);
                SP--;
            }

            // GTR greater or equal than
            if (curInstruction.M == 11)
            {
                pas[SP - 1] = (pas[SP - 1] >= pas[SP]);
                SP--;
            }
        }

        // LOD: load to the stack
        if (curInstruction.OP == 3)
        {
            SP++;
            pas[SP] = pas[base(BP, curInstruction.L) + curInstruction.M];
        }

        // STO: store from stack
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

        // INC: allocate locals
        if (curInstruction.OP == 6)
        {
            SP = SP + curInstruction.M;
        }

        // JMP: unconditional jump
        if (curInstruction.OP == 7)
        {
            PC = curInstruction.M;
        }

        // JPC: conditional jump
        if (curInstruction.OP == 8)
        {
            if (pas[SP] == 0)
            {
                PC = curInstruction.M;
                SP--;
            }
        }

        if (curInstruction.OP == 9)
        {
            
            // OUTPUT integer (print)
            if (curInstruction.M == 1)
            {
               
                printf("Output result is: %d", pas[SP]);
                SP--;
            }
            // READ integer
            if (curInstruction.M == 2)
            {
                int num = 0;

                scanf("Please Enter an Integer: %d", &num);
                SP++;
                pas[SP] = num;
               
            }
            // Halt the program
            if (curInstruction.M == 3)
            { 
                break;
            }
            
        }
    }

    return 0;
}







/* Find base L levels down from the current activation record */
int base(int BP, int L)
{
    int arb = BP; // activation record base
    while (L > 0)
    {
        arb = pas[arb]; // follow static link
        L--;
    }
    return arb;
}




//char instr[]: contiene los opcodes
//char subOP[]: contiene los opcodes para OPR
void printAll(int SP, int BP, int PC, int OPcode, int arr[], char instr[], char subOP[]){


    //print function// 

    char instructName = "";
    
    for (int i = 1; i < 10; i++){

        if(OPcode == i){
            instructName = arr[i];
            break;
        }

        if (OPcode == i && i == 2){

            //otro loop para buscar el nombre en el otro array
        }
    }



    //print 


}
