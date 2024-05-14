
fn main() {
    println(append([1, 2, 3], [4, 5, 6, 7, 8, 9]));
}

fn append(x, y) {
    let size = len(x) + len(y);
    let result = [](size);
    let i = 0;
    while (i < size) {
        if (i < len(x)) {
            result[i] = x[i];
        }
        else {
            result[i] = y[i - len(x)];
        }
        i = i + 1;
    }
    return result;
}

