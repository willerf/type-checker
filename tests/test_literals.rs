
fn main() {
    let a = zero();
    let b = int();
    let c = neg_int();
    let d = bool_true();
    let e = bool_false();
    let f = char();
    let g = empty_string();
    let h = string();

    println(a);
    println(b);
    println(c);
    println(d);
    println(e);
    println(f);
    println(g);
    println(h);
}

fn zero() {
    return 0;
}

fn int() {
    return 3;
}

// todo: support negative integer literals
fn neg_int() {
    return 0 - 5;
}

fn bool_true() {
    return true;
}

fn bool_false() {
    return false;
}

fn char() {
    return 'a';
}

fn empty_string() {
    return "";
}

fn string() {
    return "Hello world!";
}

