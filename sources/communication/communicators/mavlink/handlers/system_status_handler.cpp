#include "system_status_handler.h"

// MAVLink
#include <mavlink.h>

// Internal
#include "mavlink_protocol_helpers.h"

#include "telemetry.h"

using namespace comm;
using namespace domain;

SystemStatusHandler::SystemStatusHandler(TelemetryService* telemetryService,
                                         MavLinkCommunicator* communicator):
    AbstractMavLinkHandler(communicator),
    m_telemetryService(telemetryService)
{}

void SystemStatusHandler::processMessage(const mavlink_message_t& message)
{
    if (message.msgid != MAVLINK_MSG_ID_SYS_STATUS) return;

    TelemetryNode* node = m_telemetryService->nodeByMavId(message.sysid);
    if (!node) return;

    mavlink_sys_status_t status;
    mavlink_msg_sys_status_decode(&message, &status);

    node->setParameter({ telemetry::ahrs, telemetry::present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_AHRS);
    node->setParameter({ telemetry::ahrs, telemetry::enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_AHRS);
    node->setParameter({ telemetry::ahrs, telemetry::operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_AHRS);

    node->setParameter({ telemetry::satellite, telemetry::present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_SENSOR_GPS);
    node->setParameter({ telemetry::satellite, telemetry::enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_SENSOR_GPS);
    node->setParameter({ telemetry::satellite, telemetry::operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_GPS);

    node->setParameter({ telemetry::compass, telemetry::present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_SENSOR_3D_MAG);
    node->setParameter({ telemetry::compass, telemetry::enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_SENSOR_3D_MAG);
    node->setParameter({ telemetry::compass, telemetry::operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_3D_MAG);

    node->setParameter({ telemetry::barometric, telemetry::present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE);
    node->setParameter({ telemetry::barometric, telemetry::enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE);
    node->setParameter({ telemetry::barometric, telemetry::operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE);

    node->setParameter({ telemetry::pitot, telemetry::present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE);
    node->setParameter({ telemetry::pitot, telemetry::enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE);
    node->setParameter({ telemetry::pitot, telemetry::operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE);

    node->notify();

// TODO: MAV_SYS_STATUS_SENSOR_3D_ACCEL, MAV_SYS_STATUS_SENSOR_3D_GYRO, MAV_SYS_STATUS_SENSOR_LASER_POSITION
// TODO: battery
}