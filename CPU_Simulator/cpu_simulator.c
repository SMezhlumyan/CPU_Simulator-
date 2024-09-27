#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 256
#define NUM_REGISTERS 6
#define BUFFER_SIZE 100

typedef enum {
    ADD,
    SUB,
    MOV,
    LOAD,
    STORE,
    START,
    EXIT,
    DISC,
    LAYO,
    INVALID
} InstructionType;

typedef struct {
    int registers[NUM_REGISTERS];
    char memory[MEMORY_SIZE];
    int WSR;
    int IP;
} CPU;

typedef struct {
    InstructionType type;
    int rdest;
    int rsrc1;
    int rsrc2;
    int value;
    int address;
    int n;
} Instruction;

void myError(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void ClearCPU(CPU* cpu) {
    memset(cpu->memory, 0, MEMORY_SIZE);
    memset(cpu->registers, 0, sizeof(cpu->registers));
    cpu->WSR = 0;
    cpu->IP = 0;
}

void Store_in_file(CPU* cpu) {
    FILE* file = fopen("store.bin", "ab+");
    if (file == NULL) {
        myError("fopen");
    }

    size_t written = fwrite(cpu, sizeof(CPU), 1, file);
    if (written != 1) {
        myError("fwrite");
    }

    fclose(file);
}

void Read_from_file(int n, CPU* cpu) {
    FILE* file = fopen("store.bin", "rb");
    if (file == NULL) {
        myError("fopen");
    }

    fseek(file, n * sizeof(CPU), SEEK_SET);
    size_t read = fread(cpu, sizeof(CPU), 1, file);
    if (read != 1) {
        myError("fread");
    }

    fclose(file);
}

void PrintCPUState(const CPU* cpu) {
    printf("CPU State:\n");
    printf("WSR: %d\n", cpu->WSR);
    printf("IP: %d\n", cpu->IP);
    printf("Registers:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("R%d: %d ", i, cpu->registers[i]);
    }
    printf("\nMemory[");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%d", (unsigned char)cpu->memory[i]);
        if (i < MEMORY_SIZE - 1) {
            printf(", ");
        }
    }
    printf("] size(%d)\n", MEMORY_SIZE);
}

void ClearSpaces(char* str) {
    char* end;

    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == 0) {
        *str = '\0';
        return;
    }
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *str == '\t'))
        end--;

    *(end + 1) = '\0';
}

InstructionType get_instruction_type(const char* instr) {
    if (strcmp(instr, "ADD") == 0)
        return ADD;
    if (strcmp(instr, "SUB") == 0)
        return SUB;
    if (strcmp(instr, "MOV") == 0)
        return MOV;
    if (strcmp(instr, "LOAD") == 0)
        return LOAD;
    if (strcmp(instr, "STORE") == 0)
        return STORE;
    if (strcmp(instr, "START") == 0)
        return START;
    if (strcmp(instr, "EXIT") == 0)
        return EXIT;
    if (strcmp(instr, "DISC") == 0)
        return DISC;
    if (strcmp(instr, "LAYO") == 0)
        return LAYO;
    return INVALID;
}

void execute_instruction(CPU* cpu, Instruction* instr) {
    switch (instr->type) {
    case ADD:
        cpu->IP++;
        cpu->registers[instr->rdest] =
            cpu->registers[instr->rsrc1] + cpu->registers[instr->rsrc2];
        Store_in_file(cpu);
        break;
    case SUB:
        cpu->IP++;
        cpu->registers[instr->rdest] =
            cpu->registers[instr->rsrc1] - cpu->registers[instr->rsrc2];
        Store_in_file(cpu);
        break;
    case MOV:
        cpu->IP++;
        cpu->registers[instr->rdest] = instr->value;
        Store_in_file(cpu);
        break;
    case STORE:
        cpu->IP++;
        cpu->memory[instr->address] = cpu->registers[instr->rsrc1];
        Store_in_file(cpu);
        break;
    case LOAD:
        cpu->IP++;
        cpu->registers[instr->rdest] = cpu->memory[instr->address];
        Store_in_file(cpu);
        break;
    case LAYO:
        PrintCPUState(cpu);
        break;
    case DISC:
        Read_from_file(cpu->IP - instr->n, cpu);
        break;

    default:
        return;
    }
}

void STARTPOINT(CPU* cpu) {
    char input[BUFFER_SIZE];

    while (1) {
        printf("Enter instruction : ");
        if (fgets(input, BUFFER_SIZE, stdin) != NULL) {
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }
            ClearSpaces(input);

            char* token = strtok(input, " ");
            if (token == NULL) {
                printf("Invalid command.\n");
                continue;
            }

            char* operation = token;
            Instruction inst;
            inst.type = get_instruction_type(operation);

            char* delim = " ,";
            switch (inst.type) {
            case MOV:
                token = strtok(NULL, delim);
                if (token != NULL) {
                    int reg_number = atoi(token + 1);
                    inst.rdest = reg_number;
                }
                token = strtok(NULL, delim);
                if (token != NULL) {
                    inst.value = atoi(token);
                }
                break;
            case EXIT:
                cpu->WSR = 0;
                printf("CPU->WSR: %d\n", cpu->WSR);
                PrintCPUState(cpu);
                return;
                break;
            case SUB:
            case ADD:
                token = strtok(NULL, delim);
                if (token != NULL) {
                    int reg_number = atoi(token + 1);
                    inst.rdest = reg_number;
                }
                token = strtok(NULL, delim);
                if (token != NULL) {
                    inst.rsrc1 = atoi(token + 1);
                }
                token = strtok(NULL, delim);
                if (token != NULL) {
                    inst.rsrc2 = atoi(token + 1);
                }
                break;
            case STORE:
                token = strtok(NULL, delim);
                if (token != NULL) {
                    int reg_number = atoi(token + 1);
                    inst.rsrc1 = reg_number;
                }
                token = strtok(NULL, delim);
                if (token != NULL) {
                    inst.address = atoi(token);
                }
                break;
            case LOAD:
                token = strtok(NULL, delim);
                if (token != NULL) {
                    int reg_number = atoi(token + 1);
                    inst.rdest = reg_number;
                }
                token = strtok(NULL, delim);
                if (token != NULL) {
                    inst.address = atoi(token);
                }
                break;
            case LAYO:
                break;
            case DISC:
                token = strtok(NULL, delim);
                inst.n = atoi(token);
                break;

            case INVALID:
                printf("INVALID\n");
                return;
            default:
                return;
            }

            execute_instruction(cpu, &inst);

        } else {
            perror("Error reading input");
            break;
        }
    }
}

int main() {
    CPU* cpu = (CPU*)malloc(sizeof(CPU));

    if (cpu == NULL) {
        myError("CPU");
    }

    ClearCPU(cpu);
    char input[BUFFER_SIZE];

    while (1) {
        printf("Enter command (START/EXIT): ");
        if (fgets(input, BUFFER_SIZE, stdin) != NULL) {
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }
            ClearSpaces(input);
            if (strcmp(input, "START") == 0) {
                cpu->WSR = 1;
                printf("CPU->WSR: %d\n", cpu->WSR);
                STARTPOINT(cpu);
            } else if (strcmp(input, "EXIT") == 0) {
                ClearCPU(cpu);
                break;
            } else {
                printf("Command not recognized.\n");
            }
        } else {
            perror("Error reading input");
            break;
        }
    }

    free(cpu);
    return 0;
}
