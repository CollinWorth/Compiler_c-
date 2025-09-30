#ifndef SCANTYPE_H
#define SCANTYPE_H
struct TokenData {
    int linenum;
    char* tokenstr;
    double numValue;
    char charValue;
    char* stringValue;
    int strLen;
    int idIndex;
    char symValue;
    int boolValue;
    char* warning;
};

#endif
