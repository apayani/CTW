#include <stdlib.h>
#include "ArithmeticCoder.h"
#include <iterator>

// - - Constants - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const unsigned AC__MinLength = 0x01000000U;   // threshold for renormalization
const unsigned AC__MaxLength = 0xFFFFFFFFU;      // maximum AC interval length

const unsigned BM__LengthShift = 13;     // length bits discarded before mult.
const unsigned BM__MaxCount    = 1 << BM__LengthShift;  // for adaptive models

const unsigned DM__LengthShift = 15;     // length bits discarded before mult.
const unsigned DM__MaxCount    = 1 << DM__LengthShift;  // for adaptive models

 
void ArithmeticCoder::propagate_carry(void)
{
  int p;            // carry propagation on compressed data buffer
  for (p = code_buffer.size()-1; code_buffer[p] == 0xFFU; p--) code_buffer[p] = 0;
  ++code_buffer[p];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ArithmeticCoder::renorm_enc_interval(void)
{
  do {         
      // output and discard top unsigned char 
    code_buffer.push_back((unsigned char)(base >> 24)); 
   
    base <<= 8;
  } while ((length <<= 8) < AC__MinLength);        // length multiplied by 256
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ArithmeticCoder::renorm_dec_interval(void)
{
  do {          
      // read least-significant unsigned char 
      
    value = (value << 8) | unsigned(code_buffer[++ac_pointer]);
  } while ((length <<= 8) < AC__MinLength);        // length multiplied by 256
}

bool ArithmeticCoder::has_data()
{
    return ac_pointer < code_buffer.size() -1;
}


 
void ArithmeticCoder::encode(unsigned bit, double prob_zero)
{
  unsigned int bit_0_prob = unsigned(prob_zero * (1 << BM__LengthShift));
  unsigned x = bit_0_prob * (length >> BM__LengthShift);   // product l x p0
                                                            // update interval
  if (bit == 0)
    length  = x;
  else {
    unsigned init_base = base;
    base   += x;
    length -= x;
    if (init_base > base) propagate_carry();               // overflow = carry
  }

  if (length < AC__MinLength) renorm_enc_interval();        // renormalization
}



unsigned int ArithmeticCoder::decode(double prob_zero)
{
 
   
  unsigned int bit_0_prob = unsigned(prob_zero * (1 << BM__LengthShift));
  unsigned x = bit_0_prob * (length >> BM__LengthShift);   // product l x p0
  unsigned bit = (value >= x);                                     // decision
                                                    // update & shift interval
  if (bit == 0)
    length  = x;
  else {
    value  -= x;                                 // shifted interval base = 0
    length -= x;
  }

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  return bit;                                         // return data bit value
}
 
 
ArithmeticCoder::ArithmeticCoder()
{
    base = 0;
    value = 0;
    length = 0;
    ac_pointer = 0;
}
 
ArithmeticCoder::~ArithmeticCoder()
{
}
 
void ArithmeticCoder::start_encoder(void)
{
  base   = 0;            // initialize encoder variables: interval and pointer
  length = AC__MaxLength;
  ac_pointer = 0;                       // pointer to next data unsigned char 
}
 
void ArithmeticCoder::start_decoder(void)
{
   
  // initialize decoder: interval, pointer, initial code value
  
  length = AC__MaxLength;
  ac_pointer = 3;
  value = (unsigned(code_buffer[0]) << 24)|(unsigned(code_buffer[1]) << 16) |
          (unsigned(code_buffer[2]) <<  8)| unsigned(code_buffer[3]);
}

void ArithmeticCoder::read_from_file(string filename)
{
    ifstream is;
    is.open(filename, ios::in | ios::binary);
    is.seekg(0, is.end);
    int length = is.tellg();
    is.seekg(0, is.beg);

    code_buffer.resize(length);
    is.read((char*)code_buffer.data(), length);

    start_decoder();
}
 
unsigned ArithmeticCoder::stop_encoder(void)
{
 
  unsigned init_base = base;            // done encoding: set final data unsigned char s

  if (length > 2 * AC__MinLength) {
    base  += AC__MinLength;                                     // base offset
    length = AC__MinLength >> 1;             // set new length for 1 more unsigned char 
  }
  else {
    base  += AC__MinLength >> 1;                                // base offset
    length = AC__MinLength >> 9;            // set new length for 2 more unsigned char s
  }

  if (init_base > base) propagate_carry();                 // overflow = carry

  renorm_enc_interval();                // renormalization = output last unsigned char s

   
  return code_buffer.size();                                   // number of unsigned char s used
}
 
unsigned ArithmeticCoder::write_to_file(string filename)
{ 
    ofstream outfile(filename, ios::out | ios::binary);
    outfile.write( reinterpret_cast<const char*>(code_buffer.data()), code_buffer.size());
    return code_buffer.size();
}
 
void ArithmeticCoder::stop_decoder(void)
{
}
 