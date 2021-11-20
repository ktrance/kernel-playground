#!/bin/bash

# upgrade system
apt update && apt full-upgrade -y
# install kernel headers
apt install linux-headers-$(uname -r) -y
# install make, gcc etc
apt install build-essential -y