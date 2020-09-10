/*
	An Implementation of Context Tree Weighting compression algorithm by Ali Payani
*/


#include "ctwBinaryTree.h"
#include "math.h"
#include <iostream>
#include <fstream>
 
const double LOG2 = log(2.0);
#define get_hash(c) (c)

array <double, LOG_TBL_SIZE> log_table_plus_half;
array <double, LOG_TBL_SIZE> log_table_plus_one;



CtwBinaryTree::CtwBinaryTree(void)
{
	root = make_unique<CTWNodeMB>();
	for (int n = 0; n < LOG_TBL_SIZE; n++)
		log_table_plus_half[n] = log(n + .5);
	 
	for (int n = 0; n < LOG_TBL_SIZE; n++)
		log_table_plus_one[n] = log(n + 1.0);
	 
	
}
CtwBinaryTree::~CtwBinaryTree(void)
{
	
}

CTWNodeMB* CtwBinaryTree::FindContext(bool bUpdate)
{
	CTWNodeMB *node = root.get();
	for (int n = 0; n < depth; n++)
	{
		auto it = node->children.find(context[n]);  
		if ( it == node->children.end())
		{
			if (bUpdate && node_count<max_tree_size)
			{
				
				auto &ptr =node->children[ context[n] ] = make_unique<CTWNodeMB>(node) ;
				node = ptr.get();

				++node_count ;
			}
			else
				return node;
		}
		else
			node = it->second.get();

		
		if (prunning)
			if ( node->n0 < 2 || node->n1 < 2)
			{
				return node;
			}
	}

	return node;
}


bool CtwBinaryTree::init(int depth, int max_tree_size, bool prunning)
{
	this->prunning =  prunning ;
	this->max_tree_size = max_tree_size;
	this->depth = depth;

	// reset History
	context.assign(depth,0);
	  

	return true;
}

void CtwBinaryTree::update_tree(unsigned char  new_bit)
{

	// first update pe of leaf
	// for leaf logPw = 0 ;

	CTWNodeMB *node = FindContext(true);
	double log_gamma = log_table_plus_half[node->n0] - log_table_plus_half[node->n1];

	while (node->level > 1)
	{
		if (new_bit)
			node->n1++;
		else
			node->n0++;

		 
		if (node->n1 > 254 || node->n0 > 254)
		{
			node->n1 /= 2;
			node->n0 /= 2;
		}

		node = node->parent;

		double log_pw0, log_pw1;
		if (log_gamma >= 0)
		{
			log_pw0 = -CTWJacEst(-log_gamma);
			log_pw1 = log_pw0 - log_gamma;
		}
		else
		{
			log_pw1 = -CTWJacEst(log_gamma);
			log_pw0 = log_pw1 + log_gamma;

		}

		double log_pe0, log_pe1;


		double logcsumplusacc = log_table_plus_one[node->n0 + node->n1];
		log_pe0 = log_table_plus_half[node->n0] - logcsumplusacc;
		log_pe1 = log_table_plus_half[node->n1] - logcsumplusacc;


		double nom;
		double den;


		double logbetatimespe0 = node->logB + log_pe0;
		double diff0 = logbetatimespe0 - log_pw0;
		if (diff0 >= 0)
			nom = logbetatimespe0 + CTWJacEst(-diff0);
		else
			nom = log_pw0 + CTWJacEst(diff0);

		double logbetatimespe1 = node->logB + log_pe1;
		double diff1 = logbetatimespe1 - log_pw1;

		if (diff1 >= 0)
			den = logbetatimespe1 + CTWJacEst(-diff1);
		else
			den = log_pw1 + CTWJacEst(diff1);


		log_gamma = nom - den;


		if (new_bit)
			node->logB = logbetatimespe1 - log_pw1;
		else
			node->logB = logbetatimespe0 - log_pw0;



	}
	// for root
	if (new_bit)
		node->n1++;
	else
		node->n0++;
	if (node->n1 > 254 || node->n0 > 254)
	{
		node->n1 /= 2;
		node->n0 /= 2;
	}


	double dProb;
	if (log_gamma >= 0)
		dProb = 1 / (1 + exp(-log_gamma));
	else
		dProb = 1 - 1 / (1 + exp(log_gamma));


	//return  dProb;


}
double CtwBinaryTree::get_prob_0()
{
	CTWNodeMB*node =  FindContext(false);
	return get_prob_0(node);
}

double CtwBinaryTree::get_prob_0(CTWNodeMB* node)
{
	
	double log_gamma = log_table_plus_half[node->n0] - log_table_plus_half[node->n1];



	while (node->level > 1)
	{




		node = node->parent;

		double log_pw0, log_pw1;
		if (log_gamma >= 0)
		{
			log_pw0 = -CTWJacEst(-log_gamma);
			log_pw1 = log_pw0 - log_gamma;
		}
		else
		{
			log_pw1 = -CTWJacEst(log_gamma);
			log_pw0 = log_pw1 + log_gamma;

		}

		double log_pe0, log_pe1;


		double logcsumplusacc = log_table_plus_one[node->n0 + node->n1];
		log_pe0 = log_table_plus_half[node->n0] - logcsumplusacc;
		log_pe1 = log_table_plus_half[node->n1] - logcsumplusacc;


		double nom;
		double den;


		double logbetatimespe0 = node->logB + log_pe0;
		double diff0 = logbetatimespe0 - log_pw0;
		if (diff0 >= 0)
			nom = logbetatimespe0 + CTWJacEst(-diff0);
		else
			nom = log_pw0 + CTWJacEst(diff0);

		double logbetatimespe1 = node->logB + log_pe1;
		double diff1 = logbetatimespe1 - log_pw1;

		if (diff1 >= 0)
			den = logbetatimespe1 + CTWJacEst(-diff1);
		else
			den = log_pw1 + CTWJacEst(diff1);


		log_gamma = nom - den;

 
	}

	// for root



	double dProb;
	if (log_gamma >= 0)
		dProb = 1 / (1 + exp(-log_gamma));
	else
		dProb = 1 - 1 / (1 + exp(log_gamma));


	return  dProb;
}
void CtwBinaryTree::update_context(unsigned char  new_bit)
{
	for (int n = depth - 1; n > 0; n--)
	{
		context[n] = context[n - 1];
	}
	context[0] = new_bit;
}

void CtwBinaryTree::set_context(unsigned char new_byte)
{
	context[0] = new_byte;
}
 
 
void CtwBinaryTree::clear()
{
}

 
