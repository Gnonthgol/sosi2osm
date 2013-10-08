#include "sosi2osm.h"

#include <string.h>
#include <iconv.h>

iconv_t charDescriptor;
void setEncoding(char* encoding) {
    charDescriptor = iconv_open("UTF-8", encoding);
}

char* toUTF8(char* in, char* outBuf, size_t outlen) {
    size_t inlen = strlen(in)+1;
    char* out = outBuf;
    
    iconv(charDescriptor, NULL, NULL, NULL, NULL);
    int r = iconv(charDescriptor, &in, &inlen, &out, &outlen);
    
    if (r == -1) {
        fprintf(stderr, "Unknown character '%c' (0x%hhx)\n", in[0], in[0]);
        exit(1);
    }
    
    return outBuf;
}

void outputTags() {
    long lines = getSOSILinesLength();
    for (int i = 1; i < lines; i++) {
        char* key = getSOSILine(i);
        if (key != NULL) {
            while (key[0] == '.') key++;
            
            char* value = strchr(key, ' ');
            if (value != NULL) {
                value[0] = '\0';
                value++;
                while (value[0] == '"') value++;
                char* last = value + strlen(value);
                while (last[-1] == '"') last--;
                *last = '\0';
                
                char keyBuf[256];
                char valueBuf[256];
                
                printf("<tag k=\"%s\" v=\"%s\"/>\n",
                    toUTF8(key, keyBuf, 256),
                    toUTF8(value, valueBuf, 256));
            }
        }
    }
}
