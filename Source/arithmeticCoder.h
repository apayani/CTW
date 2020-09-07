#pragma once

#include <fstream>
#include <stdio.h>
#include <vector>

using namespace std;
 
class ArithmeticCoder
{
public:

  ArithmeticCoder();
 ~ArithmeticCoder();
 
  void     start_encoder(void);
  void     start_decoder(void);
  
  void     read_from_file(string filename);  // read code data, start decoder
  unsigned stop_encoder(void);                 // returns number of unsigned char s used
  
  unsigned write_to_file(string filename);   // stop encoder, write code data
  void     stop_decoder(void);
 
  void     encode(unsigned bit,double prob_zero);
  unsigned int decode(double prob_zero);
   
  // used by decoder to check if the stream has reached the end
  bool has_data();


private:  
  void propagate_carry(void);
  void renorm_enc_interval(void);
  void renorm_dec_interval(void);
 
  vector <unsigned char> code_buffer;
  int ac_pointer ;
  unsigned base, value, length;                     // arithmetic coding state
};
