# Linux drivers lab3 tasks - "Interfacing with hardware"

## Implement hardware access

### Extend the driver implemented in lab2 with functionalities needed to access memory mapped registers

Based on the materials from the last lecture and Linux documentation extend the driver code with:

* memory space mapping
* accessing device registers
* character device API for controlling the device

You can use the [devm_ioremap_resource()](https://elixir.bootlin.com/linux/latest/source/lib/devres.c#L181) function to map the memory.
Remember to define correct `reg` property in the device tree.

The driver should control a simple arithmetic peripheral (peripheral implementation and Renode platform description are available in the [scripts](renode/scripts) directory).
See the [Renode's pydev peripheral implementation](renode/scripts/calc_periph.py) for register map.

The driver should provide the following functionalities:

* IOCTL for selecting the operation
* IOCTL for checking status of the device
* IOCTL for resetting error code
* Data should be provided to the driver by writing the `/dev` file
* Results should be fetched by reading the `/dev` file

Example flow:

```
write <-- 2
write <-- 10
ioctl operation_add
ioctl check_status
read -> 12
```

Example error handling flow:

```
write <-- 2
write <-- 0
ioctl operation_div
ioctl check_status
ioctl error_ack
print error
```

### Implement test application

To test the driver (especially ioctls) you need to implement a simple user space application.
Remember to use [`open(2)`](https://man7.org/linux/man-pages/man2/open.2.html) (along with `read(2)`, `write(2)` and `ioctl(2)`) functions for accessing the `/dev` node files.

Cross compile your application with the toolchain generated by Buildroot, or use any other RISC-V toolchain supporting 32 bit targets.
In the latter case, remember to set correct ABI and target arch (`-march=rv32g -mabi=ilp32d`). Also, link the app statically.

Copy the app to the emulated system and test the driver.


