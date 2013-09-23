#ifndef INCLUDED_GENERIC_DECODER_H
#define INCLUDED_GENERIC_DECODER_H


#include <fec/fec_api.h>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/format.hpp>


class FEC_API generic_decoder {
 protected:
    
    static int base_unique_id;
    int my_id;
    int unique_id();
    std::string d_name;
    std::string alias(){ return (boost::format("%s%d")%d_name%unique_id()).str(); }

 public:
    virtual void generic_work(void *inBuffer, void *outBuffer) = 0;
    virtual int get_input_size() = 0;
    virtual int get_output_size() = 0;
    virtual int get_history();
    virtual float get_shift();
    virtual const char* get_conversion();
    virtual int get_input_item_size();
    virtual int get_output_item_size();
    virtual const char* get_output_conversion();
    virtual int destructive();
    virtual ~generic_decoder();
    generic_decoder(std::string name);
    
};

typedef boost::shared_ptr<generic_decoder> generic_decoder_sptr;



FEC_API int fec_get_decoder_output_size(generic_decoder_sptr my_decoder);
FEC_API int fec_get_decoder_input_size(generic_decoder_sptr my_decoder);
FEC_API float fec_get_shift(generic_decoder_sptr my_decoder);
FEC_API int fec_get_history(generic_decoder_sptr my_decoder);
FEC_API int fec_get_decoder_output_item_size(generic_decoder_sptr my_decoder);
FEC_API int fec_get_decoder_input_item_size(generic_decoder_sptr my_decoder);
FEC_API const char* fec_get_conversion(generic_decoder_sptr my_decoder);
FEC_API const char* fec_get_output_conversion(generic_decoder_sptr my_decoder);







    
#endif /* INCLUDED_GENERIC_DECODER_H */
