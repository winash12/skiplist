#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <immintrin.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <memory>
#include <vector>


#include "CSSLRangeSearchResult.h"
#include "CSSLProxyNode.h"

using std::unique_ptr;
using std::shared_ptr;
using std::vector;

#define MAX_SKIP 2
// initial size of the highest fast lane with number
// of keys that fit into one cache line
// The total size of cache line is 64 bytes or 512 bits
// We are using 64 bit integers hence 8 keys per cache line
#define TOP_LANE_BLOCK 8
// number of keys that can be stored in one SIMD register
#define SIMD_SEGMENTS 4
// data list node


class CSSLSkipList
{
 private:
 public:
  void createSkipList(uint8_t maxLevel,uint8_t skip);
  void insertElement(uint64_t key);
  CSSLDataNode searchStartKey(uint64_t startKey);
  uint64_t searchElement(uint64_t key);
  CSSLRangeSearchResult searchRange(uint64_t startKey, uint64_t endKey);
 private:
  uint64_t   insertItemIntoFastLane(int8_t level,
				    shared_ptr<CSSLDataNode> newNode);

  void      buildFastLanes();

  void     calculateFastLaneSizes();
  void     allocateFastLanes();
  void      resizeFastLanes();
  void findAndInsertIntoProxyNode(shared_ptr<CSSLDataNode> node);
  shared_ptr<CSSLDataNode> newNode(uint64_t key);
  shared_ptr<CSSLProxyNode> newProxyNode(shared_ptr<CSSLDataNode> node);

};
