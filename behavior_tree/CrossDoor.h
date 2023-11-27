#pragma once

#include "brain.h"

class CrossDoor
{
public:
    CrossDoor() = default;
    ~CrossDoor() = default;

    // SUCCESS if _door_open == true
    brain::Status isDoorClosed();
    // SUCCESS if _door_open == true
    brain::Status passThroughDoor();
    // After 3 attempts, will open a locked door
    brain::Status pickLock();
    // FAILURE if door locked
    brain::Status openDoor();
    // WILL always open a door
    brain::Status smashDoor();

private:
    bool _door_open { false };
    bool _door_locked { true };
    int _pick_attemps { 0 };
};
