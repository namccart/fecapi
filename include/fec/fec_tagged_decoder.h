#ifndef INCLUDED_FEC_TAGGED_DECODER_H
#define INCLUDED_FEC_TAGGED_DECODER_H

#include <gnuradio/block.h>
#include <fec/fec_api.h>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/format.hpp>
#include <fec/generic_decoder.h>

using gr::tag_t;

class fec_tagged_decoder;
typedef boost::shared_ptr<fec_tagged_decoder> fec_tagged_decoder_sptr;
typedef boost::shared_array<unsigned char> buf_sptr;
FEC_API fec_tagged_decoder_sptr fec_make_tagged_decoder(generic_decoder_sptr my_decoder, size_t max_frame, const std::string& lengthtagname);

class FEC_API fec_tagged_decoder : public gr::block
{
  friend fec_tagged_decoder_sptr fec_make_tagged_decoder(generic_decoder_sptr my_decoder, size_t max_frame, const std::string& lengthtagname);
  fec_tagged_decoder(generic_decoder_sptr my_decoder, size_t max_frame, const std::string& lengthtagname);
  generic_decoder_sptr d_decoder;
  size_t d_input_item_size;
  size_t d_output_item_size;
  size_t d_max_frame;
  buf_sptr d_inbuf;
  size_t d_inbuf_size;
  unsigned int d_next_size;
  unsigned int d_state;
  pmt::pmt_t d_tag;
  enum {SYNC,ACQUIRE,OVERLAP,POST};
  bool guarded_acquire(int num, const void* source);
  uint64_t get_offset(std::vector<tag_t>::iterator tag_itr);
  void pass_current_tags(int consume, int ret);
 public:
  void forecast(int noutput_items,
                gr_vector_int& ninput_items_required);
  int general_work (int noutput_items,
                    gr_vector_int& ninput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items);
  
  
};



    
#endif /* INCLUDED_FEC_TAGGED_DECODER_H */
