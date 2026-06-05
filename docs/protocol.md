# **ZMP Design**

The ZMP is a TCP-based protocol designed to allow client terminals to communicate to each other over a server. Clients can connect to the server which the server will either accept or decline. Then if accepted, the clients can communicate to each other. Any message sent by any client will be forwarded to all clients (in the global room) or to specific clients (in other rooms). The ZMP contains one global room that everyone is in when they first join but they can create upto 9 more rooms and also join those rooms.

## **Packet Layout**

```
0       1       2       3       4       5+
 +-------+-------+-------+-------+-------+----------+
 | Type  |    Checksum   |    Length     | Payload  |
 +-------+-------+-------+-------+-------+----------+
 1 byte      2 bytes         2 bytes       variable

```

## **Types of Messages**

| Name       | Value | Description                        |
|------------|-------|------------------------------------|
| CONNECT    | 0x01  | Client sends username to server    |
| MSG        | 0x02  | Chat message with payload          |
| DISCONNECT | 0x03  | Graceful disconnection             |
| ACK        | 0x04  | Server acknowledges a connection   |
| ERROR      | 0x05  | Error message                      |
| CREATE     | 0x06  | Creates a new room                 |
| JOIN       | 0x07  | Join a room                        |

## **Checksum**

For the checksum algorithm I used the Fletcher-16 algorithm. The algorithm works based on a two-sum system. It calculates the sum twice rather than once and this allows for the positioning of the bytes in the packet to be considered. The algorithm mods the sum by 255 (not 256) because the maximum value of uint8_t in C is 255. Final result is (sum2 << 8) | sum1. Before the checksum is calculated it is zeroed to make sure the calculation is accurate and the comparison produces the correct output.

## **Message Flow**

```
Client A          Server           Client B
   |                 |                 |
   |-- CONNECT ----->|                 |
   |<-- ACK ---------|                 |
   |                 |<-- CONNECT -----|
   |                 |---- ACK ------->|
   |                 |                 |
   |-- MSG --------->|                 |
   |                 |---- MSG ------->|
   |                 |                 |
   |                 |<-- MSG ---------|
   |<-- MSG ---------|                 |
   |                 |                 |
   |-- CREATE ------>|                 |
   |<-- MSG ---------|                 |
   |                 |                 |
   |                 |<----- JOIN -----|
   |                 |------ ACK ----->|
   |                 |                 |
   |-- DISCONNECT -->|                 |
   |                 |                 |
   |           [shutdown]              |
   |                 |--- DISCONNECT ->|
   |                 |                 |
```

## **Rooms**

The ZMP allows for 10 rooms that are created at the beginning of the servers creation
but are set to not in use. Once they are "created", they are set to in use and clients can
join them. They are created using the '/create' command which finds the first not in use
server and then sets it to in use. They can be joined with '/join' followed with the id of
the room that the client wants to join. There is in fact one room that is set in use from
and the beginning of the servers creation and that is the base/global/initial room.
All clients are initially put in this room.


## **Constraints**

The current constraints for the ZMP:
- Max 10 clients connected to server
- Max 10 clients in backlog
- Max message length of 65535 bytes
- Max username length is 16 bytes
- Max room size is 10

## **Error Handling**

Errors and how they are handled:
1. Checksum Mismatch -> Message is silently dropped.
2. Server full -> Connection closed immediately.
3. Client disconnects -> Client slot freed.
