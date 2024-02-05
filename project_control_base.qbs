import qbs
import "project_base.qbs" as ProjectBase

ProjectBase
{
    name: "Project (Server)"

    references:[
        "src/projectctl/project_control.qbs",
        "src/shared/shared.qbs",
        "src/yaml/yaml.qbs",
    ]
}
