# IoT server
> C99 & low level network programming

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

### Server
Starts in `main.c`. Inside `server.h` are defined all network functions. 
`control.h` introduces `struct sensor_t` with functions used to manage sensors.
Header `common.h` defines custom protocol messages and other common structures.

Common types:
```c
// common.h
struct request_t
struct response_t

// control.h
struct sensor_t 
```

### Client
