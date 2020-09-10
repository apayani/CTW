#pragma once
#include <vector>
#include <array>
#include <unordered_map>
#include <math.h>
#include <memory>

const size_t LOG_TBL_SIZE = 514;
using namespace std;

 

// this class reprsent each context in the context tree algorithm
class CTWNodeMB
{
public:

	// conditional log probability of the context
	float logB;
	CTWNodeMB* parent;

	// counters for 0 and 1 symbols
	unsigned char  n0;
	unsigned char  n1;
	int level=0;
	
	CTWNodeMB() : logB(0.0), parent(nullptr), n0(0), n1(0), level(0) {};
	CTWNodeMB(CTWNodeMB *parent) : logB(0.0), parent(parent), n0(0), n1(0) {
		this->level = parent->level+1 ;

	} ;

	// pointer to each child node in the context tree
	unordered_map <int, unique_ptr<CTWNodeMB> > children;
};


// this class update the binary context tree and provide the probability of a new symbol given context 
class CtwBinaryTree
{
	// should we prune
	bool prunning = true;

	//number of total nodes 
	int node_count = 0;
	
	unique_ptr<CTWNodeMB> root;
	int depth; // maximum depth
	int max_tree_size;
	vector <unsigned char> context;
	

	CTWNodeMB* FindContext(bool bUpdate);  // return the leaf of the context
	double get_prob_0(CTWNodeMB* node); 
	
	 
	double CTWJacEst(double d)
	{
		if (d < -2)
			return .0000001;
		return log(1 + exp(d));
	}
	

public:
	CtwBinaryTree(void) ;
	~CtwBinaryTree(void);

	void clear();
	bool init(int depth, int max_tree_size, bool prune = true);
	
	void  update_tree(unsigned char  new_bit);
	double get_prob_0();
	

	//update a single bit in the context
	void update_context(unsigned char  new_bit);
	//set the context 
	void set_context(unsigned char  new_byte);
	
	
	void set_max_size( int max_tree_size) { this->max_tree_size = max_tree_size ; }
	int get_max_tree_size() { return this->max_tree_size;};
	
	void set_pruning( bool pruning) { this->prunning  = prunning; }
	bool get_prunning() { return this->prunning;};
	
	 
};

