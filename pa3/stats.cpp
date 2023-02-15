#include "stats.h"

stats::stats(PNG & im) {
    for (int x = 0; x < im.width(); x++) {
        vector<long> rtemp;
        vector<long> gtemp;
        vector<long> btemp;
        vector<long> rsqtemp;
        vector<long> gsqtemp;
        vector<long> bsqtemp;
        for (int y = 0; y < im.height(); y++) {
            RGBAPixel *pixel = im.getPixel(x, y);
            int r = pixel->r;
            int g = pixel->g;
            int b = pixel->b;
            long rsq = pow(pixel->r,2);
            long gsq = pow(pixel->g,2);
            long bsq = pow(pixel->b,2);

            int left = x-1;
            int top = y-1;
           
            if (left>=0 && top>=0) {
                r += sumRed[left][y] + rtemp[top] - sumRed[left][top];
                g += sumGreen[left][y] + gtemp[top] - sumGreen[left][top];
                b += sumBlue[left][y] + btemp[top] - sumBlue[left][top];
                rsq += sumsqRed[left][y] + rsqtemp[top] - sumsqRed[left][top]; 
                gsq += sumsqGreen[left][y] + gsqtemp[top] - sumsqGreen[left][top];
                bsq += sumsqBlue[left][y] + bsqtemp[top] - sumsqBlue[left][top];
            } else if (left>=0) {
                r += sumRed[left][y];
                g += sumGreen[left][y];
                b += sumBlue[left][y];
                rsq += sumsqRed[left][y];
                gsq += sumsqGreen[left][y];
                bsq += sumsqBlue[left][y];
            } else if (top>=0) {
                r += rtemp[top];
                g += gtemp[top];
                b += btemp[top];
                rsq += rsqtemp[top];
                gsq += gsqtemp[top];
                bsq += bsqtemp[top];
            } else {
                // nothing
            }
            rtemp.push_back(r);
            gtemp.push_back(g);
            btemp.push_back(b);
            rsqtemp.push_back(rsq);
            gsqtemp.push_back(gsq);
            bsqtemp.push_back(bsq);
        }
        sumRed.push_back(rtemp);
        sumGreen.push_back(gtemp);
        sumBlue.push_back(btemp);
        sumsqRed.push_back(rsqtemp);
        sumsqGreen.push_back(gsqtemp);
        sumsqBlue.push_back(bsqtemp);
    }
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h) {
    int x = ul.first;
    int y = ul.second;
    int ror = x+w-1; // index of right of rectangle
    int bor = y+h-1; // index of bottom of rectangle
    int left = x-1;
    int top = y-1;

    if (left>=0 && top>=0) {
        if (channel == 'r') {
           return sumRed[ror][bor] - sumRed[left][bor] - sumRed[ror][top] + sumRed[left][top];
        }
        if (channel == 'g') {
            return sumGreen[ror][bor] - sumGreen[left][bor] - sumGreen[ror][top] + sumGreen[left][top];
        }
        if (channel == 'b') {
            return sumBlue[ror][bor] - sumBlue[left][bor] - sumBlue[ror][top] + sumBlue[left][top];
        }
    } else if (left>=0) {
        if (channel == 'r') {
            return sumRed[ror][bor] - sumRed[left][bor];
        }
        if (channel == 'g') {
            return sumGreen[ror][bor] - sumGreen[left][bor];
        }
        if (channel == 'b') {
            return sumBlue[ror][bor] - sumBlue[left][bor];
        }
    } else if (top>=0) {
        if (channel == 'r') {
            return sumRed[ror][bor] - sumRed[ror][top];
        }
        if (channel == 'g') {
            return sumGreen[ror][bor] - sumGreen[ror][top];
        }
        if (channel == 'b') {
            return sumBlue[ror][bor] - sumBlue[ror][top];
        }
    } else {
        if (channel == 'r') {
            return sumRed[ror][bor];
        }
        if (channel == 'g') {
            return sumGreen[ror][bor];
        }
    }
    return sumBlue[ror][bor];
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h) {
    int x = ul.first;
    int y = ul.second;
    int ror = x+w-1; // index of right of rectangle
    int bor = y+h-1; // index of bottom of rectangle
    int left = x-1;
    int top = y-1;

    if (left>=0 && top>=0) {
        if (channel == 'r') {
           return sumsqRed[ror][bor] - sumsqRed[left][bor] - sumsqRed[ror][top] + sumsqRed[left][top];
        }
        if (channel == 'g') {
            return sumsqGreen[ror][bor] - sumsqGreen[left][bor] - sumsqGreen[ror][top] + sumsqGreen[left][top];
        }
        if (channel == 'b') {
            return sumsqBlue[ror][bor] - sumsqBlue[left][bor] - sumsqBlue[ror][top] + sumsqBlue[left][top];
        }
    } else if (left>=0) {
        if (channel == 'r') {
            return sumsqRed[ror][bor] - sumsqRed[left][bor];
        }
        if (channel == 'g') {
            return sumsqGreen[ror][bor] - sumsqGreen[left][bor];
        }
        if (channel == 'b') {
            return sumsqBlue[ror][bor] - sumsqBlue[left][bor];
        }
    } else if (top>=0) {
        if (channel == 'r') {
            return sumsqRed[ror][bor] - sumsqRed[ror][top];
        }
        if (channel == 'g') {
            return sumsqGreen[ror][bor] - sumsqGreen[ror][top];
        }
        if (channel == 'b') {
            return sumsqBlue[ror][bor] - sumsqBlue[ror][top];
        }
    } else {
        if (channel == 'r') {
            return sumsqRed[ror][bor];
        }
        if (channel == 'g') {
            return sumsqGreen[ror][bor];
        }
    }
    return sumsqBlue[ror][bor];
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h) {

    double varRed = getSumSq('r', ul, w, h) - (pow(getSum('r', ul, w, h),2)/(w*h));
    double varGreen = getSumSq('g', ul, w, h) - (pow(getSum('g', ul, w, h),2)/(w*h));
    double varBlue = getSumSq('b', ul, w, h) - (pow(getSum('b', ul, w, h),2)/(w*h));

    return varRed + varGreen + varBlue;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h) {
    int aveRed;
    int aveGreen;
    int aveBlue;

    aveRed = getSum('r', ul, w, h)/(w*h);
    aveGreen = getSum('g', ul, w, h)/(w*h);
    aveBlue = getSum('b', ul, w, h)/(w*h);

    RGBAPixel pixel(aveRed, aveGreen, aveBlue);
    return pixel;
}
