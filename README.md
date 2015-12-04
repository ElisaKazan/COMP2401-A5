# COMP2401-A5

## Authors
Elisa Kazan and Jack McCracken

## Purpose
A two-player game similar to Probe and Hang-Man. Each player will be prompted
to enter a phrase that the other player will try to guess. Players can guess
either one letter at a time or the entire word. The first player to guess their
opponents word wins. 

## Source/Build/Data Files

- defs.h
- game.c
- main.c
- network.c
- util.c
- README.md
- Makefile

## Compilation Commands

To compile this game, type:

make

## Instructions

First, start the server by running the following command on your server
computer:

./Assignment5

To start your client (either on the same computer or on another computer),
run:

./Assignment5 SERVER_IP_ADDRESS

Where SERVER_IP_ADDRESS is the IP address of the computer you ran the first
command.
