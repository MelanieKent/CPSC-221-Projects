/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */
#include <algorithm>
#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
  // The constructor first builds the stats object used to compute
  //  * the variability and average of image subrectangles so that it can
  //  * pick the best partition of a Node of size w x h in O(wh) time.
  //  * Then it recursively partitions Nodes, starting at the root and
  //  * always using the best partition, until the Node's variability is
  //  * at most tol or the Node is a single pixel.
  //  * Default tolerance is 0.0.
SQtree::SQtree(PNG & imIn, double tol) {
  pair<int,int> ul(0,0);
  stats s(imIn);
  int w = imIn.width();
  int h = imIn.height();

  RGBAPixel a = s.getAvg(ul, w, h);
  double v = s.getVar(ul, w, h);

  root = new Node(ul, w, h, a, v);
  
  buildTreeHelper(s, tol, root);
}

/**
 * Helper for the SQtree constructor.
 */
// SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
// 				 int w, int h, double tol) {
//   RGBAPixel nodeAvg = s.getAvg(ul, w, h);
//   double nodeVar = s.getVar(ul, w, h);
//   Node *newNode = new Node(ul, w, h, nodeAvg, nodeVar);
//   if ((w==1 && h==1) || newNode->var < tol) {
//     return newNode;
//   }

//   double min = INFINITY;
//   double minSE = INFINITY;
//   int horizontalLine;
//   int verticalLine;
//   for (int yOffset = 1; yOffset <= h; yOffset++) {
//     for (int xOffset = 1; xOffset <= w; xOffset++) {
//       double maxSplit;
//       double areaSE;
//       if (!(xOffset == w && yOffset == h)) {
//         if (xOffset == w) {
//           maxSplit = horizontalSplit(s, ul, xOffset, yOffset, w, h);
//           areaSE = (h - yOffset)*w;
//         } else if (yOffset == h) {
//           maxSplit = verticalSplit(s, ul, xOffset, yOffset, w, h);
//           areaSE = (w - xOffset)*h;
//         } else {
//           maxSplit = fourSplit(s, ul, xOffset, yOffset, w, h);
//           areaSE = (w - xOffset)*(h - yOffset);
//         }
//         if (maxSplit <= min) {
//           if (maxSplit == min) {
//             if (areaSE <= minSE) {
//               minSE = areaSE;
//               horizontalLine = yOffset;
//               verticalLine = xOffset;
//               min = maxSplit;
//             }
//           } else {
//             horizontalLine = yOffset;
//             verticalLine = xOffset;
//             min = maxSplit;
//           }
//         }
//       }
//     }
//   }
//   if (horizontalLine == h) {
//     newNode->NW = buildTree(s, ul, verticalLine, h, tol);
//     pair<int, int> uR(ul.first+verticalLine, ul.second);
//     newNode->NE = buildTree(s, uR, w-verticalLine, h, tol);
//     newNode->SW = NULL;
//     newNode->SE = NULL;
//     return newNode;
//   } else if (verticalLine == w) {
//     newNode->NW = buildTree(s, ul, w, horizontalLine, tol);
//     pair<int, int> B(ul.first, ul.second+horizontalLine);
//     newNode->NE = buildTree(s, B, w, h-horizontalLine, tol);
//     newNode->SW = NULL;
//     newNode->SE = NULL;
//     return newNode;
//   } else {
//     newNode->NW = buildTree(s, ul, verticalLine, horizontalLine, tol);

//     pair<int, int> uR(ul.first+verticalLine, ul.second);
//     newNode->NE = buildTree(s, uR, w-verticalLine, horizontalLine, tol);

//     pair<int, int> lR(ul.first+verticalLine, ul.second+horizontalLine);
//     newNode->SE = buildTree(s, lR, w-verticalLine, h-horizontalLine, tol);

//     pair<int, int> lL(ul.first, ul.second+horizontalLine);
//     newNode->SW = buildTree(s, lL, verticalLine, h-horizontalLine, tol);
//     return newNode;
//   }
// }

// double SQtree::fourSplit(stats & s, pair<int,int> & ul, int xOffset, int yOffset, int w, int h) {
//   double upperLeft = s.getVar(ul, xOffset, yOffset);

//   pair<int,int> uR(ul.first + xOffset, ul.second);
//   double upperRight = s.getVar(uR, w-xOffset, yOffset);

//   pair<int,int> lL(ul.first, ul.second+yOffset);
//   double lowerLeft = s.getVar(lL, xOffset, h-yOffset);

//   pair<int,int> lR(ul.first+xOffset, ul.second+yOffset);
//   double lowerRight = s.getVar(lR, w-xOffset, h-yOffset);

//   return std::max({upperLeft, upperRight, lowerLeft, lowerRight});
// }

// double SQtree::horizontalSplit(stats & s, pair<int,int> & ul, int xOffset, int yOffset, int w, int h) {
//   double Top = s.getVar(ul, w, yOffset);

//   pair<int,int> lL(ul.first, ul.second+yOffset);
//   double Bottom = s.getVar(lL, w, h-yOffset);

//   return std::max({Top, Bottom});
// }

// double SQtree::verticalSplit(stats & s, pair<int,int> & ul, int xOffset, int yOffset, int w, int h) {
//   double Left = s.getVar(ul, xOffset, h);
  
//   pair<int,int> uR(ul.first+xOffset, ul.second);
//   double Right = s.getVar(uR, w-xOffset, h);

//   return std::max({Left, Right});
// }


/**
 * Helper for the SQtree constructor.
 */
void SQtree::buildTreeHelper(stats & s, double tol, Node *& curr) {
  if (curr == NULL) {
    return;
  }
  double minVar = curr->var;
  pair<int, int> optSplit = make_pair(0,0); // pair for optimal split

  if (curr->var > tol) {
    for (int x = 0; x < curr->width; x++) {
        for (int y = 0; y < curr->height; y++) {
            double varNW;
            double varNE;
            double varSW;
            double varSE;

            if ((x==0)&&(y==0)) {
                varNW = 0;
                varNE = 0;
                varSW = 0;
                varSE = minVar;
              } else if (x==0) {
                varNW = s.getVar(curr->upLeft, curr->width, y);
                varNE = 0;
                varSW = s.getVar(make_pair(curr->upLeft.first, curr->upLeft.second+y), curr->width, curr->height-y);
                varSE = 0;
              } else if (y==0) {
                varNW = s.getVar(curr->upLeft, x, curr->height);
                varNE = s.getVar(make_pair(curr->upLeft.first+x, curr->upLeft.second), curr->width-x, curr->height);
                varSW = 0; 
                varSE = 0;
              } else {
              varNW = s.getVar(curr->upLeft, x, y);
              varNE = s.getVar(make_pair(curr->upLeft.first+x, curr->upLeft.second), curr->width-x, y);
              varSW = s.getVar(make_pair(curr->upLeft.first, curr->upLeft.second+y), x, curr->height-y);
              varSE = s.getVar(make_pair(curr->upLeft.first+x, curr->upLeft.second+y), curr->width-x, curr->height-y);
              }

            if (std::max({varNW, varNE, varSW, varSE}) <= minVar) {
              minVar = std::max({varNW, varNE, varSW, varSE});
              optSplit = make_pair(x,y);
            }
          }
        }

    if ((optSplit.first == 0) && (optSplit.second == 0)) {
        // do nothing
      } else if (optSplit.first == 0) {
        RGBAPixel aNW = s.getAvg(curr->upLeft, curr->width, optSplit.second);
        RGBAPixel aSW = s.getAvg(make_pair(curr->upLeft.first, curr->upLeft.second+optSplit.second), curr->width, curr->height-optSplit.second);
        double vNW = s.getVar(curr->upLeft, curr->width, optSplit.second);
        double vSW = s.getVar(make_pair(curr->upLeft.first, curr->upLeft.second+optSplit.second), curr->width, curr->height-optSplit.second);
        curr->NW = new Node(curr->upLeft, curr->width, optSplit.second, aNW, vNW);
        curr->SW = new Node(make_pair(curr->upLeft.first, curr->upLeft.second+optSplit.second), curr->width, curr->height-optSplit.second, aSW, vSW);
      } else if (optSplit.second == 0) {
        RGBAPixel aNW = s.getAvg(curr->upLeft, optSplit.first, curr->height);
        RGBAPixel aNE = s.getAvg(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second), curr->width-optSplit.first, curr->height);
        double vNW = s.getVar(curr->upLeft, optSplit.first, curr->height);
        double vNE = s.getVar(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second), curr->width-optSplit.first, curr->height);
        curr->NW = new Node(curr->upLeft, optSplit.first, curr->height, aNW, vNW);
        curr->NE = new Node(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second), curr->width-optSplit.first, curr->height, aNE, vNE);
      } else {
        RGBAPixel aNW = s.getAvg(curr->upLeft, optSplit.first, optSplit.second);
        RGBAPixel aNE = s.getAvg(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second), curr->width-optSplit.first, optSplit.second);
        RGBAPixel aSW = s.getAvg(make_pair(curr->upLeft.first, curr->upLeft.second+optSplit.second), optSplit.first, curr->height-optSplit.second);
        RGBAPixel aSE = s.getAvg(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second+optSplit.second), curr->width-optSplit.first, curr->height-optSplit.second);
        double vNW = s.getVar(curr->upLeft, optSplit.first, optSplit.second);
        double vNE = s.getVar(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second), curr->width-optSplit.first, optSplit.second);
        double vSW = s.getVar(make_pair(curr->upLeft.first, curr->upLeft.second+optSplit.second), optSplit.first, curr->height-optSplit.second);
        double vSE = s.getVar(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second+optSplit.second), curr->width-optSplit.first, curr->height-optSplit.second);
        curr->NW = new Node(curr->upLeft, optSplit.first, optSplit.second, aNW, vNW);
        curr->NE = new Node(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second), curr->width-optSplit.first, optSplit.second, aNE, vNE);
        curr->SW = new Node(make_pair(curr->upLeft.first, curr->upLeft.second+optSplit.second), optSplit.first, curr->height-optSplit.second, aSW, vSW);
        curr->SE = new Node(make_pair(curr->upLeft.first+optSplit.first, curr->upLeft.second+optSplit.second), curr->width-optSplit.first, curr->height-optSplit.second, aSE, vSE);
      }
      buildTreeHelper(s, tol, curr->NW);
      buildTreeHelper(s, tol, curr->NE);
      buildTreeHelper(s, tol, curr->SW);
      buildTreeHelper(s, tol, curr->SE);
  }
          // do nothing, either good or a single pixel
}

// double SQtree::maxVar(double vNW, double vNE, double vSW, double vSE) {
//   if ((vNW >= vNE) && (vNW >= vSW) && (vNW >= vSE)) {
//     return vNW;
//   }
//   if ((vNE >= vNW) && (vNE >= vSW) && (vNE >= vSE)) {
//     return vNE;
//   }
//   if ((vSW >= vNW) && (vSW >= vNE) && (vSW >= vSE)) {
//     return vSW;
//   }
//   return vSE;
// }

/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG png(root->width, root->height);
  renderHelper(png, root);
  return png;
}

void SQtree::renderHelper(PNG &png, Node * curr) {

  if (curr == NULL) {
    return;
  }

  if ((curr->NW==NULL)&&(curr->NE==NULL)&&(curr->SW==NULL)&&(curr->SE==NULL)) {
    int xUL = curr->upLeft.first;
    int yUL = curr->upLeft.second;
    int w = curr->width;
    int h = curr->height;

    for (int x = xUL; x < w+xUL; x++) {
      for (int y = yUL; y < h+yUL; y++) {
        RGBAPixel *pixel = png.getPixel(x,y);
        *pixel = curr->avg;
      }
    }
  }

  renderHelper(png, curr->NW);
  renderHelper(png, curr->NE);
  renderHelper(png, curr->SW);
  renderHelper(png, curr->SE);
}


void SQtree::copy(const SQtree & other) {
  root = new Node(other.root->upLeft, other.root->width, other.root->height, other.root->avg, other.root->var);

  copyHelper(root, other.root);
}

void SQtree::copyHelper(Node *& newRoot, const Node * oldRoot) {

  if (oldRoot == NULL) {
    return;
  }
  if (oldRoot->NW != NULL) {
    newRoot->NW = new Node(oldRoot->NW->upLeft, oldRoot->NW->width, oldRoot->NW->height, oldRoot->NW->avg, oldRoot->NW->var);
  }
  if (oldRoot->NE != NULL) {
    newRoot->NE = new Node(oldRoot->NE->upLeft, oldRoot->NE->width, oldRoot->NE->height, oldRoot->NE->avg, oldRoot->NE->var);
  }
  if (oldRoot->SW != NULL) {
    newRoot->SW = new Node(oldRoot->SW->upLeft, oldRoot->SW->width, oldRoot->SW->height, oldRoot->SW->avg, oldRoot->SW->var);
  }
  if (oldRoot->SE != NULL) {
    newRoot->SE = new Node(oldRoot->SE->upLeft, oldRoot->SE->width, oldRoot->SE->height, oldRoot->SE->avg, oldRoot->SE->var);
  }

  copyHelper(newRoot->NW, oldRoot->NW);
  copyHelper(newRoot->NE, oldRoot->NE);
  copyHelper(newRoot->SW, oldRoot->SW);
  copyHelper(newRoot->SE, oldRoot->SE);
}

int SQtree::size() {
  return getSize(root);
}

int SQtree::getSize(const Node* curr) {
  if (curr == NULL) {
    return 0;
  } else {
    return 1 + getSize(curr->NW) + getSize(curr->NE) + getSize(curr->SW) + getSize(curr->SE);
  }
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // clearHelper(root);
}

void SQtree::clearHelper(Node * curr) {
  // while(curr != NULL) {
  //   clearHelper(curr->NW);
  //   clearHelper(curr->NE);
  //   clearHelper(curr->SE);
  //   clearHelper(curr->SW);
  // }
  // free(curr);
}

SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
 				int w, int h, double tol) {
  // not used
  return NULL;
}

