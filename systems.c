#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

/* Explanation:


 #pragma omp parallel defines a parallel region
 parallel region is a block of code that runs in parallel among multiple threads

#pragma omp critical 
a block of code where resources are shared by multiple threads. in this case multiple threads writing to one file

compile using gcc -fopenmp -o systems systems.c
run from terminal as " ./systems {}"
*/
void generate_sequences(int length, FILE *file) {
    char bases[4] = "ACGT";
    int total_sequences = 1;
    for (int i = 0; i < length; i++) {
        total_sequences *= 4; // 4**length total sequences
    }


#pragma omp parallel
{
        // printf("%d", omp_get_thread_num());
        char *sequence = (char *)malloc((length + 1) * sizeof(char));
        sequence[length] = '\0'; // for fprintf to file
        #pragma omp for 
        for (int i = 0; i < total_sequences; i++) {
            int index = i;
            for (int j = 0; j < length; j++) {
                sequence[length - j - 1] = bases[index % 4];
                index /= 4;
            }
            #pragma omp critical
            {

                fprintf(file, "%s\n", sequence);
            }
        }
        free(sequence);
    }
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
       printf("Usage: %s Max Length of Sequence\n",argv[0]);
       return 1;
    }

    int max_length = atoi(argv[1]);

    FILE *file = fopen("dna_sequences.txt", "w");

    clock_t start_time = clock();

for (int length = 1; length <= max_length; length++) {
        generate_sequences(length, file);
}


    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    fclose(file);
    printf("All DNA sequences up to length %d saved to dna_sequences.txt\n", max_length);
    printf("Time taken: %.2f seconds\n", elapsed_time);
    return 0;
}
