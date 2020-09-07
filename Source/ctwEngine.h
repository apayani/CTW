#pragma once
#include "ctwBinaryTree.h"
#include "ArithmeticCoder.h"
#include <array>
using namespace std;


class CtwEngine
{
public:
	CtwEngine(int depth, int max_tree_size);
	~CtwEngine();

	
	bool init(int nDepth, int nMaxTreeSize);
	bool update(unsigned char  data);
	void clear();


	int encode(string uncompressed, string compressed);
	int decode(string compressed, string decompressed);

private:
	array <CtwBinaryTree,8> bit_models;
	 
	 
};

