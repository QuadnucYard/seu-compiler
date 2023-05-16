set_project("seu-compiler")

set_languages("c++20")
-- set_warnings("all", "error")

if is_plat("windows") then
    add_cxflags("/wd4819") -- Add this if using msvc
else
    add_cxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")
    add_mxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")
end

add_rules("mode.debug", "mode.release")
add_rules("c.unity_build")
add_rules("c++.unity_build")

function install_package(package_path)
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

add_requires("vcpkg::fmt", {alias = "fmt"})
add_requires("tl-ranges")

add_includedirs("include")
add_includedirs("ext/tl-ranges/include")

-- add_linkdirs("E:/Program Files/vcpkg/installed/x64-windows-static/lib")

-- add_defines("FMT_HEADER_ONLY")

function add_test_target(...)
    for _, name in ipairs{...} do
        target("test_" .. name, function () 
            add_files("test/test_" .. name .. ".cpp")
            add_packages("fmt")
        end)
    end
end

function add_template(path, export_name)
	before_build(function(target)
        print("before_build")
		local fin = io.open(path, "r")
		local fout = io.open("$(buildir)/@.cpp", "w")
		assert(fin)
		assert(fout)
		fout:write("const char* " .. export_name .. " = \n")
		for line in fin:lines() do
			fout:write("R\"||(" .. line .. ")||\"\"\\n\"\n")
		end
		fout:write(";")
		fin:close()
        fout:close()
	end)
    add_files("$(buildir)/@.cpp")
end

add_test_target("utils", "graph")

target("lexer")
    set_kind("static")
    add_files("src/common/*.cpp", "src/lex/*.cpp|lex.cpp")
    add_packages("fmt")

target("lex")
    add_deps("lexer")
    add_files("src/lex/lex.cpp")

target("parser")
    set_kind("static")
    add_files("src/common/*.cpp", "src/yacc/*.cpp|yacc.cpp")
    add_packages("fmt", "tl-ranges")

target("yacc")
    add_deps("parser")
    add_files("src/yacc/yacc.cpp")


target("test_fa")
    add_deps("lexer")
    add_files("test/test_fa.cpp")

target("example_templater")
    add_template("templates/lex.yy.c", "TEMPLATE")
    add_files("examples/templater.cpp")


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

