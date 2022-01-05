//
// Created by Cody on 12/17/2021.
//

#ifndef ES2_ESTTS_CPP_INFOFIELD_H
#define ES2_ESTTS_CPP_INFOFIELD_H

#include <string>
#include <unordered_map>
#include <utility>
#include "constants.h"

class info_field {
private:
    /* Getters */
    std::string getData();

    std::string getAddress();

    std::string getTimeStamp();

    std::string getSequence();

    std::string getCommandID();

    std::string getRespCode();

protected:
    estts::telemetry_object *telemetry;

    /* Encoded Information Field Getter */
    std::string build_info_field();

    estts::command_object *build_command_object(std::string info_field);

    explicit info_field() : telemetry(nullptr) {}

    explicit info_field(estts::telemetry_object *telemetry) : telemetry(telemetry) {}
};

#endif