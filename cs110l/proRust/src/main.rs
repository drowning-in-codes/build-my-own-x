enum Location {
    Coordinate(f32, f32),
    Address(String),
    Unknown,
}
fn sum(a: i32, b: i32) -> i32 {
    a + b
}
fn main() {
    let l = Location::Coordinate(1f32, 1f32);
    let s = Location::Address("test".to_owned());
    match l {
        Location::Coordinate(x, y) => println!("{},{}", x, y),
        Location::Address(s) => println!("{}", s),
        Location::Unknown => (),
    }
    // read user input

    println!("Hello, world!");
}
