#pragma once

#include <string>
#include <mutex>
#include <iostream>

#include "queue.h"
#include "message.h"
#include "sender.h"
#include "receiver.h"
#include "print.h"

class interface_machine
{
public:
    void done()
    {
        get_sender().send( messaging::close_queue {} );
    }

    messaging::sender get_sender()
    {
        return incoming;
    }

    void run()
    {
        try {
            while ( true ) {
                incoming.wait()
                    .handle<issue_money>(
                        [&]( issue_money const & msg ) {
                            printlnEx( "Issuing ", msg.amount );
                        } )
                    .handle<display_insufficient_funds>(
                        [&]( display_insufficient_funds const & msg ) {
                            printlnEx( "Insufficient funds" );
                        } )
                    .handle<display_enter_pin>(
                        [&]( display_enter_pin const & msg ) {
                            printlnEx( "Please enter you PIN(0-9)" );
                        } )
                    .handle<display_enter_card>(
                        [&]( display_enter_card const & msg ) {
                            printlnEx( "Please enter your card(I)" );
                        } )
                    .handle<display_balance>(
                        [&]( display_balance const & msg ) {
                            printlnEx( "The balance of your account is ", msg.amount );
                        } )
                    .handle<display_withdrawal_options>(
                        [&]( display_withdrawal_options const & msg ) {
                            printlnEx( "Withdraw 50?(w)" );
                            printlnEx( "Display Balance? (b)" );
                            printlnEx( "Cancel? (c)" );
                        } )
                    .handle<display_withdrawal_cancelled>(
                        [&]( display_withdrawal_cancelled const & msg ) {
                            printlnEx( "Withdrawal cancelled" );
                        } )
                    .handle<display_pin_incorrect_message>(
                        [&]( display_pin_incorrect_message const & msg ) {
                            printlnEx( "PIN incorrect" );
                        } )
                    .handle<eject_card>(
                        [&]( eject_card const & msg ) {
                            printlnEx( "Ejecting card" );
                        } );
            }
        } catch ( messaging::close_queue const & ) {
        }
    }

private:
    messaging::receiver incoming;
};
