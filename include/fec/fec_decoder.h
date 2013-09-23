#ifndef INCLUDED_FEC_DECODER_H
#define INCLUDED_FEC_DECODER_H

#include <gnuradio/block.h>
#include <fec/fec_api.h>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <fec/generic_decoder.h>



class fec_decoder;
typedef boost::shared_ptr<fec_decoder> fec_decoder_sptr;
typedef boost::shared_array<unsigned char> buf_sptr;
FEC_API fec_decoder_sptr fec_make_decoder(generic_decoder_sptr my_decoder, size_t input_item_size, size_t output_item_size);

class FEC_API fec_decoder : public gr::block
{
  friend fec_decoder_sptr fec_make_decoder(generic_decoder_sptr my_decoder, size_t input_item_size, size_t output_item_size);
  fec_decoder(generic_decoder_sptr my_decoder, size_t input_item_size, size_t output_item_size);
  generic_decoder_sptr d_decoder;
  size_t d_input_item_size;
  size_t d_output_item_size;
  buf_sptr d_inbuf;
  const unsigned char* d_starts[2];
 public:
  int general_work (int noutput_items,
		    gr_vector_int& ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
  int fixed_rate_ninput_to_noutput(int ninput);
  int fixed_rate_noutput_to_ninput(int noutput);
  void forecast(int noutput_items,
		gr_vector_int& ninput_items_required);
};



    
#endif /* INCLUDED_FEC_DECODER_H */
