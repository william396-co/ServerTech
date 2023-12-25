#include <iostream>

#include <thread>

#include "queue.h"
#include "sender.h"

#include "bank_machine.h"
#include "atm_machine.h"
#include "interface_machine.h"

int main()
{

    bank_machine bank;
    interface_machine interface_hardware;
    atm machin( bank.get_sender(), interface_hardware.get_sender() );

    std::thread bank_thread( &bank_machine::run, &bank );
    std::thread if_thread( &interface_machine::run, &interface_hardware );
    std::thread atm_thread( &atm::run, &machine );

    messaging::sender atmqueue( machine.get_sender() );

    bool quit_pressed = false;

    while ( !quit_pressed ) {
    }

    bank.done();
    machine.done();
    interface_hardare.done();

    atm_thread.join();
    bank_thread.join();
    if_thread.join();

    return 0;
}
