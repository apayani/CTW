#pragma once
#include <vector>
#include <array>
#include <unordered_map>

const size_t LOG_TBL_SIZE = 514;
using namespace std;



class CTWNodeMB
{
public:

	float logB = 0.0;
	CTWNodeMB* parent=nullptr;
	unsigned char  context=0;  // show which context leads to this node
	unsigned char  n0=0, n1=0;
	int level=0;
	
	unordered_map <int, CTWNodeMB> children;
};

class CtwBinaryTree
{
	bool prunning = true;

	
	CTWNodeMB root;
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
	CtwBinaryTree(void);
	~CtwBinaryTree(void);

	void clear();
	bool init(int depth, int max_tree_size);
	
	void  update_tree(unsigned char  new_bit);
	double get_prob_0();
	

	void update_context(unsigned char  new_bit);
	void set_context(unsigned char  new_bit);
	int get_max_tree_size() { return max_tree_size;};
 
	
	
	
	 

};

