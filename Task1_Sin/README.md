# Task 1: Sin sum

It constructs an array of sin values from 0 to 2 Pi.
Then it computes sum of the array and prints it.

There are two build targets:
* `Task1_Sin`operates with doubles
* `Task1_SinApprox` operates with floats

To build them you need at first generate low-level build
script (for `make` or `ninja-build`).
At first run `mkdir cmake-build && cd cmake-build`, then:
* For Ninja:  `cmake -G Ninja ..`
* For Unix makefiles: `cmake -G Unix\ Makefiles ..`

Then you can build your targets as:
* Ninja: `ninja Task1_Sin`
* Unix makefiles: `make Task1_SinApprox`

Float result: `-0.0277862`

Double result: `-6.76916e-10`