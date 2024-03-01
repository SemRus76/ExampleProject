import qbs
import "project_base.qbs" as ProjectBase

ProjectBase
{
    name: "Project (Server)"
    condition: true;

    references:[
        "src/projectctl/project_control.qbs",
        "src/projectterm/project_terminal.qbs",
        "src/shared/shared.qbs",
        "src/yaml/yaml.qbs",
        "src/rapidjson/rapidjson.qbs",
        "src/pproto/pproto.qbs",
        "src/commands/commands.qbs",
    ]

//    InstallPackage
//    {
//        name: "Package"
//        type: "application"

////        type: "tar"
//        archiver.name: "Package"
//        archiver.type: "tar"
//        Depends {name: "Server"}
//    }
}
