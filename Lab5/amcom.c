#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "amcom.h"
#include <stdbool.h>
/// Start of packet character
const uint8_t  AMCOM_SOP         = 0xA1;
const uint16_t AMCOM_INITIAL_CRC = 0xFFFF;

static uint16_t AMCOM_UpdateCRC(uint8_t byte, uint16_t crc)
{
	byte ^= (uint8_t)(crc & 0x00ff);
	byte ^= (uint8_t)(byte << 4);
	return ((((uint16_t)byte << 8) | (uint8_t)(crc >> 8)) ^ (uint8_t)(byte >> 4) ^ ((uint16_t)byte << 3));
}


void AMCOM_InitReceiver(AMCOM_Receiver* receiver, AMCOM_PacketHandler packetHandlerCallback, void* userContext) {
	receiver->payloadCounter = 0;
    receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
    receiver->packetHandler = packetHandlerCallback;
    receiver->userContext = userContext;
}

size_t AMCOM_Serialize(uint8_t packetType, const void* payload, size_t payloadSize, uint8_t* destinationBuffer) {
    if(payloadSize > 200){
        return 0;
    }
    destinationBuffer[0] = AMCOM_SOP;
    destinationBuffer[1] = packetType;
    destinationBuffer[2] = payloadSize;
    uint16_t crc = AMCOM_INITIAL_CRC;
    crc = AMCOM_UpdateCRC(packetType, crc);
    crc = AMCOM_UpdateCRC(payloadSize, crc);
    for(int i = 0; i < payloadSize; i++){
        crc = AMCOM_UpdateCRC(((uint8_t*)payload)[i], crc);
    }
    destinationBuffer[3] = *(uint8_t*)&crc;
    destinationBuffer[4] = *((uint8_t*)&crc + 1);
    size_t count = 5;
    if(payload == NULL){
        return count;
    }
    for(int i = 0; i < payloadSize; i++){
        destinationBuffer[5 + i] = ((uint8_t*)payload)[i];
        ++count;
    }
	return count;
}

void AMCOM_Deserialize(AMCOM_Receiver* receiver, const void* data, size_t dataSize) {
    //puts("");
    size_t counter = 0;
    bool callFunction = false;
    while(counter < dataSize){
        uint8_t byte = ((uint8_t*)data)[counter];
        //printf("Received byte: %d = 0x%0X\n", byte, byte);
        if(receiver->receivedPacketState == AMCOM_PACKET_STATE_EMPTY){
            if(byte == AMCOM_SOP){
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_SOP;
                receiver->receivedPacket.header.sop = AMCOM_SOP;
            }
        }
        else if(receiver->receivedPacketState == AMCOM_PACKET_STATE_GOT_SOP){
            if(byte > 200){
                receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
                break;
            }
            else{
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_TYPE;
                receiver->receivedPacket.header.type = byte;
            }
        }
        else if(receiver->receivedPacketState == AMCOM_PACKET_STATE_GOT_TYPE){
            receiver->receivedPacket.header.length = byte;
            if(byte > AMCOM_MAX_PAYLOAD_SIZE){
                receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
                break;
            }
            else{
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_LENGTH;
            }
        }
        else if(receiver->receivedPacketState == AMCOM_PACKET_STATE_GOT_LENGTH){
            receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_CRC_LO;
            receiver->receivedPacket.header.crc = byte;
        }
        else if(receiver->receivedPacketState == AMCOM_PACKET_STATE_GOT_CRC_LO){
            receiver->receivedPacket.header.crc |= byte << 8;
            receiver->payloadCounter = 0;
            if(receiver->receivedPacket.header.length == 0){
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
            }
            else{
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GETTING_PAYLOAD;
            }
        }
        else if(receiver->receivedPacketState == AMCOM_PACKET_STATE_GETTING_PAYLOAD){
            if(receiver->payloadCounter == receiver->receivedPacket.header.length){
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
            }
            else{
                receiver->receivedPacket.payload[receiver->payloadCounter] = byte;
                receiver->payloadCounter++;
                if(receiver->payloadCounter == receiver->receivedPacket.header.length){
                    receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
                }
            }
        }
        if(receiver->receivedPacketState == AMCOM_PACKET_STATE_GOT_WHOLE_PACKET){
            uint16_t crc = AMCOM_INITIAL_CRC;
            crc = AMCOM_UpdateCRC(receiver->receivedPacket.header.type, crc);
            crc = AMCOM_UpdateCRC(receiver->receivedPacket.header.length, crc);
            for(int i = 0; i < receiver->receivedPacket.header.length; i++){
                crc = AMCOM_UpdateCRC(receiver->receivedPacket.payload[i], crc);
            }
            callFunction = crc == receiver->receivedPacket.header.crc;
            receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
            break;
        }
        counter++;
    }
    if(callFunction){
        (*receiver->packetHandler)(&receiver->receivedPacket, receiver->userContext);
    }
}