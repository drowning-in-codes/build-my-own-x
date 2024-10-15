fn main() {
    let input = 'x';
    match input {
        'q' => println!("Quit"),
        'a' | 's' | 'w' | 'd' => println!("Moving around"),
        '0'..='9' => println!("Number input"),
        key if key.is_lowercase() => println!("Lowercase: {key}"),
        _ => println!("something else"),
    }
}
