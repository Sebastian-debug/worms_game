cmake_minimum_required(VERSION 3.15)
project(worms_test3)

set(CMAKE_CXX_STANDARD 14)

add_executable(worms_test3 main.cpp Game.cpp Game.h ActionCommand.cpp ActionCommand.h Chest.cpp Chest.h ChooseCommand.cpp ChooseCommand.h Command.cpp Command.h
        Field.cpp Field.h HelpCommand.cpp HelpCommand.h MapCommand.cpp MapCommand.h MoveCommand.cpp MoveCommand.h QuitCommand.cpp QuitCommand.h
        StateCommand.cpp StateCommand.h Worm.cpp Worm.h Random.cpp Random.h)