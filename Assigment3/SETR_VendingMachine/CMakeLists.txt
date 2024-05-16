# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(SETR_VendingMachine_V2)

target_sources(app PRIVATE src/main.c ../Vending_Machine_Final/src-gen/VendingMachine.c ../Vending_Machine_Final/src-gen/VendingMachine_required.c)
target_include_directories(app PRIVATE ../Vending_Machine_Final/src-gen ../Vending_Machine_Final/src)

