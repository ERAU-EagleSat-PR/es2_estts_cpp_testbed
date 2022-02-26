//
// Created by Hayden Roszell on 1/3/22.
//

#include <chrono>
#include <thread>
#include <string>
#include "spdlog/spdlog.h"
#include "telemetry_handler.h"
#include "frame_constructor.h"
#include "constants.h"
#include "eps.h"
#include "acs.h"

telemetry_handler::telemetry_handler(std::vector<estts::estts_command *> command) {
    this->command = command;
}

std::vector<estts::telemetry_object *> telemetry_handler::process_command() {
    if (command[0]->address == estts::es2_endpoint::ES_EPS)
        return process_eps_command(command);
    if (command[0]->address == estts::es2_endpoint::ES_ACS)
        return process_acs_command(command);

    return {};
}


