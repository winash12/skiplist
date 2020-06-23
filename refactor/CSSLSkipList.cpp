#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <immintrin.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <iostream>
#include <stdalign.h>
#include <stdexcept>
#include <memory>
#include <functional>
#include <numeric>
#include "CSSLSkipList.h"

using std::vector;
using std::cout;
using std::endl;


CSSLSkipList::CSSLSkipList(int maxLevel,int skip,int initialFastLaneSize)
{

  this->maxLevel = maxLevel;
  this->skip  = skip;
  this->initialFastLaneSize = initialFastLaneSize;
  for (int i = 0; i < maxLevel; ++i )
    {
      (*fastLaneItems).push_back(0);
    }
  calculateFastLaneSize();
}


void CSSLSkipList::calculateFastLaneSize()

{
  // calculate level sizes level by level
  int value;
  int current;
  current = initialFastLaneSize;
  value = 0;
  fastLaneSize = initialFastLaneSize;
  for (int level = this->maxLevel - 2; level >= 0; level--)
    {
      (*itemsPerLevel).push_back(current);
      //cout << "The value of value is " << value << endl;
      (*startOfFastLane).push_back(value);
      value += current;
      current *= skip;
      //cout <<"The value of current is " << current << endl;
      fastLaneSize += current;
      //cout << "The value of value is " << fastLaneSize << endl;
    }
  //cout << "The value of fastLaneSize is " << fastLaneSize << endl;
  for ( int i = 0; i < fastLaneSize; ++i )
    {
      (*fastLanes).push_back(INT_MAX);
    }
}

void CSSLSkipList::insertElement(uint64_t key)
{
  addElementToSkipList(key);
  
  for (int level = 0;level < maxLevel;level++)
    {
      insertItemIntoFastLane(level,key);
    }
}

void CSSLSkipList::insertItemIntoFastLane(int level,uint64_t key)
{
  cout << "The value of level is " <<level << endl;
  int index1 = (*startOfFastLane).at(level);
  cout << "The value of index1 is " <<index1 << endl;
  int index2 = (*fastLaneItems).at(level);
  cout << "The value of index2 is " <<index2 << endl;


  int index3 = (*itemsPerLevel).at(level);

  int curPos = index1 + index2;
  cout << "The value of curPos is " <<curPos << endl;

  int levelLimit = curPos + index3;
  if (curPos > levelLimit)
    curPos = levelLimit;

  while(key > (*fastLanes).at(curPos) && curPos < levelLimit)
    curPos++;

  (*fastLanes).insert((*fastLanes).begin()+curPos,key);

}

void CSSLSkipList::addElementToSkipList(uint64_t key)
{
  (*skipList).push_back(key);
}
