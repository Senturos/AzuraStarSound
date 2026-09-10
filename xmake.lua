includes("lib/commonlibsse-ng")

set_project("AzuraStarSound")
set_version("1.0.1")
set_languages("c++23")
set_warnings("allextra")

add_rules("mode.debug", "mode.releasedbg", "mode.release")

target("AzuraStarSound")
    set_kind("shared")
    set_arch("x64")
    add_deps("commonlibsse-ng")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
