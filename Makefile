
########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = gcc #x86_64-elf-gcc #gcc
CXXFLAGS =  -std=c11 -std=c99 -pedantic -g -Wall 
LDFLAGS = 


ARGUMENTS ?=

# Makefile settings - Can be customized.

EXT = .c

BIN_DIR := bin
BIN_EXE := assembler
APPNAME := $(BIN_EXE)
SRC_DIR := src
OBJ_DIR := $(BIN_DIR)/obj
INC_DIRS := -I$(SRC_DIR) -I$(SFML_DIR)/include
DEP_DIR := dep



############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRC_DIR)/*$(EXT))
H_FILES = $(wildcard $(SRC_DIR)/*.h)
OBJ = $(SRC:$(SRC_DIR)/%$(EXT)=$(OBJ_DIR)/%.o)
DEP = $(OBJ:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)

# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRC_DIR)/%$(EXT)=$(OBJ_DIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME) post-build
      
# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Post-build step: Copy executable to Tests and Tests/errors
post-build:
	$(if $(findstring Windows_NT, $(OS)), \
		copy $(APPNAME) Tests\$(APPNAME)$(EXE) & copy $(APPNAME) Tests\errors\$(APPNAME)$(EXE), \
		cp $(APPNAME) Tests/$(APPNAME) && cp $(APPNAME) Tests/errors/$(APPNAME))

# Creates the dependecy rules
$(DEP_DIR)/%.d: $(SRC_DIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(SRC_DIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJ_DIR)/%.o: $(SRC_DIR)/%$(EXT) $(H_FILES)
	$(CC) $(CXXFLAGS) -c -o $@ $<


# if we type "make run" it will build and then run executable 
run: all
	@echo "Running with arguments: $(ARGUMENTS)"
	$(if $(findstring Windows_NT, $(OS)), cd Tests && assembler.exe $(ARGUMENTS) && cd .., cd Tests && ./assembler $(ARGUMENTS) && cd ..)
################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME) Tests/$(APPNAME) Tests/errors/$(APPNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)