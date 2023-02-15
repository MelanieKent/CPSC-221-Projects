#include <string>
#include <bitset>
#include <bits/stdc++.h>
#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s) :start(s),mapImg(tm) 
{
   Stack<pair<pair<int,int>,pair<int,int>>> paths;

   vector<vector<bool>> vb(mapImg.width(), vector<bool>(mapImg.height()));
   vector<vector<int>> vd(mapImg.width(), vector<int>(mapImg.height()));
   Queue<pair<int,int>> queue;

   vb[start.first][start.second] = true;
   vd[start.first][start.second] = 0;

   queue.enqueue(make_pair(start.first, start.second));
   pair<int,int> lastVisited; 

   while (!queue.isEmpty()) {

      pair<int,int> curr = queue.dequeue();
      vector<pair<int,int>> n = neighbors(curr);

      for (int i = 0; i < 4; i++) {

         if (good(vb, vd, curr, n[i])) {
            int colIndex = n[i].first;
            int rowIndex = n[i].second;
            vb[colIndex][rowIndex] = true;
            vd[colIndex][rowIndex] = vd[curr.first][curr.second]+1;
            queue.enqueue(n[i]);
            paths.push(make_pair(make_pair(curr.first, curr.second), make_pair(colIndex, rowIndex)));

         }
      }
      lastVisited = curr;
   }

   pathPts.push_back(lastVisited);

   while(!paths.isEmpty()) {
      pair<pair<int,int>, pair<int,int>> temp = paths.peek(); 
      pathPts.push_back(temp.first);

      while (paths.peek().second != temp.first && !paths.isEmpty()) {
         paths.pop();
      }
   }

   reverse(pathPts.begin(), pathPts.end());

}


PNG decoder::renderSolution()
{
   PNG newMapImg = mapImg;

   for (int i = 0; i < pathPts.size(); i++) {
      pair<int,int> temp = pathPts[i];
      RGBAPixel *pixel = newMapImg.getPixel(temp.first, temp.second);
      pixel->r = 255;
      pixel->g = 0;
      pixel->b = 0;
   }
   return newMapImg;
}

PNG decoder::renderMaze()
{
   PNG newMapImg = mapImg;
   vector<vector<bool>> vb(newMapImg.width(), vector<bool>(newMapImg.height()));
   vector<vector<int>> vd(newMapImg.width(), vector<int>(newMapImg.height()));
   Queue<pair<int,int>> queue;

   vb[start.first][start.second] = true;
   vd[start.first][start.second] = 0;

   setGrey(newMapImg, make_pair(start.first, start.second));

   queue.enqueue(make_pair(start.first, start.second));

   while (!queue.isEmpty()) {
      pair<int,int> curr = queue.dequeue();
      vector<pair<int,int>> n = neighbors(curr);
      for (int i = 0; i < 4; i++) {
         if (good(vb, vd, curr, n[i])) {
            int colIndex = n[i].first;
            int rowIndex = n[i].second;
            vb[colIndex][rowIndex] = true;
            vd[colIndex][rowIndex] = vd[curr.first][curr.second]+1;
            setGrey(newMapImg, make_pair(colIndex, rowIndex));
            queue.enqueue(n[i]);
         }
      }
   }

   for (int x = start.first-3; x <= start.first+3; x++) {
      for (int y = start.second-3; y <= start.second+3; y++) {
         if (x>=0 && x<newMapImg.width() && y>=0 && y<newMapImg.height()) {
            RGBAPixel *pixel = newMapImg.getPixel(x,y);
            pixel->r = 255;
            pixel->g = 0;
            pixel->b = 0;
         }
      }
   }
   return newMapImg;
}

void decoder::setGrey(PNG & im, pair<int,int> loc)
{
   RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*((pixel->r)/4);
    pixel->g = 2*((pixel->g)/4);
    pixel->b = 2*((pixel->b)/4);
}

pair<int,int> decoder::findSpot()
{
   return pathPts[pathPts.size()-1];
}

int decoder::pathLength()
{
   return pathPts.size();
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next)
{
   if ((next.first < 0) || (next.first >= mapImg.width()) || (next.second < 0) || (next.second >= mapImg.height())) {
      return false;
   }
   if (v[next.first][next.second] == true) {
      return false;
   }

   RGBAPixel *pixel = mapImg.getPixel(next.first, next.second);

   if (!compare(*pixel, d[curr.first][curr.second]+1)) {
      return false;
   }
   return true;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) 
{
   int currCol = curr.first;
   int currRow = curr.second;

   pair<int,int> left = make_pair(currCol-1, currRow);
   pair<int,int> below = make_pair(currCol, currRow+1);
   pair<int,int> right = make_pair(currCol+1, currRow);
   pair<int,int> above = make_pair(currCol, currRow-1);

   vector<pair<int,int>> temp;
   temp.push_back(left);
   temp.push_back(below);
   temp.push_back(right);
   temp.push_back(above);
    
   return temp;
}

bool decoder::compare(RGBAPixel p, int d)
{
   int r = p.r;
   int g = p.g;
   int b = p.b;

   std::string rstr = std::bitset<8>(r).to_string();
   std::string gstr = std::bitset<8>(g).to_string();
   std::string bstr = std::bitset<8>(b).to_string();

   int dgoal = d%64;
   std::string dbin = std::bitset<6>(dgoal).to_string();

   std::string rlastbits = rstr.substr(6,2);
   std::string glastbits = gstr.substr(6,2);
   std::string blastbits = bstr.substr(6,2);

   std::string firstd = dbin.substr(0,2);
   std::string secondd = dbin.substr(2,2);
   std::string thirdd = dbin.substr(4,2);

   if (rlastbits==firstd && glastbits==secondd && blastbits==thirdd) {
      return true;
   }
   return false;
}
