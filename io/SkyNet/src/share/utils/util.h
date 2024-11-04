#pragma once

void errif( bool cond, const char * message );

int open_clientfd( const char * hostname, const char * port );

int open_listenfd( const char * port );

