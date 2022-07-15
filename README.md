# chat-boards

A simple client-server chat app with imgui. 
An excuse to try out some libraries: imgui, nng, and serialization/deserialization libs.

## Build requirements

CMake and Conan

## To build

`conan install . -if build`

`cmake -S . -B build`

`cmake --build build`

## To run

`./build/server/ChatBoardServer`

`./build/client/ChatBoardClient`
