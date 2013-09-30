#include <rs_decoder.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <gnuradio/fec/rs.h>
#ifdef __cplusplus
}
#endif

generic_decoder_sptr
rs_make_decoder (unsigned int symsize, unsigned int gfpoly, unsigned int fcr, unsigned int prim, unsigned int nroots, unsigned int prepad) {
    return generic_decoder_sptr(new rs_decoder(symsize, gfpoly, fcr, prim, nroots, prepad));
}

rs_decoder::rs_decoder(unsigned int symsize, unsigned int gfpoly, unsigned int fcr, unsigned int prim, unsigned int nroots, unsigned int prepad):
    generic_decoder("rs_decoder"),
    d_symsize(symsize), 
    d_nroots(nroots),
    d_prepad(prepad),
    d_no_erasures(0),
    d_erasures(NULL)
{
    d_rs_struct = init_rs_char(symsize, gfpoly, fcr, prim, nroots);
    d_inout = buf_sptr(new unsigned char[((1 << symsize) - 1)]);
    d_insertion_point = d_inout.get() + prepad;
}

int rs_decoder::get_input_size() {
    return ((1 << d_symsize) - 1 - d_prepad);
}

int rs_decoder::get_output_size() {
    return ((1 << d_symsize) - 1 - d_prepad - d_nroots);
}

const char* rs_decoder::get_conversion() {
    return "packed";
}

const char* rs_decoder::get_output_conversion() {
    return "unpack";
}

int rs_decoder::get_input_item_size() {
    return 1;
}

void rs_decoder::generic_work(void* inBuffer, void* outBuffer) {
    memset((void*)(d_inout.get()), 0, d_prepad);
    memcpy((void*)d_insertion_point, inBuffer, ((1 << d_symsize) - 1 - d_prepad));
    int err = decode_rs_char(d_rs_struct, d_inout.get(), d_erasures, d_no_erasures);
    memcpy(outBuffer, d_insertion_point, ((1 << d_symsize) - 1 - d_prepad - d_nroots));
}

rs_decoder::~rs_decoder()
{
    free_rs_char(d_rs_struct);
}
