# Corewar

# Description

Main purpose of project was to create a game of Corewar type. In Corewar you have bunch of champion players which are written on assembler-like language. Every player then compiled into bytecode and loaded on circular memory field of VM. Then one process created for every player. Process can move forward and execute valid commands. There is 16 commands, some for arithmetical operations, some for loading and storing values on field. Also there's commands which can clone process and command which declares process as living. Every process which wasn't declared living in one round dies. Wins player who's process said that he's living last.
