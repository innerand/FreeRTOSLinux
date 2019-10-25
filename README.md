# FreeRTOS Linux Port 
FreeRTOS port implementation for Linux using POSIX threads.
Made to test FreeRTOS projects on Linux systems.

## Getting Started
Include the files inside the port folder into your FreeRTOS project.

###  Build the example project
```
git clone https://github.com/innerand/FreeRTOSLinux.git
make get_freertos
make 
```

## Requirements 
- FreeRTOS 10.2.1, other versions may work but are not tested.  
- Tasks must be created with dynamic memory allocation using `vTaskCreate()`.
  Tasks with statically allocated memory are not supported.
- FreeRTOS should be compiled with a c compiler. C++ doesn't work well with
  `pthread_cancel()` which is used to end FreeRTOS tasks. 
  This also applies to c++ functions called within a FreeRTOS task.
  `pthread_cancel()` throws an exception which is problematic e.g. within
  destructors or while another exception is handled. If the program aborts with
  `terminate called without an active exception` it was probably because
  `pthread_cancel()` was executed at a wrong moment. 
  However, this port is used for CI/CD testing with Gtest/Gmock (C++) and turned
  out to be quite stable when all the C++ has been put into critical sections.  

## Usage
- FreeRTOS tasks may be interrupted at anytime. Therefore all functions that are
  not async-signal safe (e.g. `printf()`) must be put into critical sections. 
  If the program crashes or deadlocks within syscalls, it's probably because a
  function hasn't been put into a critical section. 
- Add an `ApplicationIdleTask()` to the project and put a nanosleep (without a
  critical section) into it. Otherwise the port will eat all your CPU time.


## Versioning
[SemVer](http://semver.org/) is used for versioning. For the versions available,
see the [tags on this repository](https://github.com/innerand/FreeRTOSLinux/tags)


## License
This project is licensed under the MIT License - see the
[LICENSE.md](LICENSE.md) file for details

