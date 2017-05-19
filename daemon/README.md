
## planning

+ change pidfile to perhaps a lock 
+ implement singleton daemon class


+ _Cron_ 
    + a daemon that executes scheduled commands
        + starts automatically from `/etc/init.d`
        + parse `crontabs` for contab files, loaded into memory
        + push to event list 
        + sleep and wakes up until next event scheduled
+ `Crontab`
    + install, uninstall, or list tables for driving cron daemon


+ _Daemon_ 
    + [beginner's guide](http://shahmirj.com/blog/beginners-guide-to-creating-a-daemon-in-linux) and [stackoverflow answer](http://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux)
    + steps 
        + fork child, exit parent process, process adopted by `init`
        + `setsid`: 
            + creates new session
            + calling process becomes leader of new session 
            + process group leader of new process group
            + process detached from controlling tty (i.e. shell session termination no longer kills the process)
        + fork again so that parent process terminate to ensure get rid of session leading process
        + ignore/handle signals 
        + `chdir`
        + `umask`
        + `close` all fd inherited from parents
    + [tutorial](http://0pointer.de/public/systemd-man/daemon.html#New-Style%20Daemons)
    + some of the [features](http://libslack.org/daemon/manpages/daemon.1.html) to impl. later



+ `umask`
    + command determining the settings of a mask that controls how file permissios are set for newly created files in shell
    + `umask -S` displays mask symbolically

+ `mask`
    + a grouping of bits that filters permission during file creation
    + bit set to `1`: disabled; set to `0`: permission determined by program and system 
    + each process has its own mask 

+ `setsid`
    + creates new session, with calling process the session leader with no controlling terminal 
    + returns process group id of new process group on success; -1 and set `errno` on failure
    + motivation 
        + A session is a set of processes which shares a controlling terminal



+ _exception handling_ 
    + [microsoft tutorial](https://msdn.microsoft.com/en-us/library/hh279691.aspx)
        + when calling non-exception function, wrap that function in C++ that detects error and throw exceptions


+ _destructor_ 
    ```cpp
    int main(){
    Foo *ptr=  new Foo()
    return 0;
    }
    ```
    + automatic variable `ptr` is destroyed when goes out of scope 
    + however, what `ptr` points to is dynamically allocated, hence not destroyed, so have to 
        + `delete ptr;`
    + alternatively we use `std::auto_ptr` in C++03
        + `Foo` that `ptr` points is deleted in `auto_ptr`'s destructor when `ptr` goes out of scope
        + `std::auto_ptr <Foo> ptr(new Foo)`


+ _static_ 
    + properties 
        + static member has only one copy despite however many instantiation, i.e. shared by all classes
        + static function can be called without any instantiation using `::`
        + static function can access static variable only, does not have access to `this` pointer
    + syntax
        + static member can be declared `static` inside class definition, 
        + but initialization must be made outside of class definition using scope resolution operator `::` 
            + only exception to this is type `const int`, which can use inline initialization

+ _nested class_ 
    + the declaration of class/struct within another class
    + properties 
        + name of nested class exists in scope of enclosing class
        + name lookup from a member function of nested class exame scoep of enclosing class after examing scope of nested class
        + nested class has access to enclosing class member (private, protected, public), but not `this` pointer
    + note 
        + cannot use `typedef` with `static` because `typedef` declares a type, `static` quantifies an instance, not a type



+ _forward declaration_ 
    + a declaration of identifier (type, variable, constant, function) for which a complete definition is yet to be given
    + properties   
        + OK to be forward declared if only need to use pointer type (pointer are same size despite type); 
            + pointer
            + refrerence
            + as args or return types of function declaration
            + pointer/reference of previous point
        + cannot be used to declare a member, or a base class, since compiler does not know about its type (i.e. size)
            + base class, i.e. the parent class 
            + as member of class
            + _define_ function as args or return type
            + use its member variable/function
    + note 
        + nested class cannot be forward declared


+ _abstract class_
    + [microsoft tutorial](https://msdn.microsoft.com/en-us/library/c8whxhf1.aspx)
    + concepts from which more specific classes can be derived
    + _properties_ 
        + abstract class type cannot be instantiated; but pointers and references to class is allowed
            + specifically, abstract class cannot be used for variable, member data, args, return types, type conversions
        + class with at least one pure virtual function is a abstract class
        + class derived from abstract class must implement pure virtual function or they, too, are abstract classes
    + _pure function_
        + virtual function declared pure using _pure-specifier_ syntax (i.e. `= 0`)
            + `virtual char*Identity() = 0`
                ```cpp
                class Account {
                public: 
                Account(double d);
                virtual double GetBalance();
                virtual void PrintBalance() = 0;    // pure virtual function
                private:
                double _balance;
                }
                ```
    + _restriction_
        + if constructor of abstract class calls a pure virtual function, result undefined
        + _pure virtual function_ can be defined (used in base class destructor), use `abstract-class-name::function-name()` to call the function


+ _virtual function_ 
    + a function expect to be redefined in derived classes
    + behavior
        + if refer to derived class object using a pointer or reference to the base class and call a virtual function, 
        + the derived class's version of the function is executed
        + if derived class does not implement the virtual function, then base function is called
            ```cpp
            // deriv_VirtualFunctions2.cpp  
            // compile with: /EHsc  
            #include <iostream>  
            using namespace std;  

            class Base {  
            public:  
            virtual void NameOf();   // Virtual function.  
            void InvokingClass();   // Nonvirtual function.  
            };  

            // Implement the two functions.  
            void Base::NameOf() {  
            cout << "Base::NameOf\n";  
            }  

            void Base::InvokingClass() {  
            cout << "Invoked by Base\n";  
            }  

            class Derived : public Base {  
            public:  
            void NameOf();   // Virtual function.  
            void InvokingClass();   // Nonvirtual function.  
            };  

            // Implement the two functions.  
            void Derived::NameOf() {  
            cout << "Derived::NameOf\n";  
            }  

            void Derived::InvokingClass() {  
            cout << "Invoked by Derived\n";  
            }  

            int main() {  
            // Declare an object of type Derived.  
            Derived aDerived;  

            // Declare two pointers, one of type Derived * and the other  
            //  of type Base *, and initialize them to point to aDerived.  
            Derived *pDerived = &aDerived;  
            Base    *pBase    = &aDerived;  

            // Call the functions.  
            pBase->NameOf();           // Call virtual function.  
            pBase->InvokingClass();    // Call nonvirtual function.  
            pDerived->NameOf();        // Call virtual function.  
            pDerived->InvokingClass(); // Call nonvirtual function.  
            }  
            // output 
            /*
            Derived::NameOf  
            Invoked by Base  
            Derived::NameOf  
            Invoked by Derived  
            */
            ```
    + Conventions when calling a function using pointers/reference
        + a call to virtual function is resolved according to underlying type of object for which it is called 
        + call to nonvirtual functino is resolved according to type of pointer or reference
    + style convention
        + `virtual` keyword can be used when declaring overriding functinos in derived class, but is unnecessary
            + since override of `virtual` function is always `virtual`
        + `virtual` function must be defined unless they are declared `pure`

+ _inheritance_
    + new classes derive from existing class
    + syntax
        + colon followed by a list of base specifiers (multiple inheritance)
        + base class so named must be declared previously
        + _access-specifier_ is one of `public`, `protected`, and`private`
            + controls derived class permission to use member of base class
            + omission is considered `private` by default
        + `virtual`: indicates virtual inheritance
            + base class is a virtual base class
                ```cpp
                class Derived: [virtual] [access-specifier] Base{
                // member list
                }
                ```


+ _unique pointer_ 
    + properties 
        + not shared; cannot be copied to another `unique_ptr`, pass by value to function, 
        + only move is allowed. i.e. ownership of memory resource is transfered and original `unique_ptr` does not own it 

+ _smart pointer_ 
    + `<memory>`
    + resource acquisition is initialization (RAII):
        + gives ownership of any heap-allocated resource to a stack allocated object whose destructor contains code to delete or free resource and any associated cleanups
    + _smart pointer_ 
        + initialize on stack, by taking in a raw pointer that points to heap allocated object
        + after initialization, smart pointer owns the raw pointer, i.e. responsible for deleting memory that raw pointer specifies
        + smart pointer contains call to delete, which is called when smart pointer goes out of scope; this is true even if exception is thrown
            + i.e. limited garbage collection enabled by scoping rules
        + access member with `->` and `*` returns raw pointer by overloading operators
    + _usage_ 
        + declare smart pointer as automatic variable
            + no `new` or `malloc` on smart pointer
        + in type parameter, specify pointer-to-type of encapsulated pointer
        + pass raw pointer to `new`-d objct in smart pointer constructor
        + use overloaded `->` and `*` to access the object
        + let smart pointer delete the object when going out of scope
    + _member function_
        + `unique_ptr.reset()`: release memory before smart pointer goes out of scope
        + `unique_ptr.get()`: return encapsulated raw pointer for usage with legacy function that does not support it
    + _category_ 
        + `unique_ptr`: allow exactly one owner of underlying pointer, default for POCO
            + can be moved to new owner, but not copied or shared, 
        + `shared_ptr`: reference-counted smart pointer o
            + use when assign one raw pointer to multiple owners
            + raw pointer not deleted until all `shared_ptr` owners have gone out of scope, or gave up ownership
        + `weak_ptr`: special case for use in conjunction with `shared_ptr`
            + access to object that is owned by one or more `shared_ptr` instances, but does not participate in reference counting
            + used for observing an object but do not require it to remain alive
