import qbs

CppApplication {
    name: "module1"
    type : ["application", "hex", "bin"]
    cpp.cLanguageVersion: "c99"
    cpp.executableSuffix: ".elf"
    cpp.positionIndependentCode: false
    cpp.includePaths: [
        "Core/Inc",
        "Drivers/CMSIS/Device/ST/STM32F4xx/Include",
        "Drivers/CMSIS/Include"
    ]

    cpp.defines: [
        "STM32F401xC"
    ]
    cpp.assemblerFlags: [
        "-mcpu=cortex-m4",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=hard",
        "-mthumb"
    ]

    cpp.driverFlags: [
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=hard",
        "-specs=nano.specs"
    ]
    cpp.cFlags: [
        "-fdata-sections",
        "-ffunction-sections"
    ]

    cpp.driverLinkerFlags: [
        "-specs=nosys.specs",
        "-Wl,--gc-sections",
        "-lc",
        "-lm"
    ]

    Group {
        name: "Startup"
        fileTags: ["asm"]
        files: [
            "*.s",
        ]
    }

    Group {
        name: "Linker Script"
        fileTags: ["linkerscript"]
        files: ["*.ld"]
    }

    Group{
        name:"Src"
        prefix:"Core/Src/"
        files: [
            "*.c"
        ]
    }

    Group{
        name:"Inc"
        prefix:"Core/Inc/"
        files: [
            "*.h"
        ]
    }

    Rule{
        inputs: ['application']
        Artifact{
            fileTags:['hex']
            filePath: project.name + '.hex'
        }
        prepare:{
            var args = []
            args.push("-O")
            args.push("ihex")
            args.push(input.filePath)
            args.push(output.filePath)
            var hexcreator = "arm-none-eabi-objcopy"
            var cmd = new Command(hexcreator,args);
            cmd.description = 'creating ' + project.name + '.hex'
            return cmd;
        }
    }

    Rule{
        inputs: ['application']
        Artifact{
            fileTags:['bin']
            filePath: project.name + '.bin'
        }
        prepare:{
            var args = []
            args.push("-O")
            args.push("binary")
            args.push(input.filePath)
            args.push(output.filePath)
            var bincreator = "arm-none-eabi-objcopy"
            var cmd = new Command(bincreator,args);
            cmd.description = 'creating ' + project.name + '.bin'
            return cmd;
        }
    }
}
