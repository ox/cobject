# CObject

CObject is essentially a tiny copy of a much more complete [project](https://github.com/ddfreyne/cobject). I don't think I need polymorphism, just the reference counting and virtual destructors will do.

## EXAMPLE

    /* ==> main.c <== */
    #include <stdio.h>
    #include <assert.h>
    
    #include "cobject.h"
    
    /* the basic object */
    struct Client {
      struct COGuts guts; /* internals for handling retain/release */
    
      int id;
      int credit;
    };
    
    /* the free method that should handle destruction of malloc'd data */
    void client_destroy(void * a) {
      printf("destroying client insides, but none exist, so I'm gonna pass\n");
    }
    
    /* class description of the object, used for allocating and free'ing */
    struct COClass ClientType = {
      .destructor = &client_destroy,
      .size = sizeof(struct Client)
    };
    
    int main(int argc, char ** argv) {
      struct Client * cli = COCreate(&ClientType);
      cli->id = 1;
      cli->credit = 10;
    
      assert(cli->id == 1);
      assert(cli->credit == 10);
    
      CORetain(cli);
      CORelease(cli);
      CORelease(cli);
      
      /* everything should be cleaned up at this point */
    
      return 0;
    }

## BUILDING

  * `make` – builds the example
  * `make test` – builds and runs the example
  * `make lib` – creates a shared library, `libcobject.a` in the current directory 
    
## DOCUMENTATION

In order for structs to take advantage of the reference counting features of CObject, they must conform to the following **Rules and Incantations**:

### struct must start with `COGuts`

    struct Foo {
        struct COGuts guts;
        
        /* everything else */
        int bar;
    };

`struct COGuts` is used internally to count the number of references an object is being used by and how large the object is.

### structs need a corresponding `struct COClass`

    struct COClass FooType {
        .destructor = &_foo_destroy,
        .size = sizeof(struct Foo)
    };
    
`struct COClass` provides the destructor to use when all references `CORelease` the object, and the size of the object for when you want to allocate it on the heap. The destructor must have a declaration of `void (*some_destructor)(void*)`, as in, it takes a `void *`, your object, and doesn't return.

### Use `COCreate` and `COInit` appropriately

To allocate a new object of `struct Foo` on the stack, you **must** use `COCreate(struct COClass *)` and assign a new pointer to be the return of that function. Ex:

    struct Foo * foo = COCreate(&FooType);
    foo->bar = 5;
    
Note that `COCreate` only allocates the memory on the heap and initializes the `struct COGuts` in your object. You must take care of all of your own initializations your self.

If you want to create an object on the stack, you should use `COInit(void *, struct COClass *)` as follows:

    struct Foo foo;
    COInit(&foo, &FooType);
    
    foo.bar = 5;


### `CORetain` and `CORelease` the appropriate number of times

When an object has been properly initialized by either `COCreate` or `COInit` it starts with a reference count of 1. When a function wants to perform some operation on the object, it should call `CORetain(void *)` with the object as the parameter. This increments the reference count by one. When the function is done, it should call `CORelease(void *)` with the object as the parameter. This decrements the reference count by one.

    void some_func(struct Foo * foo) {
        CORetain(foo);
        /* do operations */
        foo->bar = 7;
        CORelease(foo);
    }

When the reference count for an object reaches 0, it's destructor (`FooType.destructor`) is called and the object is passed in. Note, that CObject will take care of freeing the pointer, thus for objects with no internal `malloc`'s, the destructor in it's class (`FooType` here) can be NULL.

## LICENSE

The MIT License (MIT)
Copyright (c) 2013 Artem Titoulenko

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
