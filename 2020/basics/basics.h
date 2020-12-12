#pragma once

#include "basic_types.h"

#undef assert
#define assert( Condition ) ((!!(Condition)) || Com_Assert(Condition, "", __FILE__, (u32)__LINE__))
#define assertmsg( Condition, Message ) ((!!(Condition)) || Com_Assert(Condition, Message, __FILE__, (u32)__LINE__))

#define HAS_FLAG( var, flag ) ((var & flag) == flag )
#define ARRAY_SIZE( array ) ((sizeof(array) / sizeof(array[0])))

bool I_strcmp( const char* s1, const char* s2 );
bool I_stricmp( const char* s1, const char* s2 );
size_t I_strlen( const char* str );

bool I_strispresent( const char* haystack, const char* str );

void I_strcpy( char* dest, size_t size, const char* src );
template<size_t Size>
void I_strcpy( char (&dest) [Size], const char* src )
{
	return I_strcpy( dest, Size, src );
}

template <class T>
const T I_min( const T a, const T b )
{
  return !(b<a)?a:b;
}

template <class T>
const T I_max( const T a, const T b )
{
  return (b<a)?a:b;
}

template <typename T>
const T I_abs( const T a )
{
	return abs( a );
}

bool Com_Assert( bool condition, const char* message, const char* file, u32 line );
void Com_Printf( const char* fmt, ... );
void Com_Println( const char* fmt, ... );
void Com_PrintError( const char* fmt, ... );

void Com_FatalError( int errCode, const char* fmt, ... );

int Com_sprintf( char* dest, size_t size, const char* fmt, ... );

void Com_EllipseString( char* dest, size_t size, const char* src );
template<size_t Size>
void Com_EllipseString( char (&dest) [Size], const char* src )
{
	return Com_EllipseString( dest, Size, src );
}

template<size_t Size, typename... Args>
int Com_sprintf( char (&dest) [Size], const char* fmt, Args... args )
{
    return Com_sprintf( dest, Size, fmt, args... );
}

template <typename F>
struct privDefer {
	F f;
	privDefer(F f) : f(f) {}
	~privDefer() { f(); }
};

template <typename F>
privDefer<F> defer_func(F f) {
	return privDefer<F>(f);
}

struct defer_new 
{
    template<typename F>
    privDefer<F> operator+( F f ) { return privDefer<F>( f ); }
};

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x)    DEFER_2(x, __COUNTER__)
#define defer auto DEFER_3(_defer_) = defer_new{} + [&]

template<typename T>
struct privRange {
	T* data;
	size_t size;
	T* begin() { return data; }
	T* end() { return data + size; }
};

template<typename T>
privRange<T> range( T* a, size_t size )
{
	return privRange<T> { a, size };
}

