# Assembler Project

This project is an **assembler** written in **C language**. It translates assembly language code into machine code. The assembler reads an input file containing assembly instructions and generates an output file with the corresponding machine code.

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

**Example:**

```sh
./assembler test.as
```

You can also use the **make run** command with arguments:

```sh
make run ARGUMENTS=<input_file>
```

**Example:**

```sh
make run ARGUMENTS=test.as
```

## **Cleaning the Project**

To clean the project and remove all generated files, run:

```sh
make clean
```

## **Makefile Overview**

The **Makefile** contains the build instructions for the project. Key targets include:

- \`all\` - Builds the project and runs the post-build step.
- \`post-build\` - Copies the executable to the `Tests/` and `Tests/errors/` directories.
- \`run\` - Builds the project and runs the executable with specified arguments.
- \`clean\` - Removes all generated files.

##

