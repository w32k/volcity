includes("volkrnl")
includes("common/rtl")

option("varch")
    set_values("riscv64")

option("vplatform")
    set_values("qvirt-riscv64")



target("volcity")
    set_kind("phony")
    set_options("varch", "vplatform")
    add_deps("volkrnl")




toolchain("clang-elf")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("as", "clang")
    set_toolset("ld", "ld.lld")
toolchain_end()
