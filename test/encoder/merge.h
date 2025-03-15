#include "../common.h"

static QrDataSegment segs[] = {
    QrDataSegment(0, 8, 1), 
    QrDataSegment(8,12,0), 
    QrDataSegment(12,16,1),
    QrDataSegment (16,18,0), 
    QrDataSegment(18,24,2)
};
static const QrDataSegment seg2to1[] = {
    {0, 8, 0}, {8,12,1}
};

void printSegs(QrDataSegment *s, uint8_t sl, uint8_t v){
    printf("\n%d", sl);
    for (uint8_t i = 0; i < sl; i++){
        QrMode m = s[i].mode;
        printf("\n(%3d-%3d): %5s (%3d)", s[i].from, s[i].to, m == 0 ? "Num" : m == 1 ? "Alpha" : "Byte", QrEncoder.getSegmentWordCount(s + i, v));
    }
    printf("\n");
}

void encoder_mergeSegments() {
    QrDataSegment *s = segs;
    uint8_t sl = 5,
        v = 20;
    printSegs(s, sl, v);
    QrEncoder.mergeSegments(s, sl, v, sl);
    printSegs(s, sl, v);
}

