#include <stdio.h>
#include <string.h>

#include "sosi2osm.h"

void usage() {
    printf("Usage: sosi2osm [sosi file] [lua file]\n");
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

void outputWay() {
    long int* nd;
    long int size = createNodes(&nd);
    
    printf("<way id=\"%ld\" version=\"1\" visible=\"true\">", -getSOSIId());
    outputTags();
    
    for (int i = 0; i < size; i++) {
        printf("<nd ref=\"%ld\" />", nd[i]);
    }
    
    printf("</way>\n");
    
    free(nd);
}

void outputRelation() {
    printf("<relation id=\"%ld\" version=\"1\" visible=\"true\">", -getSOSIId());
    outputTags();
    
    char* role = "outer";
    long refsLen = getSOSIRefsSize();
    long* refs = getSOSIRefs();
    for (int i = 0; i < refsLen; i++) {
        if (refs[i] == START_OY)
            role = "inner";
        else if (refs[i] == SLUTT_OY)
            role = "outer";
        else
            printf("<member ref=\"%d\" role=\"%s\" type=\"way\"/>", -abs(refs[i]), role);
    }
    
    free(refs);
    
    printf("</relation>\n");
}

int main(int argc, char** args) {
    if (argc != 3) {
        usage();
        return 1;
    }
    
    char* input_filename = args[1];
    
    loadLua(args[2]);
    
    if (!openSOSI(input_filename)) {
        closeSOSI();
        return 1;
    }
    
    printf("<?xml version=\"1.0\"?>\n"
        "<osm version=\"0.6\" upload=\"false\" generator=\"sosi2osm\">\n");
    
    while (nextSOSIObject()) {
        short type = getSOSIType();
        switch(type) {
        case L_HODE:
            handleHead();
            break;
        case L_FLATE:
            outputRelation();
            break;
        case L_PUNKT:
        case L_SYMBOL:
        case L_TEKST:
            outputNode();
            break;
        case L_KURVE:
        case L_LINJE:
        case L_BUEP:
            outputWay();
            break;
        }
    }
    
    printf("</osm>\n");
    
    closeSOSI();
    
    return 0;
}
