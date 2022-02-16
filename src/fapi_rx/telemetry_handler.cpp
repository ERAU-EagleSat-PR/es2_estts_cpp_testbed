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

telemetry_handler::telemetry_handler(std::vector<estts::estts_command *> command) {
    this->command = command;
}

std::vector<estts::telemetry_object *> telemetry_handler::process_command() {
    if (command[0]->address == estts::es2_endpoint::ES_EPS)
        return process_eps_command(command);

    return {};
}

std::vector<estts::telemetry_object *> telemetry_handler::grab_telem_stream() {
    std::vector<estts::telemetry_object *> telemetry;

    for (int i = 0; i < 20; i++) {
        auto telem = new estts::telemetry_object;
        telem->commandID = estts::es2_commands::eps::EPS_GET_HEALTH;
        telem->response_code = 00;
        telem->address = estts::es2_endpoint::ES_EPS;
        telem->sequence = 00;
        telem->timeStamp = 8457;
        telemetry.push_back(telem);
    }

    return telemetry;
}

telemetry_handler::telemetry_handler() = default;
