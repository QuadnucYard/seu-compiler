set_project("seu-compiler")

set_languages("c++20")
set_warnings("all", "error")
add_cxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")
add_mxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")
if is_plat("windows") then
    add_cxflags("/wd4819") -- Add this if using msvc
end

add_rules("mode.debug", "mode.release")
add_rules("c.unity_build")
add_rules("c++.unity_build")

install_package = function (package_path)
    return function () 
        add_deps("cmake")
        set_sourcedir(path.join(os.scriptdir(), package_path))
        on_install(function (package)
            local configs = {}
            table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
            table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
            import("package.tools.cmake").install(package, configs)
        end)
    end
end

-- package("dynamic_bitset", install_package("ext/dynamic_bitset"))
package("tl-ranges", install_package("ext/tl-ranges"))

add_requires("fmt", "tl-ranges")

add_includedirs("include")
add_includedirs("ext/tl-ranges/include")

target("lex")
    set_kind("binary")
    add_files("src/common/*.cpp", "src/lex/*.cpp")
    add_packages("fmt")

target("yacc")
    set_kind("binary")
    add_files("src/common/*.cpp", "src/yacc/*.cpp")
    add_packages("fmt", "tl-ranges")

target("test_utils")
    add_files("test/test_utils.cpp")
    add_packages("fmt")

target("test_graph")
    add_files("test/test_graph.cpp")
    add_packages("fmt")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

