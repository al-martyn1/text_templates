//-----------------------------------------------------------------------------
// Default
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
#define STRICT
#endif

#if !defined(_WINDOWS_)
#include <windows.h>
#endif

//-----------------------------------------------------------------------------
// Ansi style
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
#define STRICT
#endif

#if !defined(_WINDOWS_)
#include <windows.h>
#endif

//-----------------------------------------------------------------------------
// GNU style
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
#define STRICT
#endif

#if !defined(_WINDOWS_)
#include <windows.h>
#endif

//-----------------------------------------------------------------------------
// Linux style
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
#define STRICT
#endif

#if !defined(_WINDOWS_)
#include <windows.h>
#endif

//-----------------------------------------------------------------------------
// Kernighan and Ritchie
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
#define STRICT
#endif

#if !defined(_WINDOWS_)
#include <windows.h>
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Default
namespace foospace
    {
int Foo()
    {
    if ( isBar )
        {
        bar();
        return 1;
        }
    else
        return 0;
    }
    }

//-----------------------------------------------------------------------------
// Ansi style
namespace foospace
{
int Foo()
{
    if (isBar)
    {
        bar();
        return 1;
    }
    else
        return 0;
}
}

//-----------------------------------------------------------------------------
// GNU style
namespace foospace
  {
  int Foo()
  {
    if (isBar)
      {
        bar();
        return 1;
      }
    else
      return 0;
  }
}

//-----------------------------------------------------------------------------
// Linux style
namespace foospace
{
int Foo()
{
        if (isBar) {
                bar();
                return 1;
        } else
                return 0;
}
}

//-----------------------------------------------------------------------------
// Kernighan and Ritchie
namespace foospace {
int Foo() {
    if (isBar) {
        bar();
        return 1;
    } else
        return 0;
}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Default
class Foo
{
    public:
        Foo();
        virtual ~Foo();
};

//-----------------------------------------------------------------------------
// Ansi style
class Foo
{
public:
    Foo();
    virtual ~Foo();
};

//-----------------------------------------------------------------------------
// GNU style
class Foo
  {
  public:
    Foo();
    virtual ~Foo();
  };

//-----------------------------------------------------------------------------
// Linux style
class Foo
{
public:
        Foo();
        virtual ~Foo();
};

//-----------------------------------------------------------------------------
// Kernighan and Ritchie
class Foo {
public:
    Foo();
    virtual ~Foo();
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Default
switch (foo)
{
case 1:
    a += 1;
    break;

case 2:
{
    a += 2;
    break;
}    
}

//-----------------------------------------------------------------------------
// Ansi style
switch (foo)
{
case 1:
    a += 1;
    break;

case 2:
{
    a += 2;
    break;
}
}

//-----------------------------------------------------------------------------
// GNU style
switch (foo)
  {
  case 1:
    a += 1;
    break;

  case 2:
  {
    a += 2;
    break;
  }
  }

//-----------------------------------------------------------------------------
// Linux style
switch (foo)
{
case 1:
        a += 1;
        break;

case 2: {
        a += 2;
        break;
}
}

//-----------------------------------------------------------------------------
// Kernighan and Ritchie
switch (foo) {
case 1:
    a += 1;
    break;

case 2: {
    a += 2;
    break;
}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Default
int foo() {
    while (isFoo) {
        ...
        if (isFoo)
            goto error;
    }

error:
        ...
}

//-----------------------------------------------------------------------------
// Ansi style
int foo()
{
    while (isFoo)
    {
        ...
        if (isFoo)
            goto error;
    }

error:
    ...
}

//-----------------------------------------------------------------------------
// GNU style
int foo()
{
  while (isFoo)
    {
      ...
      if (isFoo)
        goto error;
    }

error:
  ...
}

//-----------------------------------------------------------------------------
// Linux style
int foo()
{
        while (isFoo) {
                ...
                if (isFoo)
                        goto error;
        }

error:
        ...
}

//-----------------------------------------------------------------------------
// Kernighan and Ritchie
int foo() {
    while (isFoo) {
        ...
        if (isFoo)
            goto error;
    }

error:
    ...
}

//-----------------------------------------------------------------------------

