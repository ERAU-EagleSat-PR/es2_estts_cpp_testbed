//
// Created by Taylor Hancock on 02/11/2022.
//

#include "acs.h"


std::vector<estts::telemetry_object *> process_acs_command(const std::vector<estts::estts_command *> &command) {
    if (command[0]->commandID == estts::es2_commands::acs::ACS_DEP_MAG_BOOM)
        return process_deploy_mag_boom();
    if (command[0]->commandID == estts::es2_commands::acs::ACS_ENABLE)
        return process_enable_acs();
    if (command[0]->commandID == estts::es2_commands::acs::ACS_POWER)
        return process_power_acs();
    if (command[0]->commandID == estts::es2_commands::acs::ACS_SET_CTRL_MODE)
        return process_set_ctrl_mode();
    if (command[0]->commandID == estts::es2_commands::acs::ACS_SET_EST_MODE)
        return process_set_est_mode();
}

std::vector<estts::telemetry_object *> process_deploy_mag_boom() {
    auto telem = new estts::telemetry_object;

    telem->commandID = estts::es2_commands::acs::ACS_DEP_MAG_BOOM;
    telem->response_code = 00;
    telem->address = estts::es2_endpoint::ES_ACS;
    telem->sequence = 00;
    telem->timeStamp = 1729;

    std::vector<estts::telemetry_object *> telemetry;

    telemetry.push_back(telem);
    return telemetry;
}

std::vector<estts::telemetry_object *> process_enable_acs() {
    auto telem = new estts::telemetry_object;

    telem->commandID = estts::es2_commands::acs::ACS_ENABLE;
    telem->response_code = 00;
    telem->address = estts::es2_endpoint::ES_ACS;
    telem->sequence = 00;
    telem->timeStamp = 1729;

    std::vector<estts::telemetry_object *> telemetry;

    telemetry.push_back(telem);
    return telemetry;
}

std::vector<estts::telemetry_object *> process_power_acs() {
    auto telem = new estts::telemetry_object;

    telem->commandID = estts::es2_commands::acs::ACS_POWER;
    telem->response_code = 00;
    telem->address = estts::es2_endpoint::ES_ACS;
    telem->sequence = 00;
    telem->timeStamp = 1729;

    std::vector<estts::telemetry_object *> telemetry;

    telemetry.push_back(telem);
    return telemetry;
}

std::vector<estts::telemetry_object *> process_set_ctrl_mode() {
    auto telem = new estts::telemetry_object;

    telem->commandID = estts::es2_commands::acs::ACS_SET_CTRL_MODE;
    telem->response_code = 00;
    telem->address = estts::es2_endpoint::ES_ACS;
    telem->sequence = 00;
    telem->timeStamp = 1729;

    std::vector<estts::telemetry_object *> telemetry;

    telemetry.push_back(telem);
    return telemetry;
}

std::vector<estts::telemetry_object *> process_set_est_mode() {
    auto telem = new estts::telemetry_object;

    telem->commandID = estts::es2_commands::acs::ACS_SET_EST_MODE;
    telem->response_code = 00;
    telem->address = estts::es2_endpoint::ES_ACS;
    telem->sequence = 00;
    telem->timeStamp = 1729;

    std::vector<estts::telemetry_object *> telemetry;

    telemetry.push_back(telem);
    return telemetry;
}
