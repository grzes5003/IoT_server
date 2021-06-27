# IoT server
> C99 & low level network programming & Docker

## Protocol
To exchange information new mini-protocol is defined. It resides in Application
layer on top of UDP. 
Each sent or received message consists of:
 - `MSG_TYPE` defined in `common.h`
 - `TOKEN` (currently not in use) can be used to authenticate sender
 - `PAYLOAD` depending on context, can include current state of sensor etc.

<pre>
  0          8                             72      
 +----------+------------------------------+
 | MSG_TYPE |           TOKEN              |
 -----------+-------------------------------
 |               PAYLOAD                   |
 +-----------------------------------------+
 </pre>
 
### Message types
- `MSG_HELLO`
- `MSG_READ`

Requests:
- `REQ_RETRANS`
- `REQ_READ_SENSOR`
- `REQ_SHUTDOWN`
- `REQ_CHANGE_UPDATE_PRD`

Responses:
- `RES_ACK`
- `RES_ERR`

### New sensor discovery
On start, new sensor sends `MSG_HELLO` to server, which adds the sensor 
to subscribed device list and responds with `RES_ACK`.

## Typical behaviour
Each time new sensor device starts, 
it must send hello message to the server. 
Then server adds a newly discovered sensor 
to `sensor_t` subscribers list. From now on server will follow status of device, 
collect informations and log them.
 
Client wakes up periodically, sends its status to associated server. 
Then client listens several seconds for response from the server. 
During this short period server send all queued messages to the client, if there are any.
After that period client returns to standby state.

Main reason of this approach is to limit power consumption by clients, 
which often do not have access to independent current supply. 

## Project Structure
Project consists of two directories:
- Server
- Client

Common types used by client and server:
```c
// common.h
struct request_t
struct response_t

uint64_t token_t
```

Types specific to server:
```c
// control.h
struct sensor_t
```

### Server
Starts in `main.c`. Inside `server.h` are defined all network functions. 
`control.h` introduces `struct sensor_t` with functions used to manage sensors.
Header `common.h` defines custom protocol messages and other common structures.

### Client
Initiated in `main.c`. Sets up client (among others, sending hello message)
and periodically sends to server read from its sensor. 

## How to run
Navigate to project directory and run server with desired clients by adding its name
(currently three are defined).
```bash
docker-compose run server client_1
```

## Example console output
Server:
```
21:54:45 INFO  /tmp/tmp.4aeQwZJ5yI/src/server.c:18: created socket
21:54:45 INFO  /tmp/tmp.4aeQwZJ5yI/src/server.c:36: bound socket
21:54:45 INFO  /tmp/tmp.4aeQwZJ5yI/src/main.c:27: Waiting for incoming messages ...
21:54:51 INFO  /tmp/tmp.4aeQwZJ5yI/src/server.c:64: Received message from ::ffff:192.168.56.33
21:54:51 INFO  /tmp/tmp.4aeQwZJ5yI/src/server.c:71: Message: TYPE 30, PAYLOAD 1234567
21:54:51 INFO  /tmp/tmp.4aeQwZJ5yI/src/server.c:64: Received message from ::ffff:192.168.56.33
21:54:51 INFO  /tmp/tmp.4aeQwZJ5yI/src/server.c:71: Message: TYPE 31, PAYLOAD 21.01745
21:55:06 INFO  /tmp/tmp.4aeQwZJ5yI/src/server.c:64: Received message from ::ffff:192.168.56.33
21:55:06 INFO  /tmp/tmp.4aeQwZJ5yI/src/server.c:71: Message: TYPE 31, PAYLOAD 2.254541
```
Client:
```
21:54:51 INFO  /tmp/tmp.HiNNX6DBEB/src/main.c:20: env SRV_ADDR not defined, using 192.168.56.33 instead
21:54:51 INFO  /tmp/tmp.HiNNX6DBEB/src/client.c:79: got RES_ACK from server
21:54:51 INFO  /tmp/tmp.HiNNX6DBEB/src/client.c:96: sent temperature 21.01745
21:55:06 INFO  /tmp/tmp.HiNNX6DBEB/src/client.c:96: sent temperature 2.254541
```
