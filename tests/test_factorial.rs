
fn main() {
    print("factorial_recur(5): "); 
    println(factorial_recur(5));
    
    print("factorial_iterative(5): "); 
    println(factorial_iterative(5));
}

fn factorial_recur(n) {
    if (n < 1) {
        return 1;
    }
    else {
        return n * factorial_recur(n - 1);
    }
}

fn factorial_iterative(n) {
    let result = 1;
    while (n > 1) {
        result = result * n;
        n = n - 1;
    }
    return result;
}

