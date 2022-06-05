#include "histogramWid.h"

void HistogramW::paintEvent(QPaintEvent *event){

    Q_UNUSED(event);
    int red = 0, blue = 0, green = 0;
    int rCount[256], bCount[256], gCount[256], brightCount[256],
            maxG = 0, maxR = 0, maxB = 0, maxBright = 0;
    int w, h;

    memset(rCount, 0, sizeof(rCount));
    memset(bCount, 0, sizeof(bCount));
    memset(gCount, 0, sizeof(gCount));
    memset(brightCount, 0, sizeof(brightCount));

    uchar *bite = this->image->bits();
    for (int a = 0; a < image->width() * image->height(); a++)
    {
        red = *bite;
        green = *++bite;
        blue = *++bite;
        bite+=2;
        rCount[red]++;
        bCount[blue]++;
        gCount[green]++;
        brightCount[(int)(red*0.2126 + blue*0.0722 + green*0.7152)]++;
    }

    for(int i = 0; i < 256; i++){
        if (rCount[i] > maxR) maxR = rCount[i];
        if (gCount[i] > maxG) maxG = gCount[i];
        if (bCount[i] > maxB) maxB = bCount[i];
        if (brightCount[i] > maxBright) maxBright = brightCount[i];
    }

    QPainter *painter = new QPainter();
    painter->begin(picture);
    painter->drawLine(0, 250, 256, 250);
    painter->drawLine(0, 250, 0, 50);
    painter->drawText(253, 260, "255");
    painter->drawText(1, 260, "0");
    painter->setPen(QPen(Qt::red));
    w = 1; h = 250;
    for(int i = 0; i < 256; i++, w++){
        painter->drawLine(w, h, w, h - (double)rCount[i] / (double)maxR * 200);
    }

    painter->setPen(QPen(Qt::black));
    painter->drawLine(285, 250, 540, 250);
    painter->drawLine(285, 250, 285, 50);
    painter->drawText(528, 260, "255");
    painter->drawText(286, 260, "0");
    painter->setPen(QPen(Qt::blue));
    w = 286;
    for(int i = 0; i < 256; i++, w++){
        painter->drawLine(w, h, w, h - (double)bCount[i] / (double)maxB * 200);
    }

    painter->setPen(QPen(Qt::black));
    painter->drawLine(0, 500, 256, 500);
    painter->drawLine(0, 500, 0, 300);
    painter->drawText(253, 510, "255");
    painter->drawText(1, 510, "0");
    painter->setPen(QPen(Qt::green));
    w = 1; h = 500;
    for(int i = 0; i < 256; i++, w++){
        painter->drawLine(w, h, w, h - (double)gCount[i] / (double)maxG * 200);
    }

    painter->setPen(QPen(Qt::black));
    painter->drawLine(285, 500, 540, 500);
    painter->drawLine(285, 500, 285, 300);
    painter->drawText(528, 510, "255");
    painter->drawText(286, 510, "0");
    painter->setPen(QPen(Qt::gray));
    w = 286; h = 500;
    for(int i = 0; i < 256; i++, w++){
        painter->drawLine(w, h, w, h - (double)brightCount[i] / (double)maxBright * 200);
    }

    painter->end();
    painter->begin(this);
    painter->drawPicture(0,0, *picture);
    painter->end();
}
