pub mod project_info;
use dialoguer::Input;
use dialoguer::{theme::ColorfulTheme, Confirm, Select};
use project_info::{PackageManager, ProjectInfo, ProjectType, TargetType};
use std::fs::File;
use std::io::Write;
use std::path::Path;
use std::{env, fs};
use serde_json::{json, Value};
use ureq::Error;

fn create_file(tgt_path: &Path, temp_path: &Path) {
    let src_file = File::create(tgt_path).unwrap();
    match fs::read_to_string(temp_path.parent().unwrap().join("templates").join(temp_path.file_name().unwrap())) {
        Ok(contents) => write!(&src_file, "{}",contents).unwrap(),
        Err(e) => eprintln!("{} Error reading file: {}",line!(),e),
    }
}
fn create_from_temp(tgt_path: &Path, temp_path: &Path,info:&ProjectInfo) {
    let src_file = File::create(tgt_path).unwrap();
    match fs::read_to_string(temp_path.parent().unwrap().join("templates").join(temp_path.file_name().unwrap())) {
        Ok(contents) => 
        {
            let contents = contents.replace("@PROJECT_NAME@", &info.project_name.clone()) 
                .replace("@PROJECT_VERSION@", &info.project_version.clone()).replace("@PROJECT_DESCRIPTION@", &info.project_desc.clone());
            write!(&src_file,"{}",contents).unwrap();
        }
        Err(e) => eprintln!("{} Error reading file: {}",line!(),e),
    }
}
fn copy_dir(tgt_path: &Path, src_path: &Path) {
    let entries = fs::read_dir(src_path).unwrap();
    for entry in entries {
        // 对于模板中每一个文件或者目录
        let entry = entry.unwrap();
        let path = entry.path();
        let file_name = path.file_name().unwrap();
        let tgt_path = tgt_path.join(file_name);
        if path.is_dir() {
            if !tgt_path.exists() {
                fs::create_dir(&tgt_path).unwrap();
            }
            copy_dir(&tgt_path, &path);
        } else {
            let tgt_file = File::create(&tgt_path).unwrap();
            match fs::read_to_string(&path) {
                Ok(contents) => write!(&tgt_file, "{}", contents).unwrap(),
                Err(e) => eprintln!("Error reading file: {}", e),
            }
        }
    }
}

pub fn init_project() {
    generate_project(&env::current_dir().unwrap());
}
pub fn new_project(dir_path: &str) {
    // 创建项目,指定目录
    let mut dir_path = Path::new(dir_path).to_path_buf();
    if !dir_path.is_absolute() {
        dir_path = env::current_dir().unwrap().join(dir_path);
    }
    fs::create_dir_all(&dir_path).unwrap();
    generate_project(&dir_path);
}

fn generate_project(root_dir: &Path) {
    if let Some(info) = get_project_info(root_dir) {
    // - root_dir
    //    - project_name
    //      - src
    //      - include
    //        - project_name
    //      - CMakelists.txt
    //      - .gitignore
    //      - README.md
    //      - LICENSE
    //      - CMakePresets.json
    //      - cmake
    //      - tests
    //      - doc
    let project_name = &info.project_name;
    // 创建src目录与main.cpp
    match fs::create_dir_all(root_dir.join(project_name).join("src")) {
            Ok(_)=>{},
            Err(e)=>{
                eprintln!("{}:{}",e.kind(),e);
            }
        }
            let src_file = File::create(root_dir.join(project_name).join("src/main.cpp")).unwrap();
            match fs::read_to_string(env::current_dir().unwrap().join("templates/main.cpp")) {
                Ok(contents) => write!(&src_file, "{}", contents).unwrap(),
                Err(e) => eprintln!("Error reading file: {}", e),
            }
        
        //创建tests目录和doc目录
    let test_dir = root_dir.join("tests");
        if !test_dir.exists() {
            fs::create_dir(&test_dir).unwrap();
        }
    let doc_dir = root_dir.join("doc");
        if !doc_dir.exists() {
            fs::create_dir(&doc_dir).unwrap();
        }
    // 创建include目录
    fs::create_dir_all(root_dir.join(project_name).join("include").join(project_name)).unwrap();
    //创建.gitignore文件
    create_file(
        &root_dir.join(".gitignore"),
        &env::current_dir().unwrap().join("gitignore.txt"),
    );
    //创建README.md文件
    create_file(
        &root_dir.join("README.md"),
        &env::current_dir().unwrap().join("README.md"),
    );
    // 创建LICENSE文件
    create_file(
        &root_dir.join("LICENSE"),
        &env::current_dir().unwrap().join("LICENSE.txt"),
    );
    // 创建CMakePresets.json文件
    create_file(
        &root_dir.join("CMakePresets.json"),
        &env::current_dir().unwrap().join("CMakePresets.json"),
    );
    //拷贝cmake目录
        let cmake_dir = root_dir.join("cmake");
    if !cmake_dir.exists() {
        fs::create_dir(&cmake_dir).unwrap();
        }
    copy_dir(
        &root_dir.join("cmake"),
        &env::current_dir().unwrap().join("templates/cmake"),
    );

    match info.target_type {
        TargetType::Executable => {
            // 创建可执行程序项目
            //创建CMakeLists.txt文件
            create_from_temp(
                &root_dir.join("CMakeLists.txt"),
                &env::current_dir().unwrap().join("CMakeLists_exe.txt.in"),
                &info
            );
        }
        TargetType::StaticLibrary => {
            //创建CMakeLists.txt文件
            create_from_temp(
                &root_dir.join("CMakeLists.txt"),
                &env::current_dir().unwrap().join("CMakeLists_static.txt.in"),
                &info
            );
        }
        TargetType::DynamicLibrary => {
            create_from_temp(
                &root_dir.join("CMakeLists.txt"),
                &env::current_dir().unwrap().join("CMakeLists_shared.txt.in"),
                &info
            );

        }
    }
    match info.package_manager {
        PackageManager::VCPKG => {
            // 默认在CMAKE_TOOLCHAIN_FILE中指定vcpkg路径
            let presets_path = root_dir.join("CMakePresets.json");
            let content = fs::read_to_string(&presets_path).unwrap();
            let mut p:Value = serde_json::from_str(&content).unwrap();
            p["configurePresets"][0]["cacheVariables"]["CMAKE_TOOLCHAIN_FILE"] = json!("$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake");
            fs::write(&presets_path, p.to_string()).unwrap();
        },
        PackageManager::CPM => {
            // 下载cpm.cmake到cmake目录中
            match ureq::get("https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake").call(){
                Ok(response) => {
                    fs::write(env::current_dir().unwrap().join("cmake/cpm.cmake"),response.into_string().unwrap()).unwrap();
                },
                Err(Error::Status(code,response))=>{
                    eprintln!("Error: {} {}",code,response.into_string().unwrap());
                },
                Err(_)=>{
                    eprintln!("Error: failed to download cpm.cmake");
                },
            }
        },
            PackageManager::None=>{}
    }
    }
}

pub fn get_project_info(default_root_dir_name: &Path) -> Option<ProjectInfo> {
    let theme = ColorfulTheme::default();
    // 项目名称
    //
    let root_dir_name = default_root_dir_name
                .file_name()
                .unwrap()
                .to_str()
                .unwrap();
    let project_name = Input::<String>::new()
        .with_prompt(r"Enter your project name.(project root folder name if leave blank)")
        .with_initial_text(root_dir_name.to_string())
        .interact_text()
        .unwrap_or(root_dir_name.to_string());

    // 项目版本
    let project_version = Input::<String>::new()
        .with_prompt("Enter your project version.(default: 0.1.0)")
        .with_initial_text("0.1.0".to_string())
        .interact_text()
        .unwrap_or("0.1.0".to_string());

    // 项目描述
    let project_desc = Input::<String>::new()
        .with_prompt("Enter your project description.(default: A new project)")
        .with_initial_text("A new project".to_string())
        .interact_text()
        .unwrap_or("A new project".to_string());

    // 选择项目类型
    let project_types = vec![ ProjectType::Cpp];
    let project_type_selection = Select::with_theme(&theme)
        .with_prompt("choose the peoject type")
        .items(&project_types)
        .interact()
        .unwrap();

    // 选择生成目标类型
    let target_type = vec![
        TargetType::Executable,
        TargetType::StaticLibrary,
        TargetType::DynamicLibrary,
    ];
    let target_type_selection = Select::with_theme(&theme)
        .with_prompt("choose the target type")
        .items(&target_type)
        .interact()
        .unwrap();

    // 选择包管理器
    let package_manager_list = vec![PackageManager::VCPKG, PackageManager::CPM,PackageManager::None];
    let package_manager_selection = Select::with_theme(&theme)
        .with_prompt("choose the package manager ")
        .items(&package_manager_list)
        .interact()
        .unwrap();
    let package_manager = package_manager_list[package_manager_selection].clone();

    let project_type = project_types[project_type_selection].clone();
    let target_type = target_type[target_type_selection].clone();
    let confirmation = Confirm::with_theme(&theme)
        .with_prompt(format!(
            "Default LICENSE: MIT\nConfiguration as below:\nproject_name: {}\nproject_version: {}\nproject_desc: {}\nproject_type: {}\ntarget_type: {}\npackage manager: {}\n \
                Do you want to continue?",
            project_name,project_version,project_desc, 
            project_type, target_type,package_manager,))
        .interact()
        .unwrap();

    if confirmation {
        println!("Great! Let's get started!");
    } else {
        println!("nevermind then :(");
        return None;
    }
    Some(ProjectInfo {
        project_name,
        project_version,
        project_desc,
        project_type,
        target_type,
        package_manager,
    })
}
