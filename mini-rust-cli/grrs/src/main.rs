use clap::Parser;

/// Search for a pattern in a file and display the lines that contain it.
#[derive(Parser)]
struct Cli {
    /// The pattern to look for
    pattern: String,
    /// The path to the file to read
    path: std::path::PathBuf,
}

fn main() {
    let args = Cli::parse();
    let contents = std::fs::read_to_string(&args.path).expect("could not read file");
    for line in contents.lines() {
        if line.contains(&args.pattern) {
            println!("{}", line);
        }
    }

    println!("pattern: {:?}, path: {:?}", args.pattern, args.path)
}
