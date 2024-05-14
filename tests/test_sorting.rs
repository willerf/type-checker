
fn main() {
    let arr = [96, 18, 68, 9, 95, 92, 60, 75, 75, 81, 44, 35, 32, 43, 92, 63, 24, 28, 99, 1, 45, 62, 8, 26, 27];
    println(bubble_sort(arr));   
    println(insertion_sort(arr));   
    println(merge_sort(arr));   
}

fn bubble_sort(arr) {
    let n = len(arr);
    let i = 0;
    while (i < n) {
        let j = 0;
        while (j < n - 1) {
            if (arr[j] > arr[j + 1]) {
                let tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return arr;
}

fn insertion_sort(arr) {
    let n = len(arr);
    let i = 1;
    while (i < n) {
        let key = arr[i];
        let j = i - 1;
        while ((j >= 0) && (arr[j] > key)) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
        i = i + 1;
    }
    return arr;
}

fn merge_sort(arr) {
    let n = len(arr);
    if (n == 1) {
        return arr;
    }
    let mid = n / 2;
    let lhs = [](mid);
    let rhs = [](n - mid);
    let i = 0;
    while (i < mid) {
        lhs[i] = arr[i];
        i = i + 1;
    }
    
    while (i < n) {
        rhs[i - mid] = arr[i];
        i = i + 1;
    }

    let lhs = merge_sort(lhs);
    let rhs = merge_sort(rhs);

    return merge(lhs, rhs);
}

fn merge(arr1, arr2) {
    let n = len(arr1);
    let m = len(arr2);
    let result = [](n + m);

    let i = 0;
    let j = 0;
    while ((i + j) < (n + m)) {
        let k = i + j;
        if (i >= n) {
            result[k] = arr2[j];
            j = j + 1;
        }
        else {
            if (j >= m) {
                result[k] = arr1[i];
                i = i + 1;
            }
            else {
                if (arr1[i] < arr2[j]) {
                    result[k] = arr1[i];
                    i = i + 1;
                }
                else {
                    result[k] = arr2[j];
                    j = j + 1;
                }
            }
        }
    }

    return result;
}
