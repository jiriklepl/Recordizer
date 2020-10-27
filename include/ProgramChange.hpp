#ifndef PROGRAM_CHANGE_HPP_
#define PROGRAM_CHANGE_HPP_

#include "NoteEvent.hpp"

class ProgramChange : public ProgramChangeData {
public:
    ProgramChangeEvent get_event(Duration when, unsigned char channel) {
        return {when, channel, pc_num()};
    }
};

#endif // PROGRAM_CHANGE_HPP_
