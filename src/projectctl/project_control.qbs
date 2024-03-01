import qbs
import QbsUtl

Product
{
    name: "Server"
    type: "application"
    condition: true;
    consoleApplication: true
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
    Depends { name: "Commands"}
    Depends { name: "PProto"}
    Depends { name: "RapidJson"}
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
        "application.cpp",
        "application.h",
        "main.cpp",
    ]
    qbs.install: true
    qbs.installDir: "bin"

}
