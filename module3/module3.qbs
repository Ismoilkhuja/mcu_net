import qbs
import qbs.FileInfo
import qbs.ModUtils


CppApplication {

    name: "module3"
    consoleApplication: true
    type: ["application", "hex"]

    cpp.positionIndependentCode: false

    cpp.cLanguageVersion: "c11"

    cpp.executableSuffix: ".elf"

    cpp.includePaths:[
        "Inc"
    ]

    cpp.defines:[
        "F_CPU=16000000"
    ]

    cpp.driverFlags:[
        "-mmcu=atmega328p"
    ]

    cpp.optimization: "small"

    /**

    cpp.cFlags:[
        "-ffunction-sections",
        "-fdata-sections",
        "-flto",
        "-fno-fat-lto-objects",
        "-MMD"
    ]

    cpp.linkerFlags: [
        "--gc-sections",
    ]

    */


    Group {
        name: "Src"
        prefix: "Src/"
        files: [
            "*.c"
        ]
    }

    Group {
        name: "Inc"
        prefix: "Inc/"
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
            var hexcreator = "avr-objcopy"
            var cmd = new Command(hexcreator,args);
            cmd.description = 'creating ' + project.name + '.hex'
            return cmd;
        }
    }


}


