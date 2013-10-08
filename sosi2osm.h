#include <fyba.h>

// SOSI

int openSOSI(const char* input_filename);
void closeSOSI();
int nextSOSIObject();
short getSOSIType();
long getSOSIId();
const char* getCoordinateSystem();
void getSOSICoord(long i, double* x, double* y);
long getSOSICoordsSize();
char* getSOSILine(long i);
long getSOSILinesLength();
long* getSOSIRefs();
long getSOSIRefsSize();

// Tag

void setEncoding(char* encoding);
void loadLua(char* filename);
void outputTags();

// Node

void setProjection(const char* proj);
long int createNodes(long int** ids);
void outputNode();

