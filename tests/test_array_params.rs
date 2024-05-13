
fn main() {}

fn empty_arr() {
    return [];
}

fn empty_arr2() {
    return [](10);
}

fn int_arr() {
    return [1, 2, 3];
}

fn int_arr2() {
    return [1, 2, 3](10);
}

fn empty_2d_arr() {
    return [[]];
}

fn empty_2d_arr2() {
    return [[](10), [](10), [](10)](10);
}

fn int_2d_arr() {
    return [[1, 2, 3], [4, 5, 6], [7, 8, 9]];
}

fn int_2d_arr2() {
    return [[1, 2, 3](10), [4, 5, 6](10), [7, 8, 9](10)](10);
}

