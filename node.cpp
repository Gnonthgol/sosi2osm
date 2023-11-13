#include "sosi2osm.h"

#include <algorithm>
#include <proj.h>

PJ *P;
void setProjection(const char* proj) {
    if (!(P = proj_create_crs_to_crs(PJ_DEFAULT_CTX, proj, "+proj=latlong +datum=WGS84", NULL))) 
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

    PJ_COORD p, p_out;
    p.lpzt.z = 0.0;
    p.lpzt.t = 1.7976931348623158e+308;
    for (i = 0; i < *size; i++) {
        p.lpzt.lam = x[i];
        p.lpzt.phi = y[i];
        p_out = proj_trans(P, PJ_FWD, p);
        x[i] = p_out.xy.x;
        y[i] = p_out.xy.y;
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
    	sizeM = std::max(1024, sizeM*2);
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

