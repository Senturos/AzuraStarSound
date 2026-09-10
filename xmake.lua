includes("lib/commonlibsse-ng")

set_project("AzuraStarSound")
set_version("1.0.0")
set_languages("c++23")
set_warnings("allextra")

add_rules("mode.debug", "mode.releasedbg", "mode.release")

target("AzuraStarSound")
    add_deps("commonlibsse-ng")
    add_rules("commonlibsse-ng.plugin", {
        name = "Azura Star Sound",
        author = "Senturos",
        description = "Makes the actual vanilla Azura's Star emit a looping positional sound while loaded in the world."
    })
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
