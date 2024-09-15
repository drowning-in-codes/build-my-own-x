use std::fmt::{self, Display};
#[derive(Debug, Clone)]
pub enum ProjectType {
    Cpp,
}

#[derive(Debug, Clone)]
pub enum PackageManager {
    VCPKG,
    CPM,
    None,
}
impl Display for PackageManager {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            PackageManager::VCPKG => write!(f, "VCPKG"),
            PackageManager::CPM => write!(f, "CPM"),
            PackageManager::None => write!(f, "None"),
        }
    }
}
impl Display for ProjectType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            ProjectType::Cpp => write!(f, "C/C++"),
        }
    }
}

#[derive(Debug, Clone)]
pub enum TargetType {
    Executable,
    StaticLibrary,
    DynamicLibrary,
}
impl Display for TargetType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            TargetType::Executable => write!(f, "Executable"),
            TargetType::StaticLibrary => write!(f, "Static Library"),
            TargetType::DynamicLibrary => write!(f, "Dynamic Library"),
        }
    }
}

#[derive(Debug, Clone)]
pub struct ProjectInfo {
    pub project_name: String,
    pub project_version: String,
    pub project_desc: String,
    pub project_type: ProjectType,
    pub target_type: TargetType,
    pub package_manager: PackageManager,
}
