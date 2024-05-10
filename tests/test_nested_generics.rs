
fn main() {}

fn id1(x) {
    return id2(x);
}

fn id2(x) {
    return id3(x);
}

fn id3(x) {
    return id4(x);
}

fn id4(x) {
    return id5(x);
}

fn id5(x) {
    return x;
}


