

target("rtl")
    set_kind("static")
    set_targetdir("../../output-$(vplatform)/common")
    set_filename("librtl.a")
    set_toolchains("clang-elf")
    add_includedirs("../includes")
    add_files("src/*.c")
    if is_config("varch", "riscv64") then
        add_cflags("-target riscv64-none-unknown-elf -mcmodel=medany", {force = true})
        add_asflags("-target riscv64-none-unknown-elf -mcmodel=medany", {force = true})
    end
    add_cflags("-ffreestanding -nostdlib -Wall -Wextra -Werror", {force = true})
    add_asflags("-ffreestanding -nostdlib -Wall -Wextra -Werror", {force = true})
