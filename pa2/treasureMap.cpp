#include <string>
#include <bitset>
#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    base = baseim;
    maze = mazeim;
    start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc)
{
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*((pixel->r)/4);
    pixel->g = 2*((pixel->g)/4);
    pixel->b = 2*((pixel->b)/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int dd)
{
    std::string bin = std::bitset<6>(dd%64).to_string();
    int d = std::stoi(bin);

    // get pixel in image
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

    // divide d into 3 parts; first two bits, second two bits, last two bits
    int dOne = d/10000;
    int dThree = d%100;
    int dTwo = (d-(dOne*10000))/100;

    // set the new color channel values
    pixel->r = newVal(pixel->r, dOne);
    pixel->g = newVal(pixel->g, dTwo);
    pixel->b = newVal(pixel->b, dThree);
}

int treasureMap::newVal(int colChannel, int dVal) 
{
    int dDecimalVal = (dVal/10)*2 + (dVal%10);
    // second last and last bits of the binary representation of colChannel
    int secondLastColBit = 0;
    int lastColBit = 0;

    if (colChannel%2 == 1) {
        lastColBit = 1;
    }
    if ((colChannel/2)%2 == 1) {
        secondLastColBit = 1;
    }

    int colDecimalVal = 2*secondLastColBit + lastColBit;

    if (dDecimalVal == colDecimalVal) {
        return colChannel;
    }
    if (colDecimalVal > dDecimalVal) {
        return colChannel-(colDecimalVal-dDecimalVal);
    }
    return colChannel+(dDecimalVal-colDecimalVal); 
}

PNG treasureMap::renderMap()
{
    PNG newBase = base;
    vector<vector<bool>> vb(newBase.width(), vector<bool>(newBase.height()));
    vector<vector<int>> vi(newBase.width(), vector<int>(newBase.height()));
    Queue<pair<int,int>> queue;

    vb[start.first][start.second] = true;
    vi[start.first][start.second] = 0;

    setLOB(newBase, make_pair(start.first, start.second), 0);

    queue.enqueue(make_pair(start.first, start.second));

    while (!queue.isEmpty()) {
        pair<int,int> curr = queue.dequeue();
        vector<pair<int,int>> n = neighbors(curr);
        for (int i = 0; i < 4; i++) {
            if (good(vb, curr, n[i])) {
                int colIndex = n[i].first;
                int rowIndex = n[i].second;
                vb[colIndex][rowIndex] = true;
                vi[colIndex][rowIndex] = vi[curr.first][curr.second]+1;
                setLOB(newBase, n[i], vi[colIndex][rowIndex]);
                queue.enqueue(n[i]);
            }
        }
    }
    return newBase;
}

PNG treasureMap::renderMaze()
{
    PNG newBase = base;
    vector<vector<bool>> vb(newBase.width(), vector<bool>(newBase.height()));
    vector<vector<int>> vi(newBase.width(), vector<int>(newBase.height()));
    Queue<pair<int,int>> queue;

    vb[start.first][start.second] = true;
    vi[start.first][start.second] = 0;

    setGrey(newBase, make_pair(start.first, start.second));

    queue.enqueue(make_pair(start.first, start.second));

    while (!queue.isEmpty()) {
        pair<int,int> curr = queue.dequeue();
        vector<pair<int,int>> n = neighbors(curr);
        for (int i = 0; i < 4; i++) {
            if (good(vb, curr, n[i])) {
                int colIndex = n[i].first;
                int rowIndex = n[i].second;
                vb[colIndex][rowIndex] = true;
                vi[colIndex][rowIndex] = vi[curr.first][curr.second]+1;
                setGrey(newBase, make_pair(colIndex, rowIndex));
                queue.enqueue(n[i]);
            }
        }
    }

    for (int x = start.first-3; x <= start.first+3; x++) {
        for (int y = start.second-3; y <= start.second+3; y++) {
            if (x>=0 && x<newBase.width() && y>=0 && y<newBase.height()) {
                RGBAPixel *pixel = newBase.getPixel(x,y);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
            }
        }
    }
    return newBase;  
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next)
{
    if ((next.first < 0) || (next.first >= base.width()) || (next.second < 0) || (next.second >= base.height())) {
        return false;
    }
    if (v[next.first][next.second] == true) {
        return false;
    }

    RGBAPixel *currPixel = maze.getPixel(curr.first, curr.second);
    RGBAPixel *nextPixel = maze.getPixel(next.first, next.second);

    if (!(*currPixel == *nextPixel)) {
        return false;
    }
    return true;
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) 
{
    int currCol = curr.first;
    int currRow = curr.second;

    pair<int,int> left = make_pair(currCol-1, currRow);
    pair<int,int> below = make_pair(currCol, currRow-1);
    pair<int,int> right = make_pair(currCol+1, currRow);
    pair<int,int> above = make_pair(currCol, currRow+1);

    vector<pair<int,int>> temp;
    temp.push_back(left);
    temp.push_back(below);
    temp.push_back(right);
    temp.push_back(above);
    
    return temp;
}

