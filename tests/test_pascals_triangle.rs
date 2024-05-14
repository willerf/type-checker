
fn main() {
    let n = 10;
    let pascal = generate_pascal_triangle(n);
    print_pascal_triangle(pascal);
}

fn generate_pascal_triangle(n) {
    let triangle = [](n);
    let i = 0;
    while (i < n) {
        triangle[i] = [](i + 1);
        let j = 0;
        while (j <= i) {
            if ((j == 0) || (j == i)) {
                triangle[i][j] = 1;
            } else {
                triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return triangle;
}

fn print_pascal_triangle(triangle) {
    let n = len(triangle);
    let i = 0;
    while (i < n) {
        let row = triangle[i];
        let j = 0;
        while (j < len(row)) {
            print(row[j]);
            if (j < len(row) - 1) {
                print(" ");
            }
            j = j + 1;
        }
        println("");
        i = i + 1;
    }
}

