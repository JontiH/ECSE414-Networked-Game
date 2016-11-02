README file on how to use Client.cpp and Server.cpp

1) Must compile both files on linux using gcc or g++
2) syntax on how to use both file from bash shell:
    (Assuming you named both file 'client' and 'server' respectively
    and are currently in the directory of the files)

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
    
    -client will exit upon sending 1 message.
    -server will exit upon receiving 1 message, but will wait otherwise. 
    -Forcing server to exit (i.e. CTRL+Z) will result in the socket/port being stuck in use.
        Requiring a reboot to free the socket.
