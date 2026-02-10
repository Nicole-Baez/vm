# AI Usage Disclosure Details

**Student Name:** Nicole Baez Espinosa, Lianet Martin
**Student ID:** 5544379,
**Assignment:** Homework 1

---

## Instructions

Complete this template with detailed information about your AI usage. Submit this file along with your signed PDF declaration form. Include the conversation log in order, with each round listing the prompt, AI output, and verification/edits.

---

## AI Tool #1

### Tool Name

Claude

### Version/Model

Opus 4.6

### Date(s) Used

February 9th, 2026; February 10th, 2026

### Specific Parts of Assignment

For debugging our printAll() function because it kept printing erroneous output. We also used it
to debug instructions like MUL and STO which were incorrectly calculating and placing values on the
stack.

### Conversation Log

#### Round 1

**Prompt:**
if (curInstruction.OP == 8)
{
// printf("lalalla%d", pas[SP]);
if (pas[SP] == 0)
{
PC = curInstruction.M;
SP++;
    }
} What is wrong with this statement?
**AI Output:** The stack needs to be popped regardless of whether the condition is true or not.
if (pas[SP] == 0)
{
    PC = curInstruction.M;
}
SP++; //Stack is always popped
**Verification/Edits:** I made the modification, but the PC was still giving me 33 instead of 39.

#### Round 2

**Prompt:** The desired output for PC should be 39 and I'm getting 33.
**AI Output:** The problem is that it IS doing a jump when it SHOULDN't. The issue is likely in this if statement.
if (curInstruction.M == 8)
{
    pas[SP + 1] = (pas[SP + 1] < pas[SP]);
    SP++;
}
**Verification/Edits:** I moved the SP pointer back inside the if statement.

#### Round 3

**Prompt:**
if (curInstruction.M == 3)
{
printf("lalallala");
printAll(SP, BP, curInstruction, instructions, OP2instructions);
break;
} Does the break interrupt the previous?
**AI Output:** No, the break only exits the nearest loop. In your case, this break
is inside the if (curInstruction.M == 3), which is inside the if (curInstruction.OP == 9), which
is inside the main while loop.
The break does interrupt the while loop and terminates the loop as per the SYS 0 3 instruction.
BUT there is an issue:
You are calling printAll before break and then by the end of your while loop you most likely have another
printAll, meaning SYS 0 3 will print twice.
The solution:

if (curInstruction.M == 3)
{
    // DO NOT CALL PRINTALL HERE
    // The break will be placed AFTER this print
}
At the end of the while loop, make sure you have:

printAll(SP, BP, curInstruction, instructions, OP2instructions);
 // While loop ends
This way SYS 0 3 only prints once before halting.
Or if you'd prefer to do it inside the if:

c
if (curInstruction.M == 3)
{
    printAll(SP, BP, curInstruction, instructions, OP2instructions);
    break;  // Break from while
}

**Verification/Edits:** I moved the printAll call right before the if statement but it was still not printing the HALT condition.

#### Round 4

**Prompt:**
L    M    PC    BP    SP    stack
Initial values:            0    480    481
JMP         0     45     45     480     481
INC         0     5     48     480     476
Please Enter an Integer: 8
SYS         0     2     51     480     475
CAL         0     3     3     474     475
JMP         0     6     6     474     475
INC         0     4     9     474     471
LIT         0     4     12     474     470
LIT         0     3     15     474     469
MUL         0     4     18     474     470
STO         1     4     21     474     471
LIT         0     14     24     474     470
LOD         1     4     27     474     469
LSS         0     8     30     474     470
JPC         0     39     39     474     471
LIT         0     5     42     474     470
RTN         0     0     54     480     475
Output result is: 8
SYS         0     1     57     480     476
lianetmartin@Lianets-Laptop vm %
But it is not printing the last one. Show more.
**AI Output:**
The issue with your code is due to the break you have inside the if (curInstruction.M == 3) condition.
You should exit the while loop BEFORE reaching the last printAll. Take a look at your structure:
while (!(pas[PC] == 9 && pas[PC + 1] == 0 && pas[PC + 2] == 3))
{
    // fetch
    curInstruction.OP = pas[PC];
    curInstruction.L = pas[PC + 1];
    curInstruction.M = pas[PC + 2];
    PC += 3;

// ... all the ifs ...

if (curInstruction.OP == 9)
    {
        if (curInstruction.M == 3)
        {
            break;  // ← Exits here, it never reaches the function call to printAll
        }
    }

printAll(SP, BP, curInstruction, instructions, OP2instructions);
}
The solution:

Switch this if (curInstruction.M == 3) to:

if (curInstruction.M == 3)
{
    printAll(SP, BP, curInstruction, instructions, OP2instructions);
    break;  // Now it prints BEFORE exiting the loop
}
Or better yet, switch the condition inside the while loop to:

c
while (1)  // Infinite loop
{
    // fetch...
    // execute...

printAll(SP, BP, curInstruction, instructions, OP2instructions);

if (curInstruction.OP == 9 && curInstruction.M == 3)
    {
        break;  // Exits AFTER printing
    }
}
This way SYS 0 3 is printed before the program is halted.

**Verification/Edits:** I changed the condition of my while loop to while(1). I debugged to check
it was fetching the instructions correctly.

#### Round 5

**Prompt:**
lianetmartin@Lianets-Laptop vm % ./vm input.txt
        L    M    PC    BP    SP    stack
Initial values:            0    480    481
DEBUG: Fetched OP=7 L=0 M=45 at PC=-3
JMP         0     45     45     480     481
DEBUG: Fetched OP=6 L=0 M=5 at PC=42
INC         0     5     48     480     476
DEBUG: Fetched OP=9 L=0 M=2 at PC=45
Please Enter an Integer: 8
SYS         0     2     51     480     475
DEBUG: Fetched OP=5 L=0 M=3 at PC=48
CAL         0     3     3     474     475
DEBUG: Fetched OP=7 L=0 M=6 at PC=0
JMP         0     6     6     474     475
DEBUG: Fetched OP=6 L=0 M=4 at PC=3
INC         0     4     9     474     471
DEBUG: Fetched OP=1 L=0 M=4 at PC=6
LIT         0     4     12     474     470
DEBUG: Fetched OP=1 L=0 M=3 at PC=9
LIT         0     3     15     474     469
DEBUG: Fetched OP=2 L=0 M=4 at PC=12
MUL         0     4     18     474     470
DEBUG: Fetched OP=4 L=1 M=4 at PC=15
STO         1     4     21     474     471
DEBUG: Fetched OP=1 L=0 M=14 at PC=18
LIT         0     14     24     474     470
DEBUG: Fetched OP=3 L=1 M=4 at PC=21
LOD         1     4     27     474     469
DEBUG: Fetched OP=2 L=0 M=8 at PC=24
LSS         0     8     30     474     470
DEBUG: Fetched OP=8 L=0 M=39 at PC=27
JPC         0     39     39     474     471
DEBUG: Fetched OP=1 L=0 M=5 at PC=36
LIT         0     5     42     474     470
DEBUG: Fetched OP=2 L=0 M=0 at PC=39
RTN         0     0     54     480     475
DEBUG: Fetched OP=9 L=0 M=1 at PC=51
Output result is: 8
SYS         0     1     57     480     476
DEBUG: Fetched OP=9 L=0 M=3 at PC=54
lianetmartin@Lianets-Laptop vm %
It is fetching correctly

**AI Output:**
Perfect! I can see "DEBUG: Fetched OP=9 L=0 M=3 at PC=54" - so SYS 0 3 is being fetched.
The problem is it's not printing after being fetched. This means your code is breaking before it gets to printAll.
Check your SYS 0 3 handler. You probably have something like:

c
if (curInstruction.OP == 9)
{
    // ... SYS 0 1 and SYS 0 2 ...

if (curInstruction.M == 3)
    {
        break;  // ← This breaks BEFORE printAll!
    }
}

printAll(SP, BP, curInstruction, instructions, OP2instructions);  // Never reaches here
Fix it by removing the break from inside the if block:

c
if (curInstruction.OP == 9)
{
    // ... SYS 0 1 and SYS 0 2 ...

if (curInstruction.M == 3)
    {
        // Do nothing, just let it fall through to printAll
    }
}

printAll(SP, BP, curInstruction, instructions, OP2instructions);

// Break AFTER printing
if (curInstruction.OP == 9 && curInstruction.M == 3)
{
    break;
}
This way: fetch → execute → print → then break.

**Verification/Edits:** I made these changes but my output was still not matching the sample output.
It was not printing the instructions. It was just printing SYS 0 1 and SYS 0 2.

#### Round 6

**Prompt:**
lianetmartin@Mac vm % ./vm input.txt
L M PC BP SP stack
Initial values: 0 480 481
Please Enter an Integer: 8
Output result is: 8
lianetmartin@Mac vm %
Why is my output this?

**AI Output:**
Looking at your output, it appears you have a virtual machine (VM) that's running a program from input.txt. The output shows:
Register/memory state display - showing L, M, PC (Program Counter), BP (Base Pointer), SP (Stack Pointer), and stack contents
Initial values: PC=0, BP=480, SP=481
Prompt for input: "Please Enter an Integer: 8"
Output: "Output result is: 8"
Why you're getting this output:
Your VM program appears to be doing something very simple:
It reads an integer from the user (you entered 8)
It immediately outputs that same integer (8)
This is likely because your input.txt file contains VM instructions that:
Execute a READ instruction (to get user input)
Execute a WRITE instruction (to output the value)
To understand why specifically:
Check your input.txt file - look at what instructions are in there. It probably has something like:
A SYS/READ instruction (opcode for reading input)
A SYS/WRITE instruction (opcode for writing output)
The blank lines between the prompt and output are likely just console formatting or debug output from your VM
If you expected different behavior, you may need to:
Modify the instructions in input.txt
Check if your VM is correctly executing the intended program
Would you like help understanding what instructions should be in input.txt to produce different behavior?
**Verification/Edits:** This prompt was not helpful and did not offer any helpful edits. I had to modify
it to get a more accurate answer.

#### Round 7

**Prompt:**
// MUL multiplication
if (curInstruction.M == 4)
{
pas[SP - 1] = pas[SP - 1] \* pas[SP];
SP++;  
 }

// LSS less-than comparison
if (curInstruction.M == 8)
{
pas[SP + 1] = (pas[SP + 1] < pas[SP]);  
 SP++;  
 }

/\*void printAll(int SP, int BP, IR currentIR, const char instr[][4], const char subOP[][4])
{

    char instName[4];
    const char *src = instr[currentIR.OP];

    // OPR uses sub-operation
    if (currentIR.OP == 2)
    {
        src = subOP[currentIR.M];
    }

    instName[0] = src[0];
    instName[1] = src[1];
    instName[2] = src[2];
    instName[3] = '\0';

    // print instruction + registers
    printf("%s \t\t%d \t%d \t%d \t%d \t%d\t ", instName, currentIR.L, currentIR.M, PC, BP, SP);
    int currentAR = BP;

    for (int i = 480; i > SP - 1; i--)
    {
        printf("%d ", pas[i]);
    }

    printf("\n");

}\*/

Here are some if statements and my printAll function which is commented out. What is wrong with this logic and what
changes can I make?

**AI Output:**
Looking at your code, I can see the issue! Your program is reading an integer and then outputting it, which is exactly what you're seeing. But there are several bugs in your code that are affecting the output:
Main Issues:
The actual printing logic is commented out at the bottom, so you're not seeing the execution trace you should be seeing.
Excessive newlines - You're calling printAll() after every instruction, and it only prints \n, which is why you see all those blank lines.
MUL instruction bug (line 156):

c
// MUL multiplication
if (curInstruction.M == 4)
{
pas[SP - 1] = pas[SP - 1] \* pas[SP];
SP++; // ❌ WRONG - should be SP--
}
This should decrement SP, not increment it.
LSS instruction bug (line 177):

c
// LSS less-than comparison
if (curInstruction.M == 8)
{
pas[SP + 1] = (pas[SP + 1] < pas[SP]); // ❌ WRONG indices
SP++; // ❌ WRONG direction
}
Should be:

c
pas[SP - 1] = (pas[SP - 1] < pas[SP]);
SP--;
To fix your output:
Uncomment the print logic in printAll():

c
void printAll(int SP, int BP, IR currentIR, const char instr[][4], const char subOP[][4])
{
char instName[4];
const char \*src = instr[currentIR.OP];

    // OPR uses sub-operation
    if (currentIR.OP == 2)
    {
        src = subOP[currentIR.M];
    }

    instName[0] = src[0];
    instName[1] = src[1];
    instName[2] = src[2];
    instName[3] = '\0';

    // print instruction + registers
    printf("%s\t%d\t%d\t%d\t%d\t%d\t", instName, currentIR.L, currentIR.M, PC, BP, SP);

    // Print stack contents from bottom to top
    for (int i = 479; i >= SP; i--)
    {
        printf("%d ", pas[i]);
    }

    printf("\n");

}
This will give you a proper execution trace showing each instruction and the state of the VM after execution.
**Verification/Edits:** I decremented the SP pointer in both MUL and LSS and I also uncommented my printAll function. These
modifications worked and

#### Round 8

**Prompt:**
void printAll(int SP, int BP, IR currentIR, const char instr[][4], const char subOP[][4])
{
char instName[4];
const char \*src = instr[currentIR.OP];
// OPR uses sub-operation
if (currentIR.OP == 2)
{
src = subOP[currentIR.M];
}
instName[0] = src[0];
instName[1] = src[1];
instName[2] = src[2];
instName[3] = '\0';
// print instruction + registers
printf("%s \t\t%d \t%d \t%d \t%d \t%d\t ", instName, currentIR.L, currentIR.M, PC, BP, SP);
int currentAR = BP;
for (int i = 480; i > SP - 1; i--)
{
printf("%d ", pas[i]);
}
printf("\n");
}
I just need to print the bar using this logic.

**AI Output:**
To print the bar (|) separating activation records in your stack, you need to track where each activation record begins. Here's the updated printAll() function:

void printAll(int SP, int BP, IR currentIR, const char instr[][4], const char subOP[][4])
{
char instName[4];
const char \*src = instr[currentIR.OP];

    // OPR uses sub-operation
    if (currentIR.OP == 2)
    {
        src = subOP[currentIR.M];
    }

    instName[0] = src[0];
    instName[1] = src[1];
    instName[2] = src[2];
    instName[3] = '\0';

    // print instruction + registers
    printf("%s\t%d\t%d\t%d\t%d\t%d\t", instName, currentIR.L, currentIR.M, PC, BP, SP);

    // Print stack with bars separating activation records
    int currentAR = BP;
    for (int i = 479; i >= SP; i--)
    {
        // Print bar before each new activation record
        if (i == currentAR && currentAR != 480)
        {
            printf("| ");
            // Follow the dynamic link to find the previous AR
            currentAR = pas[currentAR - 1];
        }
        printf("%d ", pas[i]);
    }

    printf("\n");

}
Key points:
currentAR starts at the current BP (base of current activation record)
When i reaches currentAR, print | before the value
Follow the dynamic link at pas[currentAR - 1] to find the previous AR's base
The condition currentAR != 480 prevents printing a bar before the first (main) activation record
This will print a | separator at each activation record boundary as you traverse the stack from bottom (479) to top (SP).

**Verification/Edits:** I made these modifications to my code and it printed the bar before the next activation record. The only
other issues I was having were with MUL and STO. I increased SP in MUL which gave the 12 and I decreased the base by M instead
of adding in the STO instruction.

### Learning & Reflection

We practiced proper debugging techniques with AI. By looking at each variable and going instruction by instruction, we were able
to slowly trace our code and also build better prompts for the AI to help debug specific sections of our code.

---

## AI Tool #2 (if applicable)

Not applicable

## AI Tool #3 (if applicable)

Not applicable

## Overall Reflection

Using AI in this assignment helped us gain a deeper understand of how a virtual machine works outside of the theoretical realm.
AI tools helped us understand concepts such as stack growth direction, activation records, static links and dynamic links, and how this
all behaves in a real world environment. We ensured our work remained our own by checking in with each other whenever we made a change
and by also tracing the output instruction by instruction to make sure our vm was behaving accordingly.

---

## Notes

- Be as specific and detailed as possible in your responses
- Keep the conversation rounds in order
- Include exact prompts and AI outputs when possible
- Explain how you verified and modified AI-generated content
- Reflect on what you learned through the AI interaction
