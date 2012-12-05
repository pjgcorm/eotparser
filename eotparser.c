#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

#include "eot/embeddedfont.h"
#include "eot/libeot.h"
#include "mtx/mtx.h"

int
main (int argc, char **argv)
{
    char *inputfile = NULL;
    char *outputfile = NULL;
    int input;
    FILE *output;

    int c;
    struct stat stat;
    eot_t *state;
    mtx_t *mtx;
    size_t rsize, dsize, csize;
    uint8_t *rest, *code; 

    opterr = 0;

    while ((c = getopt (argc, argv, "r:w:")) != -1)
        switch (c) {
            case 'r':
	            inputfile = optarg;
	            break;
            case 'w':
	            outputfile = optarg;
	            break;
            case '?':
	            if (optopt == 'c')
	                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	            else if (isprint (optopt))
	                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	            else
	                fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
	            return 1;
            default:
	                abort ();
        }

    if (inputfile) {
        printf ("[*] Processing file %s\n", inputfile);
        input = open(inputfile, O_RDONLY, 0);
        eot_init(&state, read, input);
        //eot_dump(state);    
       
        if (outputfile)
            output = fopen(outputfile, "w");
        else
            output = fopen("output.ctf", "w");

        printf ("[*] Writing to file %s\n", outputfile);
        
        if (state->head.Flags & TTEMBED_TTCOMPRESSED) {
            printf("[-] File is compressed\n");
            mtx_init(&mtx, state->fontdata, state->head.FontDataSize);
            //mtx_dump(mtx);
            
            mtx_getRest(mtx, &rest, &rsize);
            fwrite(rest, 1, rsize, output);

            mtx_getData(mtx, &state->fontdata, &state->head.FontDataSize);
            fwrite(state->fontdata, 1, state->head.FontDataSize, output);

            mtx_getCode(mtx, &code, &csize);
            fwrite(code, 1, csize, output);

            mtx_fini(mtx);

        } else { // File is not compressed
            fwrite(state->fontdata, 1, state->head.FontDataSize, output);
        }

        fclose(output);
        close(input);
        eot_fini(state);
    }
    else if (inputfile == NULL) {
        fprintf (stderr, "[!] no input file\n");
        return 1; 
    }
    printf ("[*] Done.")
    return 0;
}
