



target("volkrnl")
    set_toolchains("clang-elf")
    add_deps("rtl")
    set_targetdir("../output-$(vplatform)/system")
    set_filename("volkrnl.elf")
    add_includedirs("includes", "../common/includes")
    add_files("src/rtl/*.c", "src/kernel/*.c", "src/device/fdt/*.c", "src/memory/*.c", "src/exec/*.c")
    add_files("src/fs/**/*.c")
    add_files("src/platform/$(vplatform)/*.S",
              "src/platform/$(vplatform)/*.c")
    add_files("src/arch/$(varch)/*.S",
              "src/arch/$(varch)/*.c")
    if is_config("varch", "riscv64") then
        add_cflags("-target riscv64-none-unknown-elf -mcmodel=medany", {force = true})
        add_asflags("-target riscv64-none-unknown-elf -mcmodel=medany", {force = true})
    end
    add_cflags("-ffreestanding -nostdlib -Wall -Wextra -Werror",
                    "-DPLATFORM=\"$(vplatform)\" -DARCH=\"$(varch)\"", {force = true})
    add_asflags("-ffreestanding -nostdlib -Wall -Wextra -Werror", {force = true})
    add_ldflags("-nostdlib -Tvolkrnl/linker/$(vplatform)/linker.ld", {force = true})
