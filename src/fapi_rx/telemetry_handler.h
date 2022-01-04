//
// Created by Hayden Roszell on 1/3/22.
//

#ifndef ES2_ESTTS_CPP_TESTBED_TELEMETRY_HANDLER_H
#define ES2_ESTTS_CPP_TESTBED_TELEMETRY_HANDLER_H

#include <vector>
#include "constants.h"

class telemetry_handler {
private:
    std::vector<estts::estts_command *> command;

public:
    explicit telemetry_handler(std::vector<estts::estts_command *> command);

    std::vector<estts::telemetry_object *> process_command();
};


#endif //ES2_ESTTS_CPP_TESTBED_TELEMETRY_HANDLER_H
