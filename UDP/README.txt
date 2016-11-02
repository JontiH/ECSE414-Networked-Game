README file on how to use Client.cpp and Server.cpp

1) Must compile both files on linux using gcc or g++
2) First run compiled version of Server.cpp
3) Then run compiled version of Client.cpp 

syntax on how to use both file from bash shell:
    (Assuming you named both compiled files 'client' and 'server'.)

    Client:
        ./client [ip] [port] [message]

        [ip]: can be any ipv4, ipv6, service name or website.
              (127.0.0.1, ::1, chrome, www.google.com, etc.)

        [port]: port number between 1025-65535 and not currently in use.

        [message]: actual message you want to send in double quote.
                   ("Hello World!")

    Server:
        ./server [port]

        [port]: port number between 1025-65535 and not currently in use.

Notes:
   
    - The port number must be open on both machine (i.e. not blocked by a firewall).
    - client will exit upon sending 1 message.
    - server will exit upon receiving 1 message, but will wait otherwise. 
    - Forcing server to exit (i.e. CTRL+Z) will result in the socket/port being stuck in use.
        Requiring a reboot of the machine to free the socket.
