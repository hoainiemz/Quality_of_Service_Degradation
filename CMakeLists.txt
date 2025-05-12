cmake_minimum_required(VERSION 3.31)
project(Quality_of_Service_Degradation)

set(CMAKE_CXX_STANDARD 23)

add_executable(Quality_of_Service_Degradation main.cpp
        Grapher.cpp
        Grapher.h
        Constants.cpp
        Constants.h
        MyRandom.cpp
        MyRandom.h
        IterativeSolution.cpp
        IterativeSolution.h
        IterativeGreedy.cpp
        IterativeGreedy.h
        AdaptiveTrading.cpp
        AdaptiveTrading.h
        SamplingApproach.cpp
        SamplingApproach.h
        Utils.cpp
        Utils.h)
