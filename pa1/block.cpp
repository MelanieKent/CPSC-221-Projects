#include "block.h"
#include <cmath>
#include <iostream>

int Block::width() const
{
    return data[0].size();
}
int Block::height() const
{
    return data.size();
}

void Block::render(PNG &im, int column, int row) const // should work
{
    int hIndex = 0;

    for (int y = row; y < (row + height()); y++) {
        int wIndex = 0;
        for (int x = column; x < (column + width()); x++) {
            RGBAPixel *pixel = im.getPixel(x,y);
            *pixel = data[hIndex][wIndex];
            wIndex++;
        }
        hIndex++;
    }
}

void Block::build(PNG &im, int column, int row, int width, int height)
{
     for (int y = row; y < (row + height); y++) {
        vector<RGBAPixel> currRow;
        for (int x = column; x < (column + width); x++) {
            RGBAPixel currPixel = *im.getPixel(x,y);
            currRow.push_back(currPixel);
        }
        data.push_back(currRow);
    } 
}

void Block::flipVert()
{
    int currLastPos = height() - 1;
    for (int y = 0; y < (height()/2); y++) {
        vector<RGBAPixel> currVec = data[y];
        data[y] = data[currLastPos];
        data[currLastPos] = currVec;
        currLastPos--;
    }
}


void Block::flipHoriz()
{
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width()/2; x++) {
            RGBAPixel pixel = data[y][x];
            data[y][x] = data[y][width()-x-1];
            data[y][width()-x-1] = pixel;
        }
    }
}


void Block::rotateRight()
{
    vector<vector<RGBAPixel>> newData;
    for (int x = 0; x < width(); x++) {
        vector<RGBAPixel> newVec;

        for (int y = (height()-1); y >= 0; y--) {
            RGBAPixel currPixel = data[y][x];
            newVec.push_back(currPixel);
        }
        newData.push_back(newVec);
    }
    data = newData;
}
