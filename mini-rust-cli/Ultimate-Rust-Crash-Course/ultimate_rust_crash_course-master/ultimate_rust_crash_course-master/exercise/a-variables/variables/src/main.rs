fn main() {
    let mut a = String::from("hello");
    a = "aa".to_string();
    let b = &mut a;
    *b = "aaa".to_string();
    println!("{} ", b);
}
