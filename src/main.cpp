#include "qr.h"

int main(){
    uint8_t l;
    const char* in = "12354232151jb51lkjgcgklgzGKLB=Bv  o8T8VI=/VR)V trbv9 /R)767rV B)B 6";
    QrDataSegment *s = QrEncoder.optimizeSegments(in, strlen(in), 30, l );
    
}