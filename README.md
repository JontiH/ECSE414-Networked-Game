# ECSE414-Networked-Game
a networked game developed for ECSE-414

Setup Instruction

    1) Have GCC version 4.9+ installed (does not work with 4.8 and older).
    2) Have SFML 2.4+ installed.
        On Arch Linux the package is called: libdev-sfml
        Note on Ubuntu the package is outdated -> must compile the library
         manually see: http://www.sfml-dev.org/tutorials/2.4/compile-with-cmake.php
        Precompiled library from sfml-dev.org don't work on all linux distro.
    3) Clone the git repo or the source file provided by our team.
    4) On the server's machine compile the server.cpp file using:
        make server
        (assuming you are in the source file directory)
    5) On both client's machine compile the main.cpp file using:
        make client
        (assuming you are in the source file directory)
    6) On the server's machine run the following command:
        sudo ./server [port]
        where [port] is an unused port number without [].
    7) On both client's machine run the following command:
        sudo ./client [IP] [PORT]
        where [IP] is the server's public ip without [].
        where [PORT] is the SAME port used by step 6) without [].
    Notes:
       Both clients must not have the same public ip
       i.e. They must not be on the same local network...
