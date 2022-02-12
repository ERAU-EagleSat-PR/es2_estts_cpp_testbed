//
// Created by Hayden Roszell on 1/3/22.
//

#ifndef ES2_ESTTS_CPP_TESTBED_EPS_H
#define ES2_ESTTS_CPP_TESTBED_EPS_H

#include <vector>
#include "constants.h"

std::vector<estts::telemetry_object *> process_eps_command(const std::vector<estts::estts_command *> &command);

std::vector<estts::telemetry_object *> process_get_health();

std::vector<estts::telemetry_object *> process_get_voltage();

std::vector<estts::telemetry_object *> process_get_battery_current();

#endif //ES2_ESTTS_CPP_TESTBED_EPS_H