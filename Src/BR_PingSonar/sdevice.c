#include "BR_PingSonar/core.h"
#include "BR_PingSonar/Internal/messaging.h"

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(BR_PingSonar, handle)
{
    handle->Dynamic.ResponcePayloadData = handle->Constant->ReceiveBuffer + sizeof(BR_PingSonarMessagePreamble);
    handle->Dynamic.RequestPayloadData = handle->Constant->TransmitBuffer + sizeof(BR_PingSonarMessagePreamble);
    return true;
}
