pub mod utils;
use clap::{command, Arg, Command};
use utils::{init_project, new_project};
fn main() {
    let starter_config = command!()
        .propagate_version(true)
        .subcommand_required(true)
        .arg_required_else_help(true)
        .subcommand(
            Command::new("new")
                .about("Create a new project in the specified directory")
                .arg(
                    Arg::new("project_root_dir")
                        .short('p')
                        .long("project_root_dir")
                        .value_name("DIR")
                        .value_parser(clap::builder::NonEmptyStringValueParser::new())
                        .help("The root dir of the project")
                        .required(true),
                ),
        )
        .subcommand(Command::new("init").about("Init the project in the current directory"))
        .get_matches();

    match starter_config.subcommand() {
        Some(("new", sub_matches)) => {
            new_project(sub_matches.get_one::<String>("project_root_dir").unwrap())
        }
        Some(("init", _)) => init_project(),
        _ => println!("Please specify a subcommand"),
    }
}
