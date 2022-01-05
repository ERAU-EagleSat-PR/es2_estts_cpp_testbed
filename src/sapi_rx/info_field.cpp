//
// Created by Cody on 12/17/2021.
//

#include <iostream>
#include <string>
#include <sstream>
#include "info_field.h"
#include "spdlog/spdlog.h"

using std::cerr;
using std::endl;
using std::string;

/**
 * @description Encodes the Address section of the AX.25 Information Field
 * @return string of bits (e.g. "10010101")
 */
string info_field::getAddress() {
    SPDLOG_TRACE("Setting info field address to {}", this->telemetry->address);
    auto address = std::to_string(this->telemetry->address);
    if (this->telemetry->sequence < 10)
        address.insert(0, "0");
    return address;
}

/**
 * @description Encodes the Timestamp section of the AX.25 Information Field
 * @return string of bits (e.g. "10010101")
 */
string info_field::getTimeStamp() {
    SPDLOG_TRACE("Setting info field timestamp to {}", this->telemetry->timeStamp);
    return std::to_string(this->telemetry->timeStamp);
}

/**
 * @description Encodes the Sequence section of the AX.25 Information Field
 * @return string of bits (e.g. "10010101")
 */
string info_field::getSequence() {
    SPDLOG_TRACE("Setting info field frame sequence to {}", this->telemetry->sequence);
    auto sequence = std::to_string(this->telemetry->sequence);
    if (this->telemetry->sequence < 10)
        sequence.insert(0, "0");
    return sequence;
}

/**
 * @description Encodes the Command ID section of the AX.25 Information Field
 * @return string of bits (e.g. "10010101")
 */
string info_field::getCommandID() {
    SPDLOG_TRACE("Setting info field command ID to {}", this->telemetry->commandID);
    auto command_id = std::to_string(this->telemetry->commandID);
    if (this->telemetry->commandID < 10)
        command_id.insert(0, "0");
    return command_id;
}

/**
 * @description Encodes the Method section of the AX.25 Information Field
 * @return One bit as a char (e.g. "1")
 */
string info_field::getRespCode() {
    SPDLOG_TRACE("Setting info field response code to {}", this->telemetry->response_code);
    auto method = std::to_string(this->telemetry->response_code);
    return method;
}

/**
 * @description Encodes the Data section of the AX.25 Information Field
 * @return string of bits (e.g. "10010101...")
 */
string info_field::getData() {
    if (this->telemetry->data != nullptr) {
        SPDLOG_TRACE("Setting info field data to {}", this->telemetry->data);
        return reinterpret_cast<char const *>(this->telemetry->data);
    } else return "";
}

/**
 * @description Retrieves the encodings of the entire AX.25 Information Field
 * @return string of bits (e.g. "10010101...")
 */
string info_field::build_info_field() {
    std::stringstream infoFieldStream;
    infoFieldStream << getAddress() << getTimeStamp() << getSequence() << getCommandID() << getRespCode() << getData();
    SPDLOG_DEBUG("SAPI Info Field encoded to {}", infoFieldStream.str());

    return infoFieldStream.str();
}

estts::command_object *info_field::build_command_object(std::string info_field) {
    auto comm = new estts::command_object;

    int address;
    std::istringstream(info_field.substr(0, 2)) >> address;
    comm->address = address;

    int timestamp;
    std::istringstream(info_field.substr(2, 4)) >> timestamp;
    comm->timeStamp = timestamp;

    int sequence;
    std::istringstream(info_field.substr(6, 2)) >> sequence;
    comm->sequence = sequence;

    int command_id;
    std::istringstream(info_field.substr(8, 2)) >> command_id;
    comm->commandID = command_id;

    int method;
    std::istringstream(info_field.substr(10, 1)) >> method;
    comm->method = method;

    // resp->data = info_field.substr(12).c_str(); todo figure this shit out

    SPDLOG_TRACE("Frame info field decoded successfully. Command object stored at {}",
                  static_cast<const void *>(comm));

    return comm;
}
