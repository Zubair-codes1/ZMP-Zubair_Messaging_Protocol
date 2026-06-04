# **ZMP Design**

The ZMP is a TCP-based protocol designed to allow client terminals to communicate to each other over a server. Clients can connect to the server which the server will either accept or decline. Then if accepted, the clients can communicate to each other. Any message sent by any client will be forwarded to all clients. In future reworks of the ZMP, I plan to add chatrooms for private conversations.

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
   |-- DISCONNECT -->|                 |
   |                 |                 |
   |           [shutdown]              |
   |                 |--- DISCONNECT ->|
   |                 |                 |
```

## **Constraints**

The current constraints for the ZMP:
- Max 10 clients connected to server
- Max 10 clients in backlog
- Max message length of 65535 bytes
- Max username length is 16 bytes

## **Error Handling**

Errors and how they are handled:
1. Checksum Mismatch -> Message is silently dropped.
2. Server full -> Connection closed immediately.
3. Client disconnects -> Client slot freed.
