#ifndef POOLED_DECODER_H
#define POOLED_DECODER_H

#include<fec/generic_decoder.h>
#include<fec/pooled_resource.h>
#include<tuple>

template<class T, class IDX=int>
    class pooled_decoder {
 public:
 //required: static gene T::make(IDX)
    pooled_decoder(int max=8):
    d_decoders(boost::bind(&T::make, _1), max) 
        {}
    decode(void* inBuffer, void* outBuffer, int len, IDX idx) {
        generic_decoder* dec = d_decoders.acquire(idx);
        dec->set_framebits(len);
        dec->generic_work(inBuffer, outBuffer);
        d_decoders.release(idx, dec);
    }


 private:
    managed_resource_pool<T, IDX> d_decoders;
};



#endif /*POOLED_DECODER_H*/
