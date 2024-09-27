set_project("seu-compiler")

set_languages("c++latest")
set_warnings("allextra")

add_rules("mode.debug", "mode.release")
-- add_rules("c.unity_build")
-- add_rules("c++.unity_build")

-- add_requires("vcpkg::fmt", {alias = "fmt"})
-- add_requires("vcpkg::argparse", {alias = "argparse"})
-- add_requires("vcpkg::tl-ranges", {alias = "tl-ranges"})
add_requires("tl-ranges")
add_requires("fmt")
add_requires("argparse")

add_sysincludedirs("vcpkg_installed/x64-mingw-static/include")

set_rundir("output")
before_run(function ()
    os.mkdir("output")
end)

includes(
    "packages/ctoy-grammar",
    "packages/ctoy-lex",
    "packages/ctoy-lex-cli",
    "packages/ctoy-syn",
    "packages/ctoy-syn-cli",
    "packages/ctoy-utils"
)
