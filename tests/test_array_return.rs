
fn main() {

    let a = empty_arr();
    let b = empty_arr2();
    let c = int_arr();
    let d = int_arr2();
    let e = empty_2d_arr();
    let f = empty_2d_arr2();
    let g = int_2d_arr();
    let h = int_2d_arr2();

    println(a);
    println(b);
    println(c);
    println(d);
    println(e);
    println(f);
    println(g);
    println(h);
}

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

