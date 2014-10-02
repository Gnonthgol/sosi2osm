#include <stdio.h>
#include <string.h>

#include "sosi2osm.h"

char* execname;

void usage() {
    fprintf(stderr,"Usage: sosi2osm <sosi file> <lua file> [output file]\n");
}

void handleHead() {
    setProjection(getCoordinateSystem());
    
    long lines = getSOSILinesLength();
    for (int i = 0; i < lines; i++) {
        char* line = getSOSILine(i);
        if (line != NULL && strncmp(line, "..TEGNSETT ", 11) == 0) {
            setEncoding(line+11);
        }
    }
}

void outputWay(FILE* output) {
    long int* nd;
    long int size = createNodes(&nd, output);
    
    fprintf(output, "<way id=\"%ld\" version=\"1\" visible=\"true\">", -getSOSIId());
    outputTags(output);
    
    for (int i = 0; i < size; i++) {
        fprintf(output, "<nd ref=\"%ld\" />", nd[i]);
    }
    
    fprintf(output, "</way>\n");
    
    free(nd);
}

void outputRelation(FILE* output) {
    fprintf(output, "<relation id=\"%ld\" version=\"1\" visible=\"true\">", -getSOSIId());
    outputTags(output);
    
    bool outer = true;
    long refsLen = getSOSIRefsSize();
    long* refs = getSOSIRefs();
    for (int i = 0; i < refsLen; i++) {
        if (refs[i] == START_OY)
            outer = false;
        else if (refs[i] == SLUTT_OY)
            outer = true;
        else
            fprintf(output, "<member ref=\"%d\" role=\"%s\" type=\"way\"/>", -abs(refs[i]), outer?"outer":"inner");
    }
    
    free(refs);
    
    fprintf(output, "</relation>\n");
}

int main(int argc, char** args) {
    if (argc < 3 || argc > 4) {
        usage();
        return 1;
    }
    
    execname = args[0];
    char* input_filename = args[1];
    char* output_filename = "-";
    if (argc > 3) output_filename = args[3];
    
    FILE* output;
    if (strcmp(output_filename, "-") == 0)
        output = stdout;
    else
        output = fopen(output_filename, "w");
    if (output == NULL) {
        fprintf(stderr, "Could not open file for output '%s'\n", output_filename);
        return 1;
    }
    
    loadLua(args[2]);
    
    if (!openSOSI(input_filename)) {
        closeSOSI();
        return 1;
    }
    
    fprintf(output, "<?xml version=\"1.0\"?>\n"
        "<osm version=\"0.6\" upload=\"false\" generator=\"sosi2osm\">\n");
    
    while (nextSOSIObject()) {
        short type = getSOSIType();
        switch(type) {
        case L_HODE:
            handleHead();
            break;
        case L_FLATE:
            outputRelation(output);
            break;
        case L_PUNKT:
        case L_SYMBOL:
        case L_TEKST:
            outputNode(output);
            break;
        case L_KURVE:
        case L_LINJE:
        case L_BUEP:
            outputWay(output);
            break;
        }
    }
    
    fprintf(output, "</osm>\n");
    
    closeSOSI();
    fclose(output);
    
    return 0;
}
