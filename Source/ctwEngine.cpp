#include "ctwEngine.h"
using namespace std;

const unsigned short shift_mask[] = { 	/* use the most significant part first */
	0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe
};

#define BytePrefix(b,f) (b & shift_mask[f])
#define ByteBit(b,f)    ((b >> (7-f)) & 1)
const double EPSILLON = .000001;


CtwEngine::CtwEngine(int depth, int max_tree_size)
{
	init(depth,   max_tree_size);
}


CtwEngine::~CtwEngine()
{

}


bool CtwEngine::init(int depth, int max_tree_size)
{
	for (auto &m : bit_models)
		if (!m.init(depth, max_tree_size))
			return false;
		
	return true;
}
 
  
 
void CtwEngine::clear()
{
	for (auto m : bit_models)
	{
		m.clear();
	}
 
}

int CtwEngine::encode(string uncompressed, string compressed)
{
	 
	// set up an arithmetic coder
	ArithmeticCoder ac;
	ac.start_encoder();


	// read inut file into a buffer
	ifstream is;
	is.open(uncompressed, ios::in | ios::binary);
	is.seekg(0, is.end);
	int length = is.tellg();
	is.seekg(0, is.beg);

	vector <unsigned char> buffer(length);
	is.read( (char*)buffer.data(), length);

	for( auto byte : buffer)
	{
		for (int pos = 0; pos < 8; ++pos)
		{
			unsigned char  bit = ByteBit(byte, pos); 
			unsigned char  prefix = BytePrefix(byte, pos);


			// setting previous bits of current unsigned char  as history
			bit_models[pos].update_context(prefix);


			double prob_0 = bit_models[pos].get_prob_0();
			prob_0 = min(1 - EPSILLON, max(EPSILLON, prob_0));
			ac.encode(bit, prob_0);


			bit_models[pos].update_tree(bit);
			bit_models[pos].set_context(byte) ;
		}

	}

	return ac.write_to_file(compressed) ;

}

int CtwEngine::decode(string compressed, string decompressed)
{
	
	// read compressed file via arithmetic coder
	ArithmeticCoder ac;
	ac.read_from_file(compressed);

	// create outut stream
	ofstream ofs;
	ofs.open(decompressed, ios::out | ios::binary);
	
	 
	 
	int size = 0;
	while(ac.has_data())
	{

		unsigned char cur_byte = 0;
		
		for (int pos = 0; pos < 8; pos++)
		{
			unsigned char  bit, prefix;
			prefix = cur_byte ;


			// setting previous bits of current unsigned char  as context
			bit_models[pos].update_context(prefix);


			double prob_0 = bit_models[pos].get_prob_0();
			prob_0 = min(1 - EPSILLON, max(EPSILLON, prob_0));
			bit = ac.decode(prob_0);


			cur_byte  |= (bit << (7 - pos));
			bit_models[pos].update_tree(bit);
		}
		for (int pos = 0; pos < 8; pos++)
		{
			bit_models[pos].set_context(cur_byte);
		}
		// write the decoded byte to file
		ofs.write((char*)&cur_byte , 1);
		++size;

	}

	return size;
}

