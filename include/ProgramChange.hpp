#ifndef PROGRAM_CHANGE_HPP_
#define PROGRAM_CHANGE_HPP_

#include "Event.hpp"

class ProgramChange : public ProgramChangeData {
public:
    ProgramChangeEvent get_event(Duration when, unsigned char channel) const {
        return {when, channel, pc_num()};
    }
};

#endif // PROGRAM_CHANGE_HPP_
