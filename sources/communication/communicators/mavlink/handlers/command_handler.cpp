#include "command_handler.h"

// MAVLink
#include <mavlink.h>

// Qt
#include <QDebug>

// Internal
#include "service_registry.h"

#include "command_service.h"
#include "command.h"

#include "vehicle_service.h"
#include "vehicle.h"

#include "mavlink_communicator.h"

using namespace comm;
using namespace domain;

namespace
{
    const QMap<quint16, Command::CommandType> mavCommandMap =
    {
        { MAV_CMD_COMPONENT_ARM_DISARM, Command::ArmDisarm },
        { MAV_CMD_NAV_RETURN_TO_LAUNCH, Command::Return },
        { MAV_CMD_MISSION_START, Command::Start },
        { MAV_CMD_DO_PAUSE_CONTINUE, Command::PauseContinue },
        { MAV_CMD_DO_PARACHUTE, Command::Parachute }
    };
}

CommandHandler::CommandHandler(MavLinkCommunicator* communicator):
    AbstractMavLinkHandler(communicator),
    m_commandService(ServiceRegistry::commandService()),
    m_vehicleService(ServiceRegistry::vehicleService())
{
    connect(m_commandService, &CommandService::gotCommand, this, &CommandHandler::onGotCommand);
}

void CommandHandler::processMessage(const mavlink_message_t& message)
{
    Q_UNUSED(message) // TODO: handle command quitantion
}

void CommandHandler::onGotCommand()
{
    while (m_commandService->hasCommand())
    {
        this->sendCommand(m_commandService->headCommand());
    }
}

void CommandHandler::sendCommand(const Command& command)
{
    mavlink_message_t message;
    mavlink_command_long_t mavCommand;

    dao::VehiclePtr vehicle = m_vehicleService->vehicle(command.vehicleId());
    if (vehicle.isNull()) return;

    mavCommand.target_system = vehicle->mavId();
    mavCommand.target_component = 0;
    mavCommand.confirmation = 0;
    mavCommand.command = ::mavCommandMap.key(command.type());

    QVariantList args = command.arguments();
    if (args.count() > 0) mavCommand.param1 = args.at(0).toFloat();
    if (args.count() > 1) mavCommand.param2 = args.at(1).toFloat();
    if (args.count() > 2) mavCommand.param3 = args.at(2).toFloat();
    if (args.count() > 3) mavCommand.param4 = args.at(3).toFloat();
    if (args.count() > 4) mavCommand.param5 = args.at(4).toFloat();
    if (args.count() > 5) mavCommand.param6 = args.at(5).toFloat();
    if (args.count() > 6) mavCommand.param7 = args.at(6).toFloat();

    AbstractLink* link = m_communicator->mavSystemLink(vehicle->mavId());
    if (!link) return;

    mavlink_msg_command_long_encode_chan(m_communicator->systemId(),
                                    m_communicator->componentId(),
                                    m_communicator->linkChannel(link),
                                    &message, &mavCommand);
    m_communicator->sendMessage(message, link);
}
