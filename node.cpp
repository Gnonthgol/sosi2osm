#include "sosi2osm.h"

#include <proj_api.h>

projPJ origProj, osmProj;
void setProjection(const char* proj) {
    if (!(origProj = pj_init_plus(proj)) )
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
long createNode(double lat, double lon, short kp, FILE* output) {
	if (kp == 0) {
    	fprintf(output, "<node id=\"%ld\" lat=\"%.7f\" lon=\"%.7f\" version=\"1\" visible=\"true\"/>\n", nodeId, lat, lon);
	    return nodeId--;
    }
    
    static int sizeM = 0;
    static int lenM = 0;
    static double* latM = NULL;
    static double* lonM = NULL;
    static short* kpM = NULL;
    static long* idM = NULL;
    
    for (int i = 0; i < lenM; i++) {
    	if (lat == latM[i] && lon == lonM[i] && kp == kpM[i]) {
    		return idM[i];
    	}
    }
    
    if (lenM >= sizeM) {
    	sizeM = max(1024, sizeM*2);
    	latM = (double*)realloc(latM, sizeof(double) * sizeM);
    	lonM = (double*)realloc(lonM, sizeof(double) * sizeM);
    	kpM = (short*)realloc(kpM, sizeof(short) * sizeM);
    	idM = (long*)realloc(idM, sizeof(long) * sizeM);
    }
    
    latM[lenM] = lat;
    lonM[lenM] = lon;
    kpM[lenM] = kp;
    idM[lenM] = nodeId;
    lenM++;
    fprintf(output, "<node id=\"%ld\" lat=\"%.7f\" lon=\"%.7f\" version=\"1\" visible=\"true\"/>\n", nodeId, lat, lon);
    return nodeId--;
}

long int createNodes(long int** ids, FILE* output) {
	long int size;
    double *lat, *lon;
    getCoords(&size, &lat, &lon);
    long int *nd = (long*)malloc(sizeof(long) * size);
    *ids = nd;
    
    for (int i = 0; i < size; i++) {
        nd[i] = createNode(lat[i], lon[i], LC_GetKp(i+1), output);
    }
    
    free(lat);
    free(lon);
    return size;
}

void outputNode(FILE* output) {
    long int size;
    double *lat, *lon;
    getCoords(&size, &lat, &lon);
    
    for (int i = 0; i < size; i++) {
        fprintf(output, "<node id=\"%ld\" lat=\"%.7f\" lon=\"%.7f\" version=\"1\" visible=\"true\">\n", nodeId--, lat[i], lon[i]);
        outputTags(output);
        fprintf(output, "</node>\n");
    }
    
    free(lat);
    free(lon);
}

