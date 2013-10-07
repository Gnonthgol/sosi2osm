#include <stdio.h>
#include <string.h>
#include <proj_api.h>

#include "sosi2osm.h"

void usage() {
    printf("Usage: sosi2osm [sosi file]\n");
}

projPJ origProj, osmProj;
void handleHead() {
    if (!(origProj = pj_init_plus(getCoordinateSystem())) )
        exit(1);
    if (!(osmProj = pj_init_plus("+proj=latlong +datum=WGS84")) )
        exit(1);
}

void getCoords(long int* size, double** lat, double** lon) {
    *size = getSOSICoordsSize();
    double* x = (double*)malloc(sizeof(double) * *size);
    double* y = (double*)malloc(sizeof(double) * *size);
    *lat = y;
    *lon = x;
    
    int i;
    for (i = 0; i < *size; i++) {
        getSOSICoord(i, x+i, y+i);
    }
    
    pj_transform(origProj, osmProj, *size, 1, x, y, NULL );
    
    for (i = 0; i < *size; i++) {
        x[i] = x[i]*RAD_TO_DEG;
        y[i] = y[i]*RAD_TO_DEG;
    }
}

long nodeId = -1;
long createNode(double lat, double lon) {
    printf("<node id=\"%ld\" lat=\"%.7f\" lon=\"%.7f\" visible=\"true\"/>\n", nodeId, lat, lon);
    return nodeId--;
}

void outputTags() {
    long lines = getSOSILinesLength();
    for (int i = 0; i < lines; i++) {
        char* key = getSOSILine(i);
        if (key != NULL) {
            char* value = strchr(key, ' ');
            if (value != NULL) {
                value[0] = '\0';
                value++;
                while (value[0] == '"') value++;
                char* last = value + strlen(value);
                while (last[-1] == '"') last--;
                *last = '\0';
                
                printf("<tag k=\"%s\" v=\"%s\"/>\n", key, value);
            }
        }
    }
}

void outputNode() {
    long int size;
    double *lat, *lon;
    getCoords(&size, &lat, &lon);
    
    for (int i = 0; i < size; i++) {
        printf("<node id=\"%ld\" lat=\"%.7f\" lon=\"%.7f\" visible=\"true\">\n", nodeId--, lat[i], lon[i]);
        outputTags();
        printf("</node>\n");
    }
    
    free(lat);
    free(lon);
}

void outputWay() {
    long int size;
    double *lat, *lon;
    getCoords(&size, &lat, &lon);
    long* nd = (long*)malloc(sizeof(long) * size);
    
    for (int i = 0; i < size; i++) {
        nd[i] = createNode(lat[i], lon[i]);
    }
    
    printf("<way id=\"%ld\" visible=\"true\">", -getSOSIId());
    outputTags();
    
    for (int i = 0; i < size; i++) {
        printf("<nd ref=\"%ld\" />", nd[i]);
    }
    
    printf("</way>\n");
    
    free(lat);
    free(lon);
}

void outputRelation() {

}

int main(int argc, char** args) {
    if (argc != 2) {
        usage();
        return 1;
    }
    
    char* input_filename = args[1];
    
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
