#!/bin/bash

g++ -std=c++20 \
./*.cpp ./includes/*.cpp \
-Wall -Wextra -O2 \
-o server.out