## AITestBed
 The test bed for the CS 600 Project.

 Scoped to work on Windows, Linux, and Mac OS X environments.

## Requirements
 A compiler that supports the C++11 standard.

## Web Component
 * To build server
 ```
 $make server
 ```

 Using Mongoose (https://github.com/cesanta/mongoose) for an embedded server environment.

 The webserver provides access for the testing of neural networks by specifing the following:
 * Number of layers in the network.
 * Topography of the network.
 * Weights of the nodes.
 * Input to the neural network.
 * And an optional sigmoid.

## Tests
 Currently using the Catch framework at https://github.com/philsquared/Catch 

 * To build the tests
 ```
 $make unit_tests
 ```

 * Execute test suite
 ```
 $./unit_tests
 ```

## Cleaning Build
 ```
 $make clean
 ```
