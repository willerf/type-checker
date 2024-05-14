### Table of Contents

- [About](https://github.com/willerf/type-checker#about)

### About
This fun little research project explores algorithms to accomplish full type inference for imperative programming languages.
The syntax of the sample programming language has many similiarities to Rust but with a few key differences. 

### Understanding Outputs
Consider the following example:
```rs
fn max(x, y) {
    if (x > y) {
        return x;
    }
    else {
        return y;
    }
}
```
Running `./type-check` on this function provides the following type definition.

`max {'a Ord}: ('a, 'a) -> 'a`

### Examples

### Getting Started

#### Install & Build
- Ensure you have [cmake](https://cmake.org/download) and C++20 installed
  - To verify cmake was installed correctly, run `cmake --version`
- Clone the repo and `cd` into the project directory
- Then run the following command(s)
```bash
mkdir build && cd build && cmake .. && make -j8
```

#### User Guide
Within the build directly, two executables can be found, `./type-check` and `./execute`. To have the type checker print out the inferred function types, run `./type-check` 
with the filepath provided as an argument. To execute your program using the provided interpreter, run `./execute` with the filepath provided as an argument.

```bash
./type-check main.rs
```
```bash
./execute main.rs
```
