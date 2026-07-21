#ifndef FUZZER_H
#define FUZZER_H

typedef struct
{
    unsigned int seed;
    int current_op;
    int max_ops;
    char** op_log;
    int log_capacity;
} FuzzerState;

void fuzzer_init(FuzzerState* fuzzer, unsigned int seed, int max_ops);
void fuzzer_free(FuzzerState* fuzzer);
void fuzzer_log_op(FuzzerState* fuzzer, const char* format, ...);
void fuzzer_dump_log(const FuzzerState* fuzzer);
int fuzzer_rand_int(FuzzerState* fuzzer, int min, int max);

#endif // FUZZER_H
