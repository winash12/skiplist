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

using std::cout;
using std::endl;


CSSLSkipList::CSSLSkipList(int maxLevel,int skip,int initialFastLaneSize)
{

  this->maxLevel = maxLevel;
  this->skip  = skip;
  this->initialFastLaneSize = initialFastLaneSize;
  fastLaneItems->resize(maxLevel);
  startOfFastLane->resize(maxLevel);
  for (int i = 0; i < maxLevel; ++i )
    {
      fastLaneItems->push_front(0);
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
  itemsPerLevel->push_front(current);
  startOfFastLane->push_front(value);
  for (int level = this->maxLevel - 2; level >= 0; level--)
    {
      value += current;
      current *= skip;
      itemsPerLevel->push_front(current);
      startOfFastLane->push_front(value);
      fastLaneSize += current;
    }

  /*for(auto it = startOfFastLane->begin(); it != startOfFastLane->end(); it++)
    {
      cout<< *it << endl;  
    }
  cout << "The value of fastLaneSize is " << fastLaneSize << endl;  
  
  for(auto it = itemsPerLevel->begin(); it != itemsPerLevel->end(); it++)
    {
      cout<< *it << endl;  
      
      }*/

  fastLanes->resize(fastLaneSize,INT_MAX);
}

void CSSLSkipList::printFastLanes()
{
  for (auto it = fastLanes->begin(); it != fastLanes->end(); ++it)
    {
      if (*it != INT_MAX)
	cout << *it << endl;
    }
}

void CSSLSkipList::insertItemsIntoFastLanes(std::vector<uint64_t> inputData)
{

  for (int level = 0,p=5;level < maxLevel;p *=5,level++)
    {
      for (int i = 0;i < inputData.size();i +=p)
      {
	insertItemIntoFastLane(level,inputData.at(i));
      }
    }
  skipList.reset(new std::vector<uint64_t>(std::move(inputData)));
}

void CSSLSkipList::insertItemIntoFastLane(int level,uint64_t key)
{
  int index1 = startOfFastLane->at(level);
  //cout << "The value of index1 is " <<index1 << endl;
  int index2 = fastLaneItems->at(level);
  //cout << "The value of index2 is " <<index2 << endl;
  int curPos = index1 + index2;
  int index3 = itemsPerLevel->at(level);
 
  //cout << "The value of curPos is " <<curPos << endl;

  int levelLimit = curPos + index3;
  //cout << "The value of levelLimit is " << levelLimit << endl;

  if (curPos > levelLimit)
  curPos = levelLimit;


  while( (key > fastLanes->at(curPos)) && (curPos < levelLimit))
    curPos++;


  fastLanes->insert(fastLanes->begin()+curPos,key);

  fastLaneItems->at(level)++;
}

