### Table of Contents

- [About](https://github.com/willerf/type-checker#about)

### About
This fun little research project explores algorithms to accomplish full type inference for imperative programming languages.
The syntax of the sample programming language has many similiarities to Rust but with a few key differences. 

### Understanding Outputs
Consider the following example:
```rs
fn increment(x) {
    return x + 1;
}
```
Running `./type-check` on this function provides the following type definition.

`increment: (int) -> int`

The above type definition can be interpreted as follows:
- To the left of the arrow is the list of parameter types, in this case `(int)`
- To the right of the arrow is the return type, in this case it is also `int`

Let us consider an example using generic types:
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
In the implementation of function `max`, we can see that the only restriction on the types of
x and y is that they must be comparable by the greater than operator. As such, the following
type definition is inferred:

`max {'a Ord}: ('a, 'a) -> 'a`

To the left of the colon, we can see all of the generics specified. Generic type variables
are indicated by a single quote followed by a lower case letter. Following the type variable
are the type class restrictions to the type, in this case the restriction `Ord` is specified.
Indicating the type must be orderable.

To the right of the colon, we can see that the function takes two parameters of the same type `'a` and
also returns a value of type `'a`.

### Complete Example

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
