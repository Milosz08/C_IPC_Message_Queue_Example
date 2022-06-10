# Simple IPC message queue example

A simple example showing a message queue in UNIX based on client and server software. The client sends a message, which the server processes and sends back to the client. The server "listens" by continuously running in a loop.

## Compile and Run
 - In order to run both the server and the client programs you need to compile them beforehand (e.g. using the built-in gcc compiler on Linux):
```
$ gcc client.c -o compClient
$ gcc server.c -o compServer
```
- Then run the client program in separate consoles.
```
$ ./compClient
```
```
$ ./compServer
```
> To stop the server, press Ctr+C (Linux) or Command+C (macOS).

## License
This program is on GNU-GPL 3.0 license.
