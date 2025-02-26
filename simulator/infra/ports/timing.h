/**
 * timing.h - Implementation of safe timing primitive types
 *
 * @author Denis Los
 * Copyright 2018 MIPT-MIPS project
 */

#ifndef INFRA_TIMING_H
#define INFRA_TIMING_H

#include <infra/macro.h>
#include <infra/types.h>

#include <boost/operators.hpp>

#include <iostream>
#include <string>

class Cycle;
class Latency;

class Cycle : public boost::totally_ordered<Cycle>
{
    public:
        constexpr explicit Cycle( uint64 value) : value( value) { }

        constexpr auto operator==( const Cycle& rhs) const { return value == rhs.value; }
        constexpr auto operator<( const Cycle& rhs) const { return value < rhs.value; }

        constexpr void inc() { ++value; }
        constexpr explicit operator double() const { return narrow_cast<double>( value); }

        constexpr uint64 operator%( uint64 number) const { return value % number; }

        constexpr Cycle   operator+( const Latency& latency) const;
        constexpr Cycle   operator-( const Latency& latency) const;
        constexpr Latency operator-( const Cycle& cycle) const;

        friend std::ostream& operator<<( std::ostream& os, const Cycle& cycle)
        {
            return os << cycle.value;
        }
        friend std::istream& operator>>( std::istream& is, Cycle& cycle)
        {
            return is >> cycle.value;
        }
        std::string to_string() const { return std::to_string( value); }

    private:
        uint64 value;
};

// NOLINTNEXTLINE(google-runtime-int) https://bugs.llvm.org/show_bug.cgi?id=24840
constexpr inline auto operator""_cl( unsigned long long int number)
{
    return Cycle( uint64{ number});
}

class Latency : public boost::totally_ordered<Latency>
{
    public:
        constexpr explicit Latency( int64 value) : value( value) { }

        constexpr auto operator==( const Latency& rhs) const { return value == rhs.value; }
        constexpr auto operator<( const Latency& rhs) const { return value < rhs.value; }
        constexpr auto operator+( const Latency& rhs) const { return Latency( value + rhs.value); }
        constexpr auto operator-( const Latency& rhs) const { return Latency( value - rhs.value); }
        constexpr auto operator/( int64 number) const { return Latency( value / number); }
        constexpr auto operator*( int64 number) const { return Latency( value * number); }

        friend std::ostream& operator<<( std::ostream& os, const Latency& latency)
        {
            return os << latency.value;
        }
        friend std::istream& operator>>( std::istream& is, Latency& latency)
        {
            return is >> latency.value;
        }

        friend constexpr inline Cycle Cycle::operator+( const Latency& latency) const;
        friend constexpr inline Cycle Cycle::operator-( const Latency& latency) const;
        std::string to_string() const { return std::to_string( value); }
        size_t to_size_t() const { return value; }

    private:
        int64 value;
};

// NOLINTNEXTLINE(google-runtime-int) https://bugs.llvm.org/show_bug.cgi?id=24840
constexpr inline auto operator""_lt( unsigned long long int number)
{
    return Latency( narrow_cast<int64>( number));
}

constexpr inline auto operator*( int64 number, const Latency& latency) { return latency * number; }

constexpr Cycle   Cycle::operator+( const Latency& latency) const { return Cycle( value + latency.value); }
constexpr Cycle   Cycle::operator-( const Latency& latency) const { return Cycle( value - latency.value); }
constexpr Latency Cycle::operator-( const Cycle& cycle) const { return Latency( value - cycle.value); }

#endif // INFRA_TIMING_H
