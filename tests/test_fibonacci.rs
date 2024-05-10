
fn main() {
    println("fibonacci_recur(10): "); 
    println(fibonacci_recur(10));
    
    println("fibonacci_iterative(10): "); 
    println(fibonacci_iterative(10));
}

fn fibonacci_recur(n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return fibonacci_recur(n - 1) + fibonacci_recur(n - 2);
}

fn fibonacci_iterative(n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    let a = 0;
    let b = 1;
    let c = 1;

    let i = 2;
    while (i < n) {
        a = b;
        b = c;
        c = a + b;
        i = i + 1;
    }

    return c;
}

