#!/bin/bash

# Instalar gcc
sudo dnf install gcc -y

make compress
make decompress

echo "Installation and compilation completed."