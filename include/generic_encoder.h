#ifndef INCLUDED_GENERIC_ENCODER_H
#define INCLUDED_GENERIC_ENCODER_H


#include <fec_api.h>
#include <boost/shared_ptr.hpp>
#include <vector>



class FEC_API generic_encoder{
 public:
    virtual void generic_work(void *inBuffer, void *outBuffer) = 0;
    virtual int get_input_size() = 0;
    virtual int get_output_size() = 0;
    virtual ~generic_encoder();
 
    
};

typedef boost::shared_ptr<generic_encoder> generic_encoder_sptr;




FEC_API int fec_get_encoder_output_size(generic_encoder_sptr my_encoder);
FEC_API int fec_get_encoder_input_size(generic_encoder_sptr my_encoder);






    
#endif /* INCLUDED_GENERIC_ENCODER_H */
