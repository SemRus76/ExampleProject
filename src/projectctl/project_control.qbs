import qbs
import QbsUtl

Application
{
    name: "Server"
//    type: "application"
    condition: true;
    consoleApplication: true
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
    Depends { name: "SharedLib" }
    Depends { name: "Yaml" }
    Depends { name: "Qt"; submodules: ["core", "network", "sql"]}

    property var cppDefinesProduct:
    {
        var def = project.cppDefines;

        def.push("IMAGE_SIZE=1920")

        return def
    }

    cpp.defines: cppDefinesProduct;

    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    // Декларация для подавления Qt warning-ов
    cpp.systemIncludePaths: Qt.core.cpp.includePaths

    files: [
        "main.cpp",
    ]
    qbs.install: true
    qbs.installDir: "bin"

}
