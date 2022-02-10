//
// Created by Hayden Roszell on 1/3/22.
//

#include "eps.h"

std::vector<estts::telemetry_object *> process_eps_command(const std::vector<estts::estts_command *> &command) {
    if (command[0]->commandID == estts::es2_commands::eps::EPS_GET_HEALTH)
        return process_get_health();
    else if (command[0]->commandID == estts::es2_commands::eps::EPS_VOLTAGE)
        return process_get_voltage();
}

std::vector<estts::telemetry_object *> process_get_health() {
    auto telem = new estts::telemetry_object;

    telem->commandID = estts::es2_commands::eps::EPS_GET_HEALTH;
    telem->response_code = 00;
    telem->address = estts::es2_endpoint::ES_EPS;
    telem->sequence = 00;
    telem->timeStamp = 8457;

    std::vector<estts::telemetry_object *> telemetry;

    telemetry.push_back(telem);
    return telemetry;
}

std::vector<estts::telemetry_object *> process_get_voltage() {
    auto telem = new estts::telemetry_object;

    telem->commandID = estts::es2_commands::eps::EPS_VOLTAGE;
    telem->response_code = 00;
    telem->address = estts::es2_endpoint::ES_EPS;
    telem->sequence = 00;
    telem->timeStamp = 8457;

    std::vector<estts::telemetry_object *> telemetry;

    telemetry.push_back(telem);
    return telemetry;
}