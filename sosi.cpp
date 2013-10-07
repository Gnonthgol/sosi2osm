#include <string.h>

#include "sosi2osm.h"

LC_BASEADM* base = NULL;
LC_FILADM* fileadm = NULL;

int openSOSI(const char* input_filename) {
    LC_Init();
    
    // Is this a valid file
    UT_INT64 end = 0;
    if (HO_TestSOSI(input_filename, &end) == UT_FALSE) {
        fprintf(stderr, "Not a SOSI file: %s\n", input_filename);
        return 0;
    }
    
    // Open the file for reading
    LC_BASEADM* base = LC_OpenBase(LC_BASE);
    LC_FILADM* fileadm;
    short status = 0;
    LC_OpenSos(input_filename, LC_BASE_FRAMGR, LC_GML_IDX, LC_VIS_STATUS, &fileadm, &status);
    if (status != 0) {
        fprintf(stderr, "Could not open SOSI file: %s (%d)\n", input_filename, status);
        return 0;
    }
    
    return 1;
}

void closeSOSI() {
    if (fileadm) LC_CloseSos(fileadm, RESET_IDX);
    if (base) LC_CloseBase(base, RESET_IDX);
    LC_Close();
}

LC_BGR group = {NULL, 0};

int nextSOSIObject() {
    if (group.pFil == NULL) {
        LC_InitNextBgr(&group);
    }
    
    if (LC_NextBgr(&group, LC_FRAMGR | LC_BAKGR)) {
        short lines;
        long coordinates;
        unsigned short info;
        LC_RxGr(&group, LES_OPTIMALT, &lines, &coordinates, &info);
        return 1;
    }
    
    return 0;
}

short getSOSIType() {
    short lines;
    long coordinates;
    unsigned short info;
    return LC_GetGrPara(&lines, &coordinates, &info);
}

long getSOSIId() {
	return LC_GetSn();
}

const char* getCoordinateSystem() {
	unsigned short mask = LC_TR_ALLT;
    LC_TRANSPAR trans;
	if (LC_GetTransEx(&mask,&trans) == UT_FALSE) {
        fprintf(stderr, "TRANSPAR section not found - No reference system information available.");
        return "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs ";
    }
    
    switch (trans.sKoordsys) {
        /* NGO 1984 Axis I-VIII */
        case 1: return "+proj=tmerc +lat_0=58 +lon_0=-4.666666666666667 +k=1 +x_0=0 +y_0=0 +a=6377492.018 +b=6356173.508712696 +towgs84=278.3,93,474.5,7.889,0.05,-6.61,6.21 +pm=oslo +units=m +no_defs ";
        case 2: return "+proj=tmerc +lat_0=58 +lon_0=-2.333333333333333 +k=1 +x_0=0 +y_0=0 +a=6377492.018 +b=6356173.508712696 +towgs84=278.3,93,474.5,7.889,0.05,-6.61,6.21 +pm=oslo +units=m +no_defs ";
        case 3: return "+proj=tmerc +lat_0=58 +lon_0=0 +k=1 +x_0=0 +y_0=0 +a=6377492.018 +b=6356173.508712696 +towgs84=278.3,93,474.5,7.889,0.05,-6.61,6.21 +pm=oslo +units=m +no_defs ";
        case 4: return "+proj=tmerc +lat_0=58 +lon_0=2.5 +k=1 +x_0=0 +y_0=0 +a=6377492.018 +b=6356173.508712696 +towgs84=278.3,93,474.5,7.889,0.05,-6.61,6.21 +pm=oslo +units=m +no_defs ";
        case 5: return "+proj=tmerc +lat_0=58 +lon_0=6.166666666666667 +k=1 +x_0=0 +y_0=0 +a=6377492.018 +b=6356173.508712696 +towgs84=278.3,93,474.5,7.889,0.05,-6.61,6.21 +pm=oslo +units=m +no_defs ";
        case 6: return "+proj=tmerc +lat_0=58 +lon_0=10.16666666666667 +k=1 +x_0=0 +y_0=0 +a=6377492.018 +b=6356173.508712696 +towgs84=278.3,93,474.5,7.889,0.05,-6.61,6.21 +pm=oslo +units=m +no_defs ";
        case 7: return "+proj=tmerc +lat_0=58 +lon_0=14.16666666666667 +k=1 +x_0=0 +y_0=0 +a=6377492.018 +b=6356173.508712696 +towgs84=278.3,93,474.5,7.889,0.05,-6.61,6.21 +pm=oslo +units=m +no_defs ";
        case 8: return "+proj=tmerc +lat_0=58 +lon_0=18.33333333333333 +k=1 +x_0=0 +y_0=0 +a=6377492.018 +b=6356173.508712696 +towgs84=278.3,93,474.5,7.889,0.05,-6.61,6.21 +pm=oslo +units=m +no_defs ";
        /* UTM ZONE 31-36 */
        case 21: return "+proj=utm +zone=31 +ellps=GRS80 +units=m +no_defs ";
        case 22: return "+proj=utm +zone=32 +ellps=GRS80 +units=m +no_defs ";
        case 23: return "+proj=utm +zone=33 +ellps=GRS80 +units=m +no_defs ";
        case 24: return "+proj=utm +zone=34 +ellps=GRS80 +units=m +no_defs ";
        case 25: return "+proj=utm +zone=35 +ellps=GRS80 +units=m +no_defs ";
        case 26: return "+proj=utm +zone=36 +ellps=GRS80 +units=m +no_defs ";
        /* UTM ZONE 31-36 / ED50 */
        case 31: return "+proj=utm +zone=31 +ellps=intl +units=m +no_defs ";
        case 32: return "+proj=utm +zone=32 +ellps=intl +units=m +no_defs ";
        case 33: return "+proj=utm +zone=33 +ellps=intl +units=m +no_defs ";
        case 34: return "+proj=utm +zone=34 +ellps=intl +units=m +no_defs ";
        case 35: return "+proj=utm +zone=35 +ellps=intl +units=m +no_defs ";
        case 36: return "+proj=utm +zone=36 +ellps=intl +units=m +no_defs ";
        /* WSG84 */
        case 84: return "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs ";
        default: {
            fprintf(stderr, "(Yet) unsupported coodinate system in SOSI-file: %i. Defaulting to EPSG:4326.", trans.sKoordsys);
            return "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs ";
        }
    }
}

void getSOSICoord(long i, double* x, double* y) {
    LC_GetTK(i+1, x, y);
}

long getSOSICoordsSize() {
    short lines;
    long coordinates;
    unsigned short info;
    LC_GetGrPara(&lines, &coordinates, &info);
    return coordinates;
}

char* getSOSILine(long i) {
    if (i == 0) return NULL;
    char* r = LC_GetGi(i+1);
    if (r[0] == ':' || r[0] == '(' || r[0] == '!')
        return NULL;
    while (r[0] == '.') r++;
    if (strncmp(r, "REF", 3) == 0)
        return NULL;
    
    return r;
}

long getSOSILinesLength() {
    short lines;
    long coordinates;
    unsigned short info;
    LC_GetGrPara(&lines, &coordinates, &info);
    return lines;
}

long* getSOSIRefs() {
    long size = LC_InqAntRef();
    long* ret = (long*)malloc(sizeof(long) * size);
    short line = 1, pos = 1;
    
    LC_GetRef(ret, size, &line, &pos);
    
    return ret;
}

long getSOSIRefsSize() {
    return LC_InqAntRef();
}

