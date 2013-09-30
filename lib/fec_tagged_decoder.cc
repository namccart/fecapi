#include <fec_tagged_decoder.h>
#include <gnuradio/io_signature.h>
#include <stdio.h>


fec_tagged_decoder_sptr
fec_make_tagged_decoder(generic_decoder_sptr my_decoder, size_t max_frame, const std::string& lengthtagname)
{
    return gnuradio::get_initial_sptr( new fec_tagged_decoder(my_decoder, max_frame, lengthtagname));
}


fec_tagged_decoder::fec_tagged_decoder(generic_decoder_sptr my_decoder, size_t max_frame, const std::string& lengthtagname)
  : gr::block("fec_tagged_decoder",
              gr::io_signature::make(1, 1, my_decoder->get_input_item_size()),
              gr::io_signature::make(1, 1, my_decoder->get_output_item_size())),
    d_input_item_size(my_decoder->get_input_item_size()), 
    d_output_item_size(my_decoder->get_output_item_size()),
    d_max_frame(max_frame),
    d_inbuf_size(0),
    d_tag(pmt::mp(lengthtagname))
{
    set_history(d_decoder->get_history() + 1);          
    d_state=SYNC;
    set_relative_rate((double)(my_decoder->get_output_size())/my_decoder->get_input_size());
    set_min_output_buffer(relative_rate() * d_max_frame);
    d_inbuf = buf_sptr(new unsigned char[(d_max_frame + my_decoder->get_history())*d_input_item_size]);
    d_decoder = my_decoder;
    
    
}
          

bool
fec_tagged_decoder::guarded_acquire(int num, const void* source) {
    if (d_inbuf_size + num > d_max_frame) {
        d_inbuf_size = 0;
        d_state = SYNC;
        return false;
    }
    else {
        memcpy((void*)(d_inbuf.get() + d_inbuf_size), source, num); 
        d_inbuf_size += num;
        return true;
    }
}

void 
fec_tagged_decoder::forecast(int noutput_items, 
		      gr_vector_int& ninput_items_required) {
    ninput_items_required[0] = (int)(((d_decoder->get_input_size()/(double)d_decoder->get_output_size()) * noutput_items) + .5);
    return;
}

          
uint64_t 
fec_tagged_decoder::get_offset(std::vector<tag_t>::iterator tag_itr) {
    return ((tag_itr->offset - nitems_read(0)) * relative_rate()) + nitems_written(0);
}

void
fec_tagged_decoder::pass_current_tags(int start, int end) {
    std::vector<tag_t> tags;
    std::vector<tag_t>::iterator tag_itr;
    get_tags_in_range(tags, 0, nitems_read(0) + (start/d_input_item_size), nitems_read(0) +(end/d_input_item_size));
    for (tag_itr = tags.begin(); tag_itr != tags.end(); tag_itr++) {
        add_item_tag(0, get_offset(tag_itr), tag_itr->key, tag_itr->value);
    }
}

int
fec_tagged_decoder::general_work (int noutput_items,
			   gr_vector_int& ninput_items,
			   gr_vector_const_void_star &input_items,
			   gr_vector_void_star &output_items)
{
    const unsigned char *inBuffer = (unsigned char*) input_items[0];
    unsigned char *outBuffer = (unsigned char *)output_items[0];
    int consume = 0;
    int ret = 0;
    std::vector<tag_t> tags;
    std::vector<tag_t>::iterator tag_itr;
    while(consume < ninput_items[0] * d_input_item_size) {
        switch(d_state) {
        case SYNC:
            
            get_tags_in_range(tags, 0, nitems_read(0) + (consume/d_input_item_size), nitems_read(0) +ninput_items[0], d_tag);
            if (tags.size() == 0) {
                consume = ninput_items[0] * d_input_item_size;
            }
            else {
                tag_itr = tags.begin(); 
                consume = (tag_itr->offset - nitems_read(0)) * d_input_item_size;
                d_state = ACQUIRE;
                    
            }
            tags.clear();
            break;
            
        case ACQUIRE:
            get_tags_in_range(tags, 0, nitems_read(0) + (consume/d_input_item_size), nitems_read(0) +ninput_items[0], d_tag);
            if (tags.size() == 0) {
                if(guarded_acquire((ninput_items[0] * d_input_item_size) - consume, inBuffer + consume)) {
                    pass_current_tags(consume, ninput_items[0] * d_input_item_size);
                }
                consume = ninput_items[0] * d_input_item_size;
                
            }
            else {
                tag_itr = tags.begin();
                if(guarded_acquire(((tag_itr->offset - nitems_read(0)) * d_input_item_size) - consume, inBuffer + consume)) {
                    pass_current_tags(consume, (tag_itr->offset - nitems_read(0)) * d_input_item_size);
                    d_state=POST;
                }
                consume = (tag_itr->offset - nitems_read(0)) * d_input_item_size;
               
            }
             
            tags.clear();
            break;

        case POST:
            if (noutput_items >= relative_rate() * (d_inbuf_size/d_input_item_size)) {
                d_decoder->set_framebits(d_inbuf_size/d_input_item_size);
                d_decoder->generic_work((void*) d_inbuf.get(), (void*) (outBuffer + ret));
                ret += (relative_rate() * (d_inbuf_size/d_input_item_size) * d_output_item_size);
                d_inbuf_size = 0;
                d_state=ACQUIRE;
            }
            else {
                //not enough room in output_buffer... exit work!
                consume_each(consume/d_input_item_size);
                return (ret/d_output_item_size);
            }
            break;
        }
    }
    
    consume_each(consume/d_input_item_size);
    return (ret/d_output_item_size);

}


