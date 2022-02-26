//
// Created by Taylor Hancock on 02/11/2022.
//

#ifndef ES2_ESTTS_CPP_TESTBED_ACS_H
#define ES2_ESTTS_CPP_TESTBED_ACS_H

#include <vector>
#include "constants.h"

std::vector<estts::telemetry_object *> process_acs_command(const std::vector<estts::estts_command *> &command);

std::vector<estts::telemetry_object *> process_deploy_mag_boom();

std::vector<estts::telemetry_object *> process_enable_acs();

std::vector<estts::telemetry_object *> process_power_acs();

std::vector<estts::telemetry_object *> process_set_ctrl_mode();

std::vector<estts::telemetry_object *> process_set_est_mode();

std::vector<estts::telemetry_object *> process_est_attitude_ang();

#endif //ES2_ESTTS_CPP_TESTBED_ACS_H
