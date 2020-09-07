#include <iostream>
#include <chrono>
#include "Source/ctwEngine.h"

using namespace std;



void encode(string src, string dest)
{
	auto t1 = chrono::high_resolution_clock::now(); 
	
	CtwEngine modeler(8, 1000000);
	int size = modeler.encode(src, dest);
	
	auto t2 = chrono::high_resolution_clock::now();


	cout << "encoding finished,   out size: " << size << endl;
	cout << "elapsed :" << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << endl;


}

void decode(string src, string dest)
{

	auto t1 = chrono::high_resolution_clock::now();
	
	CtwEngine modeler(8, 1000000);
	int size = modeler.decode(src, dest);

	auto t2 = chrono::high_resolution_clock::now();


	cout << "decoding finished,   out size: " << size << endl;
	cout << "elapsed :" << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << endl;
	

}


void disp_usage()
{
	printf("\nIncorrect arguments");
	printf("\nUSAGE for encode:   CTW_CMP.exe e  src_file  dest_file");
	printf("\nUSAGE for decode:   CTW_CMP.exe d  src_file  dest_file");
}

int main(int argc, char* argv[])
{



	if (argc != 4)
	{
		disp_usage();
		return 0;
	}


	if (!_stricmp(argv[1], "e"))
	{
		encode(argv[2], argv[3]);
		return 0;
	}

	if (!_stricmp(argv[1], "d"))
	{
		decode(argv[2], argv[3]);
		return 0;
	}

	disp_usage();
	return 0;

}

