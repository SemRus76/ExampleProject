import qbs
import QbsUtl

Product
{
    name: "Client (desktop)"
    type: "application"
    condition: true;
    consoleApplication: false
    destinationDirectory: "./bin"

    Depends { name: "cpp"       }
    Depends { name: "Commands"  }
    Depends { name: "PProto"    }
    Depends { name: "RapidJson" }
    Depends { name: "SharedLib" }
    Depends { name: "Yaml"      }
    Depends { name: "Qt"; submodules: ["core", "widgets", "network"]}

    cpp.defines: project.cppDefines;

    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    // Декларация для подавления Qt warning-ов
    cpp.systemIncludePaths: Qt.core.cpp.includePaths

    Group {
        name: "Resourses / Ресуры"
        prefix: "resourses/"
        files: [
         "resourses.qrc",
        ]
    }

    files: [
        "project_terminal.cpp",

    ]
    qbs.install: true
    qbs.installDir: "bin"

}
