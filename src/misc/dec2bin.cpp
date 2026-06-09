#include <dec2bin.h>

int dec2bin(int Dec){
    int bin = 0,i=2 ;
    int binCreator[3] = {0,0,0};
    while(Dec>0){
        binCreator[i] = Dec%2;
        Dec/=2;
        i--;
    }
    for(i=0;i<3;i++){
        bin*=10;
        bin+=binCreator[i];
    }
    return bin;
}