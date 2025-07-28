#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

void secret() {
    printf("\n*** Secret function executed! ***\n");
    exit(42);
}

void safe_function() {
    printf("Normal function executed.\n");
}

void vulnerable_function(const char* input) {
    char buffer[64];

    printf("Copying input (%zu bytes) into buffer[64]...\n", strlen(input));
    strcpy(buffer, input);

    printf("Done copying. Buffer content: %s\n", buffer);
}

int main(int argc, char* argv[]);

void print_addresses() {
    printf("Function addresses:\n");
    printf("  main               : %p\n", (void*)&main);
    printf("  safe_function      : %p\n", (void*)&safe_function);
    printf("  secret             : %p\n", (void*)&secret);
    printf("  vulnerable_function: %p\n", (void*)&vulnerable_function);
    printf("\n");
}

void menu() {
    printf("Stack Exploitation Lab\n");
    printf("======================\n");
    printf("Choose scenario:\n");
    printf("1. Safe function\n");
    printf("2. Vulnerable function (input from stdin)\n");
    printf("3. Vulnerable function (crafted payload)\n");
    printf("\nSelect mode: ");
}

int main(int argc, char* argv[]) {
    print_addresses();

    menu();
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    getchar();

    if (choice == 1) {
        safe_function();
    }
    else if (choice == 2) {
        char input[256];
        printf("Enter your input: ");
        fgets(input, sizeof(input), stdin);

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        vulnerable_function(input);
    }
    else if (choice == 3) {
        const char* crafted = 
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
            "BBBBBBBB"
            "\x12\x34\x56\x78";

        printf("Using crafted payload (%zu bytes)...\n", strlen(crafted));
        vulnerable_function(crafted);
    }
    else {
        printf("Unknown mode.\n");
    }

    printf("Program completed.\n");
    return 0;
}
