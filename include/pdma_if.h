#ifndef _PDMA_IF_H_
#define _PDMA_IF_H_

class pdma_if {
public:
    virtual bool is_stalled_pdma(int tid, uint32_t addr, int num_words) = 0;
    virtual vector<uint32_t> read_pdma(int tid, uint32_t start_addr, bool& stalled, int num_words) = 0;
    virtual ~pdma_if() {};
};

#endif
