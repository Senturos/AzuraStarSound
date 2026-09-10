-- CommonLibSSE-NG plugin project
includes("lib/commonlibsse-ng")

set_project("AzuraStarSound")
set_version("1.0.0")
set_languages("c++23")
set_warnings("allextra")

add_rules("mode.release")

target("AzuraStarSound")
    add_rules("commonlibsse-ng.plugin", {
        name = "Azura Star Sound",
        author = "Senturos",
        description = "Makes the actual vanilla Azura's Star emit a looping positional sound while loaded in the world."
    })
    add_files("src/**.cpp")
