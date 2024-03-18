#include "commands.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(CtlDiscovery     , "69d7bd49-e0e4-4001-b20c-c24142cfb1f3")
REGISTRY_COMMAND_SINGLPROC(ServerInformation, "434cb1fc-7f02-46de-b946-0ef3b24beae1")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

bserial::RawVector CtlDiscovery::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << info;
    stream << applId;
    stream << hostPoint;
    B_SERIALIZE_RETURN
}
void CtlDiscovery::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> info;
    stream >> applId;
    stream >> hostPoint;
    B_DESERIALIZE_END
}

bserial::RawVector ServerInformation::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << HelloWorld;
    stream << name;
    stream << address;
    B_SERIALIZE_RETURN
}

void ServerInformation::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> HelloWorld;
    stream >> name;
    stream >> address;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
