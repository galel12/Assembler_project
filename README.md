# Assembler Project

This project is a ***two-pass assembler*** written in **C language**. It translates assembly language code into machine code. The assembler reads an input file containing assembly instructions and generates an output file with the corresponding machine code.

**Note:** The computer model for this project and the given assembly language are **imaginary**.

## **Project Structure**

- **`src`** - Contains the source code files (`.c` and `.h`).
- **`Tests`** - Contains test files and directories for testing the assembler.
- **`Tests/errors`** - Contains test files with errors for testing error handling.

## **Prerequisites**

To build and run the assembler, ensure you have the following installed:

- **GCC** (GNU Compiler Collection) or any compatible C compiler.
- **Make** (build automation tool).

## **Building the Project**

To build the project, navigate to the project directory and run:

```sh
make
```

This will:

- Compile the source code.
- Generate the executable file `assembler.exe` in the main directory.
- Copy `assembler.exe` to the `Tests/` and `Tests/errors/` directories.

## **Running the Assembler**

To run the assembler, use the following command:

```sh
./assembler <input_file>
```

After preparing assembly files **with an `.as` extension**, open *terminal* and pass file names as arguments (without the file extensions) as following:

**Example:**

```sh
./assembler test
```

You can also use the **make run** command with arguments:

```sh
make run ARGUMENTS=<input_file>
```

**Example:**

```sh
make run ARGUMENTS=test
```

The assembler will generate output files with the same filenames and the following extensions:  
- `.am` - Macros expansion file
- `.ob` - Object file
- `.ent` - Entries file
- `.ext` - Externals file

An example of input and output files can be found under the 'tests' folder.
## **Cleaning the Project**

To clean the project and remove all generated files, run:

```sh
make clean
```

## Hardware
- CPU
- RAM (including a stack), with the size of 256 *words*.
- A *word*'s size in memory is **12 bits**.
- Uses signed *2's complement* arithmetic for integers (with no support for real numbers).

   ### Registers
   The CPU has 9 registers: 8 general registers (named r0,r1,...,r7) and a register called *PSW* (program status word).  
   The size of each register is a *word* (12 bits).
  
## Instruction Structure
Every software *instruction* is encoded into a few *words* in memory (max is 3 *words*).
The first word is of the following structure:

|         11&emsp;10&emsp;9         |  8&emsp;7&emsp;6&emsp;5  | 4&emsp;3&emsp;2                       |  1&emsp;0  |
| :-------------------------------: | :----------------------: | :-----------------------------------: | :--------: |
|  Source operand addressing method |           Opcode         | Destination operand addressing method |  A, R, E   |


Encoding of each instruction word is done using base64 as defined here:
https://en.wikipedia.org/wiki/Base64#Base64_table_from_RFC_4648

so that each *word* can be encoded as 2 digits in this base.

## Commands
The commands allowed in bits 5-8 are:

| Opcode (decimal) | Command Name |
| ---------------- | ------------ |
|	0	|	mov	|
|	1	|	cmp	|
|	2	|	add	|
|	3	|	sub	|
|	4	|	not	|
|	5	|	clr	|
|	6	|	lea	|
|	7	|	inc	|
|	8	|	dec	|
|	9	|	jmp	|
|	10	|	bne	|
|	11	|	red	|
|	12	|	prn	|
|	13	|	jsr	|
|	14	|	rts	|
|	15	|	stop	|

## Directives
A **_directive_** line of the following structure:

1. An **optional** preceding *label*. e.g. `PLACE1: `.
2. A _directive_: `.data`, `.string`, `.entry` or `.extern`.
3. Operands according to the type of the *directive*.

   ### `.data`
   This directive allocates memory in the data image for storing received integers later in memory (also increases the _data counter_ and updates the _symbol table_).
   The parameters of `.data` are valid integers (separated by a comma).  
   e.g. `LABEL1: .data      +7, -56, 4, 9`.
   
   ### `.string`
   This direcive receives a string as an operand and stores it in the data image. It stores all characters by their order in the string, encoded ny their *ASCII* values.
   e.g. `STRING1: .string "abcdef"` is a valid directive.
   
   ### `.entry`
   This directive outputs a received name of a *label* to the *symbol table*, so that later it will be recognized by other assembly files (and they would be able to use it).
   e.g. 
   ```
   ; file1.as
   .entry HELLO
   HELLO: add #1, r1 
   ```
   ### `.extern`
   This directive receives a name of a *label* as a parameter and declares the *label* as being external (defined in another file) and that the current file shall use it.  
   This way, the directive `.extern HELLO` in `file2.as` will match the `.entry` directive in the previous example.
   
## **Makefile Overview**

The **Makefile** contains the build instructions for the project. Key targets include:

- \`all\` - Builds the project and runs the post-build step.
- \`post-build\` - Copies the executable to the `Tests/` and `Tests/errors/` directories.
- \`run\` - Builds the project and runs the executable with specified arguments.
- \`clean\` - Removes all generated files.

##

