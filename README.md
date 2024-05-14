### Table of Contents

- [About](https://github.com/willerf/type-checker#about)
- [Understanding Outputs](https://github.com/willerf/type-checker#understanding-outputs)
  - [Concrete Types Example](https://github.com/willerf/type-checker#concrete-types-example)
  - [Generic Types Example](https://github.com/willerf/type-checker#generic-types-example)
  - [Complete Example](https://github.com/willerf/type-checker#complete-example)
- [Getting Started](https://github.com/willerf/type-checker#getting-started)
  - [Install & Build](https://github.com/willerf/type-checker#install--build)
  - [User Guide](https://github.com/willerf/type-checker#user-guide)
- [Programming Language Features](https://github.com/willerf/type-checker#programming-language-features)

### About
This fun little research project explores algorithms to accomplish full type inference for imperative programming languages.
The syntax of the sample programming language has many similiarities to Rust but with a few key differences. Most notably, the
programmer cannot specify any types, all of them are inferred. Since all function types are inferred in the most general
form, a single implementation could be called by multiple sets of arguments of varying types.

### Understanding Outputs
#### Concrete Types Example
Consider the following example.
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

#### Generic Types Example
Let us consider an example using generic types.
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

To the left of the colon, we can see a set of generics specified between curly braces. Generic 
type variables are indicated by a single quote followed by a lower case letter. Following the 
type variable are the type class restrictions to the type, in this case the restriction `Ord` 
is specified. Indicating the type must be orderable. There are 7 type classes in total, Plus, 
Minus, Star, Slash, Percent, Eq, and Ord.

To the right of the colon, we can see that the function takes two parameters of the same type `'a` and
also returns a value of type `'a`.

#### Complete Example
Consider the following function that implements and utilizes a `push` function.
```rs
fn main() {
    let arr1 = [1, 2, 3];
    println(push(arr1, 4)); 

    let arr2 = ["Hello", "World"];
    println(push(arr2, "!!")); 
}

fn push(arr, elem) {
    let size = len(arr);
    // create new array of length: size + 1
    let result = [](size + 1);

    let i = 0;
    while (i < size) {
        result[i] = arr[i];
        i = i + 1;
    }

    result[size] = elem;
    return result;
}
```
Running `./type-check`:

`push {'a}: (['a], 'a) -> ['a]`

As described in English, `push` is a function that takes as arguments an array containing elements 
of type `'a` and an element of type `'a` and returns an array containing elements of type `'a`.

Running `./execute`:

```bash
[1, 2, 3, 4]
[Hello, World, !!]
```

More examples can be found in the `tests` directory.

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

### Programming Language Features

`Types:` int, bool, char, str and array

`Operators:` +, -, *, /, %, ==, !=, >, >=, <, <=, [..]

`Literals:` int - `5`, bool - `true`/`false`, char - `'a'`, str - `"Hello"`, array - `[1, 2, 3]`

`Constructs:` let, if else, while, function calls/recursion

`Provided Functions:`
- `print` {'a}: ('a) -> unit
- `println` {'a}: ('a) -> unit
- `len` {'a}: (['a]) -> int
