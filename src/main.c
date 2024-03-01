#include <stdio.h>
#include <stdlib.h>
#include "examples.h"

#define EXAMPLE_COUNT 5
#define FLAG_COUNT 3

typedef int (*example_func)(void);
struct example_entry {
	const char* name;
	example_func func;
	unsigned long long int hash;
};

struct example_entry examples[EXAMPLE_COUNT] = {
	{ .func = example_current, .name = "current" },
	{ .func = example_triangle, .name = "triangle" },
	{ .func = example_uniforms, .name = "uniforms" },
	{ .func = example_texture, .name = "texture" },
	{ .func = example_hlsl, .name = "hlsl" },
};

typedef void (*flag_func)(void);
struct flag_entry {
	const char* name;
	const char* desc;
	flag_func func;
	unsigned long long int hash;
};

void flag_help(void);
void flag_usage(void);
void flag_list(void);
struct flag_entry flags[FLAG_COUNT] = {
	{ .func = flag_help, .name = "help", .desc = "Lists all flags and descriptions" },
	{ .func = flag_usage, .name = "usage", .desc = "Provides usage message for this program" },
	{ .func = flag_list, .name = "list", .desc = "Lists all example names" },
};

unsigned long long int djbhash(const char* string) {
	unsigned long long int hash = 5381;
	int c;
	while ((c = *string++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

int isnumber(const char* string) {
	for (const char* c = string; *c; ++c) {
		if (*c < '0' || *c > '9') {
			return 0;
		}
	}
	return 1;
}

const char* program_name;
const char* example_arg;

int main(int argc, char** argv) {
	program_name = argv[0];

	if (argc < 2) {
		printf("Usage:\n    %s {flags} \"[example name]\"\n  or\n    %s {flags} [example number]\n", program_name, program_name);
		printf("Flags are stored in 1 argument only \"-flag1,flag2,flag3\"\n");
		return 1;
	}

	example_arg = argv[1];

	for (unsigned int i = 0; i < FLAG_COUNT; ++i) {
		flags[i].hash = djbhash(flags[i].name);
	}

	unsigned int flag_index = argc;
	for (int i = 0; i < argc; ++i) {
		if (argv[i][0] == '-') {
			size_t size = 0;
			char* buffer = malloc(strlen(argv[i]));
			if (buffer == NULL) {
				printf("Failed to allocate memory for flags\n");
				return 1;
			}

			unsigned int last = 1;
			for (size_t j = 1; j < strlen(argv[i]) + 1; ++j) {
				if (argv[i][j] == ',' || argv[i][j] == '\0') {
					size = j - 1;
					memcpy(buffer, argv[i] + last, size);
					buffer[size - last + 1] = '\0';
					last = j + 1;
					unsigned long long int hash = djbhash(buffer);
					for (unsigned int k = 0; k < FLAG_COUNT; ++k) {
						if (flags[k].hash == hash) {
							flags[k].func();
							hash = 0;
							break;
						}
					}

					if (hash != 0) {
						printf("Invalid flag %s\n", buffer);
						return 1;
					}
				}
			}

			flag_index = i;
			break;
		}
	}

	if (flag_index == 1 && argc < 3) {
		printf("No example specified\n");
		return 1;
	}

	if (flag_index == 1) {
		example_arg = argv[2];
	}

	#ifdef KGFX_MACOS
	if (argc >= 3) {
		chdir(argv[2]);
	}
	#endif

	for (unsigned int i = 0; i < EXAMPLE_COUNT; ++i) {
		examples[i].hash = djbhash(examples[i].name);
	}

	unsigned int example_index;
	if (isnumber(example_arg)) {
		example_index = strtoul(example_arg, NULL, 10);
		if (example_index >= EXAMPLE_COUNT) {
			printf("Invalid example number %s\n", example_arg);
			return 1;
		}
	} else {
		unsigned long long int hash = djbhash(example_arg);
		example_index = EXAMPLE_COUNT;
		for (unsigned int i = 0; i < EXAMPLE_COUNT; ++i) {
			if (examples[i].hash == hash) {
				example_index = i;
				break;
			}
		}
		if (example_index == EXAMPLE_COUNT) {
			printf("Invalid example name %s\n", example_arg);
			return 1;
		}
	}

	int res = examples[example_index].func();
	if (res != 0) {
		printf("Example %s failed with code %d\n", examples[example_index].name, res);
		return res;
	}
}

void flag_help(void) {
	printf("Flags:\n");
	for (unsigned int i = 0; i < FLAG_COUNT; ++i) {
		printf("    -%s: %s\n", flags[i].name, flags[i].desc);
	}
}

void flag_usage(void) {
	printf("Usage:\n    %s {flags} \"[example name]\"\n  or\n    %s {flags} [example number]\n", program_name, program_name);
	printf("Flags are stored in 1 argument only \"-flag1,flag2,flag3\"\n");
}

void flag_list(void) {
	printf("Examples:\n");
	for (unsigned int i = 0; i < EXAMPLE_COUNT; ++i) {
		printf("    [%u]: %s\n", i, examples[i].name);
	}
}