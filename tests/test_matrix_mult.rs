fn main() {
    let a = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ];
    
    let b = [
        [9, 8, 7],
        [6, 5, 4],
        [3, 2, 1]
    ];
    
    let result = matrix_multiply(a, b, 0);
    println(result);
}

fn matrix_multiply(a, b, init_val) {
    let rows_a = len(a);
    let cols_a = len(a[0]);
    let cols_b = len(b[0]);
    
    let result = [](rows_a);
    let i = 0;
    while (i < rows_a) {
        result[i] = [](cols_b);
        let j = 0;
        while (j < cols_b) {
            result[i][j] = init_val;
            let k = 0;
            while (k < cols_a) {
                result[i][j] = result[i][j] + (a[i][k] * b[k][j]);
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    
    return result;
}

