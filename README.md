# maclookup
query oui.txt to find producer of a network interface

# Input

A valid full length mac address or the first 8 characters (manufacturer prefix).

# HTTP request

We must avoid "Connection: keep-alive\r\n" in the request.
This header is to make multiple connection, whereas we only need one. Were we to
use this socket, the program would hang up on the call to read. The server will
not imediatly close the connection since it waits for another request.
