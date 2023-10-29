#!/bin/bash

pushd build/



rm ./raytracer

# create platform build
g++ -ggdb -DDEBUG -o raytracer ../src/raytracer.cpp -lSDL2 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Werror -DDEBUG -Wno-class-memaccess


popd
