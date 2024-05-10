
fn main() {}

fn plus(x, y) {
    x + y;
}

fn minus(x, y) {
    x - y;
}

fn star(x, y) {
    x * y;
}

fn slash(x, y) {
    x / y;
}

fn percent(x, y) {
    x % y;
}

fn plus_minus(x, y) {
    plus(x, y);
    minus(x, y);
}

fn minus_star(x, y) {
    minus(x, y);
    star(x, y);
}

fn plus_minus_star(x, y) {
    plus_minus(x, y);
    minus_star(x, y);
}

fn slash_percent(x, y) {
    slash(x, y);
    percent(x, y);
}

fn plus_minus_star_slash_percent(x, y) {
    plus_minus_star(x, y);
    slash_percent(x, y);
}

fn plus_minus_star_slash_percent2(x, y) {
    plus(x, y);
    plus_minus(x, y);
    plus_minus_star(x, y);
    minus(x, y);
    slash(x, y);
    slash_percent(x, y);
    percent(x, y);
}

