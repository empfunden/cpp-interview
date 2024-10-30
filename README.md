# Air Traffic Control

## Background

Let's say that we're an air traffic control team, and we'd like to write some software to help our human traffic controllers ensure that the aircraft under our management at any given moment aren't at risk of colliding with each other. Towards that objective, we'd like to write a C++ library that processes messages that contain information about aircraft location. Each message consists of the following fields:

- `timestamp_ns`: the epoch timestamp in nanoseconds
- `aircraft_id`: the aircraft's identifier
- `position`: a 3D vector indicating the current location of the aircraft
- `direction`: a unit 3D vector indicating the forward direction of the aircraft
- `speed`: the aircraft's speed in meters per second

In addition, there are announcement messages that arise when an aircraft enters and exits our airspace.

Your task is to finish the code in `src/Planespotter.cpp` and to write some basic documentation in `DOCUMENTATION.md`. The unit tests in `test.cpp` will act as the entry point for running your library code and as a basic form of validation. Please feel free to modify existing files or to add files. 

While this scenario is safety-themed, it isn't necessary to construct an implementation that adheres to best practices within the domain of realtime, safety-critical systems programming that actual collision avoidance software might want to. 


### Some things to consider

We expect that solving this will take you about two hours, although you're free to take as much or as little time as you want. Your implementation should prioritize correctness and simplicity, rather than performance, generality, or the accurate simulation of real-world collision avoidance systems.

Here are some of the things we'll be looking for in your solution:
  * Can you understand the development environment and be productive in it?
  * Can you create a straightforward and effective solution in a reasonable amount of time?
  * Is there any unnecessary complexity? Is the amount of abstraction appropriate?
  * Does it resemble software that others would want to maintain in perpetuity?
  * Does it demonstrate an awareness for contemporary best practices in C++?

## Development

The repository, in its current state, is only compatible with a 64-bit version of a Linux-based OS. We recommend accessing one for free via a Github Codespace (see `codespace.png` if you're unfamiliar with how to obtain one). A Codespace is a preconfigured, isolated development container running in the cloud, and so insulates you from some potential development environment woes. The next stage of the interview process will also be conducted within a collaborative Codespace (due to the availability of a convenient live sharing extension), extending the work you do in this stage.

When you've got the repository checked out in your environment of choice, it's important to first create a working branch with `git checkout -b [your branch name]`, since you'll be submitting a `git diff` of the result of your work and the state of `main`.

When you're on your branch, `make build` will build the unit test binary, and `make test` will run it. If you're not working in a Codespace, these commands will get dependencies if they're not already present, so it'll take a while the first time you run either of these. You can modify `environment.yml` as well as the `Makefile` and `CMakeLists.txt` to adjust the development environment as you see fit.

You can build and run in release mode by setting the environment variable `BUILD_TYPE=release` in your invocation of either `make build` or `make test`.

When you're done, run `make patch` and send the result, `aqtc_cpp.patch`, to your recruiter via e-mail. 
