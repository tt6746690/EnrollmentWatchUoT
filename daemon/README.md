
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


