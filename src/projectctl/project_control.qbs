import qbs
import QbsUtl

Product
{
    type: "application"
    consoleApplication: true
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
//    Depends { name: "SharedLib" }
//    Depends { name: "Yaml" }
    Depends { name: "Qt"; submodules: ["core", "network", "sql"]}

    cpp.defines: project.cppDefines
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    // Декларация для подавления Qt warning-ов
    cpp.systemIncludePaths: Qt.core.cpp.includePaths

    files: [
        "main.cpp",
    ]
}
