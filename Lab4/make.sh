#!/bin/bash

# Clean existing binaries
echo "Cleaning existing executables..."
rm -f server client

# Compile server.c
echo "Compiling server.c..."
gcc server.c -o server -Wall -Wextra
if [ $? -ne 0 ]; then
    echo "|v| Error: Failed to compile server.c"
    exit 1
else
    echo "|^| Successfully compiled server.c -> ./server"
fi

# Compile client.c
echo "Compiling client.c..."
gcc client.c -o client -Wall -Wextra
if [ $? -ne 0 ]; then
    echo "|v| Error: Failed to compile client.c"
    exit 1
else
    echo "|^| Successfully compiled client.c -> ./client"
fi

echo "|^| Build complete. Executables: ./server and ./client"
