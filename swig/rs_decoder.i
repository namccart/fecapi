/* -*- c++ -*- */

%{
#include "rs_decoder.h"
%}





generic_decoder_sptr rs_make_decoder(unsigned int symsize, unsigned int gfpoly, unsigned int fcr, unsigned int prim, unsigned int nroots, unsigned int prepad);

class rs_decoder {
 public:
    ~rs_decoder();
 private:
    rs_decoder(unsigned int symsize, unsigned int gfpoly, unsigned int fcr, unsigned int prim, unsigned int nroots, unsigned int prepad);
};
