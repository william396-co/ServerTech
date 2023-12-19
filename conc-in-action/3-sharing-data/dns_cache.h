#pragma once

#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>

/*
 * read-write-lock
 */
class dns_entry
{};

class dns_cache
{
public:
    dns_entry find_entry( std::string const & domain ) const
    {
        std::shared_lock lock( entry_mtx );
        auto const it = entries.find( domain );
        return ( it == entries.end() ) ? dns_entry {} : it->second;
    }

    void update_or_add_entry( std::string const & domain, dns_entry const & dns_details )
    {
        std::lock_guard lock( entry_mtx );
        entries[domain] = dns_details;
    }

private:
    std::map<std::string, dns_entry> entries;
    mutable std::shared_mutex entry_mtx;
};
