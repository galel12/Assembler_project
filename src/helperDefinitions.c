#include "libraries.h"

const Registers registers[8] = {{"@r0"},{"@r1"},{"@r2"},{"@r3"},
                                {"@r4"},{"@r5"},{"@r6"},{"@r7"}};

const op_Code instruction_opCode[16]={ {"mov", 0},{"cmp",1}, {"add", 2},{"sub", 3},
                              {"not", 4},{"clr",5}, {"lea", 6},{"inc", 7},
                              {"dec", 8},{"jmp",9}, {"bne", 10},{"red", 11},
                              {"prn", 12},{"jsr",13}, {"rts", 14},{"stop", 15}
                            };

const LegalAddressingMethods instruction_legalMethods[16] = {
                              {"mov",{1,3,5},{3,5}},{"cmp", {1,3,5}, {1,3,5}}, {"add", {1,3,5},{3,5}},
                              {"sub", {1,3,5},{3,5}},
                              {"not",{0},{3,5}},{"clr",{0},{3,5}}, 
                              {"lea", {3},{3,5}},
                              {"inc", {0},{3,5}},{"dec", {0},{3,5}},
                              {"jmp",{0},{3,5}}, {"bne", {0},{3,5}},{"red", {0},{3,5}},
                              {"prn", {0},{1,3,5}},{"jsr",{0},{3,5}}, {"rts", {0},{0}},
                              {"stop",{0},{0}}
                              };

const LegalNumOfOperands Legal_Num_Of_Operands[3] = {{{"mov","cmp", "add", "sub", "lea"}, 2},
                                                     {{"not", "clr", "inc", "dec","jmp",  "bne", "red","prn", "jsr"}, 1},
                                                     {{"rts","stop"}, 0}};

const Saved_Words saved_words[28] = {{"@r0"},{"@r1"},{"@r2"},{"@r3"},
                                     {"@r4"},{"@r5"},{"@r6"},{"@r7"},
                                     {"mov"},{"cmp"}, {"add"},{"sub"},
                                     {"not"},{"clr"}, {"lea"},{"inc"},
                                     {"dec"},{"jmp"}, {"bne"},{"red"},
                                     {"prn"},{"jsr"}, {"rts"},{"stop"},
                                     {".data"},{".string"},{".entry"},{".extern"}};


int num_of_op;

int Line_counter;

int error;





