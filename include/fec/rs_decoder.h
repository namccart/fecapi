#ifndef INCLUDED_RS_DECODER_H
#define INCLUDED_RS_DECODER_H


#include <map>
#include <string>
#include <fec/generic_decoder.h>
#include <fec/cc_common.h>
#include <boost/shared_array.hpp>

typedef boost::shared_array<unsigned char> buf_sptr;

FEC_API generic_decoder_sptr
rs_make_decoder (unsigned int symsize, unsigned int gfpoly, unsigned int fcr, unsigned int prim, unsigned int nroots, unsigned int prepad);


class FEC_API rs_decoder : public generic_decoder
{
 protected:    
    
    //befriend the global, swigged make function
    friend generic_decoder_sptr
	rs_make_decoder (unsigned int symsize, unsigned int gfpoly, unsigned int fcr, unsigned int prim, unsigned int nroots, unsigned int prepad);

    //plug into the generic fec api
    int get_output_size();
    int get_input_size();
    int get_input_item_size();
    const char* get_conversion();
    const char* get_output_conversion();
    //everything else...
    unsigned int d_symsize;
    unsigned int d_nroots;
    unsigned int d_prepad;
    void *d_rs_struct;
    buf_sptr d_inout;
    unsigned char *d_insertion_point;
    int *d_erasures;
    int d_no_erasures;
    
    


 public:
    rs_decoder(unsigned int symsize, unsigned int gfpoly, unsigned int fcr, unsigned int prim, unsigned int nroots, unsigned int prepad);
    void generic_work(void *inBuffer, void *outbuffer);
    ~rs_decoder ();
    
    
    
};

#endif /* INCLUDED_RS_DECODER_H */
